/* Ejemplo de escaneo de redes WiFi con ESP32 */
#include "WiFi.h"  // Incluye la librería WiFi para el ESP32

void setup() {
  Serial.begin(115200);  // Inicia la comunicación serie a 115200 baudios
  Serial.println("Inicializando WiFi...");
  
  WiFi.mode(WIFI_STA);  // Configura el ESP32 en modo estación (cliente)
  Serial.println("Configuración terminada!");
}

void loop() {
  Serial.println("Escaneando...");

  // WiFi.scanNetworks devuelve el número de redes encontradas
  int n = WiFi.scanNetworks();
  Serial.println("Escaneo terminado!");
  
  if (n == 0) {
    Serial.println("No se encontraron redes.");
  } else {
    Serial.println();
    Serial.print(n);
    Serial.println(" redes encontradas");
    
    // Recorre todas las redes encontradas
    for (int i = 0; i < n; ++i) {
      Serial.print(i + 1);  // Muestra el número de red
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));  // Muestra el nombre de la red (SSID)
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));  // Muestra la intensidad de la señal (RSSI)
      Serial.print(")");

      // Verifica si la red tiene seguridad (encriptación)
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
      
      delay(10);  // Pequeña pausa para evitar saturar la salida serie
    }
  }
  Serial.println("");

  // Espera 5 segundos antes de volver a escanear
  delay(5000);
}
