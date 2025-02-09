// Librerías
#include <DHT.h>

// Puertos GPIO
#define led 32
#define sensor 33

// DHT22
DHT dht(sensor, DHT22); // DHTPIN, DHTTYPE

// Variables globales
int valor_temperatura = 0;
int valor_humedad = 0;

void setup() {
    pinMode(led, OUTPUT); // Configurar puerto 2 como salida
    dht.begin(); // Inicializar DHT
    Serial.begin(115200);
}

void loop() {
    delay(1000);
    
    valor_temperatura = dht.readTemperature();
    valor_humedad = dht.readHumidity();

    Serial.print("Temperatura: ");
    Serial.println(valor_temperatura);
    Serial.print("Humedad: ");
    Serial.println(valor_humedad);

    // Si la humedad es mayor al 50% encender el LED
    if(valor_humedad >= 50) { 
        digitalWrite(led, HIGH);
    } else {
        digitalWrite(led, LOW);
    }
}

