#define pinGPIO1 1

void setup() {
  pinMode(pinGPIO1, OUTPUT);
  digitalWrite(pinGPIO1, HIGH);
}

void loop() {
  delay(3000);
  digitalWrite(pinGPIO1, LOW);
}
