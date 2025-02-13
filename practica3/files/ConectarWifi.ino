#include <WiFi.h>  // Incluye la librería WiFi para gestionar la conexión
#include <HTTPClient.h>  // Incluye la librería para realizar solicitudes HTTP

// Define la URL a la que se realizará la solicitud HTTP
String message = "http://example.com/index.html";

void setup() {

    Serial.begin(115200);  // Inicia la comunicación serie a 115200 baudios

    // Conectar a la red WiFi
    WiFi.begin("MSI 9472", "|05791xZ");  // Nombre de la red WiFi y contraseña

    // Esperar hasta que se establezca la conexión WiFi
    while ((WiFi.status() != WL_CONNECTED)) {
        delay(500);
        Serial.print(".");  // Muestra puntos en la consola mientras se conecta
    }
    Serial.println("WiFi conectado");  // Mensaje de confirmación de conexión
}

void loop() {
    // Verifica si el ESP32 está conectado a WiFi
    if ((WiFi.status() == WL_CONNECTED)) {

        HTTPClient http;  // Crea un objeto HTTPClient
        http.begin(message);  // Inicia la conexión con la URL definida

        // Inicia la conexión y envía la solicitud HTTP GET, retorna un código de error si falla
        int httpCode = http.GET();
        Serial.print("[HTTP] GET...\n");  // Mensaje indicando que se está realizando la solicitud
        Serial.printf("[HTTP] GET... código: %d\n", httpCode);  // Muestra el código de respuesta del servidor
        
        http.end();  // Finaliza la conexión HTTP
    }
    delay(2000);  // Espera 2 segundos antes de realizar otra solicitud
}
