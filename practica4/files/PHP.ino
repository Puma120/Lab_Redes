#include <WiFi.h>        // Librería para WiFi
#include <HTTPClient.h>  // Librería para solicitudes HTTP
#include <DHT.h>         // Librería para el sensor DHT22

// ------------------------ Puertos GPIO (pines del ESP32) ------------------------
int pot   = 32;  // Pin donde está conectado el potenciómetro
int boton = 25;  // Pin donde está conectado el botón
int PIR   = 26;  // Pin donde está conectado el sensor de movimiento (PIR)

// ------------------------ Configuración del sensor DHT22 ------------------------
DHT dht(33, DHT22); // Pin 33 con un DHT22

// ------------------------ URLs y Strings ----------------------------------------
String url        = "https://actividad4labredes.onrender.com/";
String thinkspeak = "https://api.thingspeak.com/update?api_key=7TVR7VIG20OBP2RC&field";

// Nombres de sensores, tal como en tu código
String sensor1 = "Boton";
String sensor2 = "Potenciometro";
String sensor3 = "Temperatura";
String sensor4 = "Humedad";
String sensor5 = "Movimiento";
String sensores[5] = {sensor1, sensor2, sensor3, sensor4, sensor5};

// Arreglo para guardar los valores leídos
String valores[5];

// Variables para guardar la lectura de temperatura/humedad
int valor_temperatura = 0;
int valor_humedad     = 0;

// Tarea para enviar datos a tu servidor (spock)
void taskEnviarSpock(void *pvParameters) {
  (void) pvParameters;

  for(;;) {
    // Verifica que siga conectado a WiFi
    if(WiFi.status() == WL_CONNECTED) {
      valores[0] = String(digitalRead(boton));      // Botón
      valores[1] = String(analogRead(pot));         // Potenciómetro
      valores[4] = String(digitalRead(PIR));        // PIR (movimiento)
      HTTPClient http;
      String message;

      message = url + "?" 
              + sensores[0] + "=" + valores[0] + '&'
              + sensores[1] + "=" + valores[1] + '&'
              + sensores[4] + "=" + valores[4];

      // Iniciar la petición HTTP con la URL generada
      http.begin(message);
      int httpCode = http.GET();
      
      // Mostrar la URL y el código de respuesta en el monitor serie
      Serial.println(message);
      Serial.print("GET code spock: ");
      Serial.println(httpCode);

      // Finalizar la conexión HTTP
      http.end();
    }
    
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}


void taskEnviarThingSpeak(void *pvParameters) {
  (void) pvParameters;

  for(;;) {
    // Verifica que siga conectado a WiFi
    if(WiFi.status() == WL_CONNECTED) {
      valor_temperatura = dht.readTemperature();
      valor_humedad     = dht.readHumidity();
      valores[2] = String(valor_temperatura);       // Temperatura
      valores[3] = String(valor_humedad);           // Humedad
      HTTPClient http;
      String message;

      message = thinkspeak + "1" + "=" + valores[2] + "&field" + "2" + "=" + valores[3];     // Temperatura -> field2

      // Iniciar la petición HTTP GET
      http.begin(message);
      int httpCode = http.GET();

      // Mostrar la URL y el código de respuesta en el monitor serie
      Serial.println(message);
      Serial.print("GET code TS: ");
      Serial.println(httpCode);

      // Finalizar la conexión HTTP
      http.end();
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// -----------------------------------------------------------------------------
// setup(): igual que tu versión original, solo que al final creamos las tasks
void setup() {
  delay(1000);           
  Serial.begin(115200);  

  pinMode(boton, INPUT);
  pinMode(pot,   INPUT);
  pinMode(PIR,   INPUT);

  dht.begin();

  // Conectar a la red WiFi
  WiFi.begin("MSI 9472", "|05791xZ"); 

  // Esperar hasta que la conexión se establezca
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected"); 

  // ---------------- Creación de las 2 tareas (threads) ----------------
  xTaskCreate(
    taskEnviarSpock,              // Función que implementa la tarea
    "TaskEnviarSpock",            // Nombre para debug
    4096,                         // Tamaño de la pila
    NULL,                         // Parámetros (no usado aquí)
    1,                            // Prioridad de la tarea
    NULL                          // Handle de la tarea (opcional)
  );

  xTaskCreate(
    taskEnviarThingSpeak,
    "TaskEnviarThingSpeak",
    4096,
    NULL,
    1,
    NULL
  );
}

void loop() {
}
