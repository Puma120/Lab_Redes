#define pinGPIO1 1
#define pinGPIO21 21
#define pinGPIO47 47

int state1 = 0;
int state2 = 0;

void setup() {
  pinMode(pinGPIO1, OUTPUT);
  pinMode(pinGPIO21, INPUT);
  pinMode(pinGPIO47, INPUT);
  digitalWrite(pinGPIO1, LOW);
}

void loop() {
  state1 = digitalRead(pinGPIO21);
  state2 = digitalRead(pinGPIO47);
  if (state1 && state2){
    digitalWrite(pinGPIO1, HIGH);
  } else{
    digitalWrite(pinGPIO1, LOW);
  }
}



