#include <WiFi.h>  // Incluye la librería WiFi para la conexión a redes
#include <HTTPClient.h>  // Librería para realizar peticiones HTTP

// Puertos GPIO (pines del ESP32)
int boton = 33;  // Pin donde está conectado el botón
int led = 32;    // Pin donde está conectado el LED

// Variable para almacenar el estado del botón
int status_boton = 0;

// Configuración para enviar datos a ThingSpeak
String url = "https://api.thingspeak.com/update?"; // URL del servidor de ThingSpeak
String api_key = "XTUNK0M3XFB5YD9C"; // Clave API personal del tablero en ThingSpeak
String sensor = "field1"; // Campo donde se enviará el dato del botón

void setup() {
  // Configuración de los pines
  pinMode(led, OUTPUT);  // Configura el pin del LED como salida
  pinMode(boton, INPUT); // Configura el pin del botón como entrada

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
  // Leer el estado del botón (0 si no está presionado, 1 si está presionado)
  status_boton = digitalRead(boton);

  // Encender o apagar el LED según el estado del botón
  if (status_boton == 1) {
    digitalWrite(led, HIGH);  // Enciende el LED si el botón está presionado
  } else {
    digitalWrite(led, LOW);   // Apaga el LED si el botón no está presionado
  }

  // Verifica si el ESP32 está conectado a WiFi antes de enviar datos
  if ((WiFi.status() == WL_CONNECTED)) {
    // Crear un objeto HTTPClient para la solicitud HTTP
    HTTPClient http;

    // Construir la URL con los datos del botón
    String message = url + "api_key=" + api_key + "&" + sensor + "=" + String(status_boton);
    http.begin(message); // Iniciar la conexión HTTP

    // Realizar la petición GET a ThingSpeak y obtener el código de respuesta
    int httpCode = http.GET();
    Serial.println(message);  // Muestra la URL de la petición en la consola
    Serial.print("Código GET: ");
    Serial.println(httpCode);  // Muestra el código de respuesta de la solicitud HTTP

    http.end();  // Finaliza la conexión HTTP
  }

  delay(3000);  // Espera 3 segundos antes de enviar nuevamente
}

