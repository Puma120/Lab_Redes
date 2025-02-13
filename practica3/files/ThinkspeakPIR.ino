// Librerías necesarias
#include <WiFi.h>      // Librería para la conexión WiFi
#include <HTTPClient.h> // Librería para hacer solicitudes HTTP

// Definición de pines GPIO
int led = 32;  // Pin donde está conectado el LED
int PIR = 33;  // Pin donde está conectado el LED

//Variables globales
int pirState = LOW;            // Variable para considerar que el movimiento esta en curso
int val = 0;  

// Configuración para enviar datos a ThingSpeak
String url = "https://api.thingspeak.com/update?"; // URL del servidor de ThingSpeak
String api_key = "XTUNK0M3XFB5YD9C"; // Clave API personal del tablero en ThingSpeak
String sensor = "field4"; // Campo donde se enviarán los datos de movimiento

void setup() {
  // Configuración de los pines
  pinMode(led, OUTPUT);  // Configura el pin del LED como salida
  pinMode(PIR, INPUT);   // Declarar sensor PIR como entrada

  delay(1000);  // Pequeña pausa antes de iniciar la conexión
  Serial.begin(115200);  // Inicia la comunicación serie a 115200 baudios
    
  // Conexión a la red WiFi
  WiFi.begin("MSI 9472", "|05791xZ");  // Nombre de la red WiFi y contraseña (vacía en este caso)

  // Esperar hasta que se establezca la conexión WiFi
  while ((WiFi.status() != WL_CONNECTED)) {
    delay(500);
    Serial.print(".");  // Muestra puntos en la consola mientras se conecta
  }   
  Serial.println("WiFi conectado");  // Mensaje de confirmación de conexión
}

void loop() {
  delay(1000); // Pequeña pausa entre lecturas

  val = digitalRead(PIR);  // Leer el estado del sensor PIR

  if (val == HIGH) {            // Si se detecta movimiento
        digitalWrite(led, HIGH); // Encender LED
        if (pirState == LOW) {
            Serial.println("¡Movimiento detectado!");
            pirState = HIGH;
        }
    } else {
        digitalWrite(led, LOW); // Apagar LED
        if (pirState == HIGH) {
            Serial.println("¡Movimiento finalizado!");
            pirState = LOW;
        }
    }

  // Verifica si el ESP32 está conectado a WiFi antes de enviar datos
  if ((WiFi.status() == WL_CONNECTED)) {
    // Crear un objeto HTTPClient para la solicitud HTTP
    HTTPClient http;

    // Construir la URL con los de movimiento (en curso o no)
    String message = url + "api_key=" + api_key + "&" + sensor + "=" + String(pirState);
    http.begin(message); // Iniciar la conexión HTTP

    // Realizar la petición GET a ThingSpeak y obtener el código de respuesta
    int httpCode = http.GET();
    Serial.println(message);  // Muestra la URL de la petición en la consola
    Serial.print("Código GET: ");
    Serial.println(httpCode);  // Muestra el código de respuesta de la solicitud HTTP

    http.end();  // Finaliza la conexión HTTP
  }

  delay(1000);  // Espera 1 segundo antes de enviar nuevamente
}
