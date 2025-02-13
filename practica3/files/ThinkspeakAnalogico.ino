#include <WiFi.h>  // Incluye la librería WiFi para la conexión a redes
#include <HTTPClient.h>  // Librería para realizar peticiones HTTP

// Puertos GPIO (pines del ESP32)
int pot = 33;  // Pin donde está conectado el potenciómetro
int led = 32;  // Pin donde está conectado el LED

// Variable para almacenar el valor analógico leído del potenciómetro
int valor_analogico = 0;

// Configuración para enviar datos a ThingSpeak
String url = "https://api.thingspeak.com/update?"; // URL del servidor de ThingSpeak
String api_key = "XTUNK0M3XFB5YD9C"; // Clave API personal del tablero en ThingSpeak
String sensor = "field2"; // Campo donde se enviará el dato del potenciómetro

void setup() {
  // Configuración de los pines
  pinMode(led, OUTPUT);  // Configura el pin del LED como salida

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
  // Leer el valor analógico del potenciómetro
  valor_analogico = analogRead(pot);
  // Serial.println(valor_analogico); // Descomentar para ver el valor en la consola

  // Encender o apagar el LED según el valor del potenciómetro
  if (valor_analogico >= 2048) {  // Si el valor es mayor o igual a la mitad del rango (0-4095)
    digitalWrite(led, HIGH);  // Enciende el LED
  } else {
    digitalWrite(led, LOW);   // Apaga el LED
  }
  
  // Verifica si el ESP32 está conectado a WiFi antes de enviar datos
  if ((WiFi.status() == WL_CONNECTED)) {
    // Crear un objeto HTTPClient para la solicitud HTTP
    HTTPClient http;

    // Construir la URL con los datos del potenciómetro
    String message = url + "api_key=" + api_key + "&" + sensor + "=" + String(valor_analogico);
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
