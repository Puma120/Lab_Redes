// Librerías necesarias
#include <DHT.h>       // Librería para el sensor DHT22
#include <WiFi.h>      // Librería para la conexión WiFi
#include <HTTPClient.h> // Librería para hacer solicitudes HTTP

// Definición de pines GPIO
int led = 32;  // Pin donde está conectado el LED

// Configuración del sensor DHT22
DHT dht(33, DHT22); // DHTPIN (Pin 33), DHTTYPE (Sensor DHT22)

// Variables para almacenar los valores de temperatura y humedad
int valor_temperatura = 0;
int valor_humedad = 0;

// Configuración para enviar datos a ThingSpeak
String url = "https://api.thingspeak.com/update?"; // URL del servidor de ThingSpeak
String api_key = "XTUNK0M3XFB5YD9C"; // Clave API personal del tablero en ThingSpeak
String sensor = "field3"; // Campo donde se enviarán los datos de humedad

void setup() {
  // Configuración de los pines
  pinMode(led, OUTPUT);  // Configura el pin del LED como salida
  dht.begin();           // Inicializa el sensor DHT22

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

  // Leer valores de temperatura y humedad desde el sensor DHT22
  valor_temperatura = dht.readTemperature();
  valor_humedad = dht.readHumidity();

  // Mostrar los valores en la consola serie
  Serial.print("Temperatura: ");
  Serial.println(valor_temperatura);
  Serial.print("Humedad: ");
  Serial.println(valor_humedad);

  // Encender el LED si la humedad es mayor o igual al 50%
  if (valor_humedad >= 50) { 
    digitalWrite(led, HIGH);  // Enciende el LED
  } else {
    digitalWrite(led, LOW);   // Apaga el LED
  }

  // Verifica si el ESP32 está conectado a WiFi antes de enviar datos
  if ((WiFi.status() == WL_CONNECTED)) {
    // Crear un objeto HTTPClient para la solicitud HTTP
    HTTPClient http;

    // Construir la URL con los datos de humedad
    String message = url + "api_key=" + api_key + "&" + sensor + "=" + String(valor_humedad);
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
