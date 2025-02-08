#define pinGPIO1 1

void setup() {
  pinMode(pinGPIO1, OUTPUT);
  digitalWrite(pinGPIO1, LOW);
}

void loop() {
    delay(500);
    digitalWrite(pinGPIO1, HIGH);
    delay(500);
    digitalWrite(pinGPIO1, LOW);
}


