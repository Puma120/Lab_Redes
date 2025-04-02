#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>  // Librería para el sensor DHT22
#include "time.h"

// ------------------ Ajustes NTP y WiFi ------------------
const char* ntpServer          = "pool.ntp.org"; //Servidor generico para obtener el tiempo
const long  gmtOffset_sec      = -21600; // UTC -6
const int   daylightOffset_sec = 0;

String red        = "";    // SSID de la red a conectar (Se requiere que se acepte )

// ------------------ Pines y objetos para sensores ------------------
#define PIN_LDR  35
#define PIN_SOIL 32
#define PIN_DHT  33

DHT dht(PIN_DHT, DHT22); // Pin 33 para DHT22

// ------------------ Variables globales para datos ------------------
float luzInput        = 0;
float valor_temperatura = 0;
float valor_humedad     = 0;
int   moisturePercent   = 0;

// Ajustes sensor de suelo
const int wetValue = 1352;
const int dryValue = 4095;

// Ajustes LDR
float A = 1000000.0;
float B = 1;
float lux;

// ------------------ Firebase o URL de destino ------------------
String nameDevice = ""; //Introdusca nombre de dispositivo
String url        = ""; //Introdusca un link para poder realizar una operacion post (se ocupo la real time database de firebase)

// ------------------ Temporización de envío ------------------
unsigned long previousMillisEnvio = 0;
// 1 hora = 3600000 ms
const unsigned long intervaloEnvio = 1800000; //Establesca el intervalo a esperar el muestreo

// ------------------ Funciones auxiliares ------------------
String obtenerTiempoISO8601() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Error al obtener la hora local");
    return "false";
  }
  
  // Buffer para la cadena de tiempo
  char timeString[30];
  // Formato: YYYY-MM-DDTHH:MM:SS
  strftime(timeString, sizeof(timeString), "%Y-%m-%dT%H:%M:%S", &timeinfo);
  
  String firebaseTimestamp = String(timeString) + "-06:00";
  return firebaseTimestamp;
}

// ------------------ Tarea de captura de datos ------------------
void tareaCaptura(void *pvParameters) {
  for (;;) {
    // Leer LDR
    int lecturaLuz = analogRead(PIN_LDR);
    // Leer DHT
    float temp = dht.readTemperature();
    float hum  = dht.readHumidity();
    // Leer sensor de suelo
    int sensorValue = analogRead(PIN_SOIL);
    
    // Guardar en variables globales
    luzInput         = float(lecturaLuz);
    valor_temperatura = temp;
    valor_humedad     = hum;

    // Mapear humedad de suelo
    moisturePercent = map(sensorValue, dryValue, wetValue, 0, 100);
    moisturePercent = constrain(moisturePercent, 0, 100);

    // Calcular lux
    float Vadc = (3.3 * luzInput) / 4095.0;
    // Evitar división por cero
    if (Vadc < 0.001) Vadc = 0.001; 
    float Rldr = 1000 * ((3.3 / Vadc) - 1);
    lux = pow((A / Rldr), (1.0 / B));

    // Esperar 2 segundos
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

// ------------------ Tarea de envío de datos ------------------
void tareaEnvio(void *pvParameters) {
  for (;;) {
    // Revisa cada ciclo si ya pasó el intervalo de tiempo
    if ( (millis() - previousMillisEnvio) >= intervaloEnvio ) {

      // Hora
      String timeStamp = obtenerTiempoISO8601();
      if (timeStamp == "false") {
        // Si no se pudo obtener la hora, se reintenta
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        continue;
      }
      

      // Conexión WiFi
      if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(url);
        http.addHeader("Content-Type", "application/json");
        
        // Arma el JSON con los últimos datos capturados
        String jsonPayload = String("{") +
          "\"device\":\"" + nameDevice + "\"," +
          "\"time\":\"" + timeStamp + "\"," +
          "\"light\":" + String(lux) + "," +
          "\"soil_moisture\":" + String(moisturePercent) + "," +
          "\"air_temperature\":" + String(valor_temperatura) + "," +
          "\"air_humidity\":" + String(valor_humedad) +
          "}";

        Serial.println("Enviando datos a Firebase (una vez cada hora):");
        Serial.println(jsonPayload);

        // POST
        int httpResponseCode = http.POST(jsonPayload);
        if (httpResponseCode > 0) {
          String response = http.getString();
          Serial.println("Respuesta de Firebase: " + response);
          // Ya que se obtuvo la hora se puede enviar el dato, por lo cual se actualiza el contador
          previousMillisEnvio = millis(); // Actualiza el contador
        } else {
          Serial.println("Error al enviar datos: " + String(httpResponseCode));
            // Conexión WiFi
          Serial.println("\nWiFi conectado");
        }

        http.end(); // Cierra la conexión HTTP
      } else {
        Serial.println("WiFi no conectado, no se pueden enviar datos en este momento.");\
        
        while (WiFi.status() != WL_CONNECTED){
          WiFi.reconnect();
        }
      }
    }
    // Esperar un poco antes de checar de nuevo. 
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// ------------------ Setup ------------------
void setup() {
  Serial.begin(115200);
  delay(1000);

  // Inicializa DHT
  dht.begin();

  // Pines de entrada
  pinMode(PIN_LDR,  INPUT);
  pinMode(PIN_SOIL, INPUT);
  Serial.println("Encendi");

  // Conexión WiFi
  WiFi.begin(red.c_str());
  while (WiFi.status() != WL_CONNECTED) {

  }
  Serial.println("\nWiFi conectado");

  // Configurar NTP para hora
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  // ------------------ Crear tareas (FreeRTOS) ------------------
  xTaskCreatePinnedToCore(
    tareaCaptura,     // Función que implementa la tarea
    "TareaCaptura",   // Nombre de la tarea
    4096,             // Tamaño de la pila en bytes
    NULL,             // Parámetro que se le pasa a la tarea
    1,                // Prioridad de la tarea
    NULL,             // Manejador de la tarea
    0                 // Núcleo donde se ejecutará (0 ó 1)
  );

  xTaskCreatePinnedToCore(
    tareaEnvio,       
    "TareaEnvio",     
    4096,
    NULL,
    1,
    NULL,
    1
  );
}

void loop() {
  // Como hay hilos no es necesario
}
