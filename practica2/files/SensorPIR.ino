#define led 32               // GPIO32 para el LED
#define PIR 33             // GPIO33 para el sensor PIR

//Variables globales
int pirState = LOW;            // Variable para considerar que el movimiento esta en curso
int val = 0;                 // Variable para leer el estado del sensor

void setup() {
    pinMode(led, OUTPUT);    // Declarar LED como salida
    pinMode(PIR, INPUT);   // Declarar sensor PIR como entrada
    Serial.begin(115200);
}

void loop() {
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
}


