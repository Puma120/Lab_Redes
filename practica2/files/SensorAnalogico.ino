#define pot 33
#define led 32

// Variable para dato analogico
int valor_analogico = 0;

void setup() {
    pinMode(led, OUTPUT); // Configurar puerto 2 como salida
    Serial.begin(115200);
}

void loop() {
    // Leer analogicamente el pin 33
    valor_analogico = analogRead(pot);
    Serial.println(valor_analogico);

    //Si el valor analogico esta por encima del 50% del valor maximo del pot enceder
    if(valor_analogico >= 2048) {
        digitalWrite(led, HIGH);
    } else {
        digitalWrite(led, LOW);
    }
}


