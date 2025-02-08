#define gpio20 20
#define gpio21 21
#define gpio39 39

int boton1 = 0;
void setup() {
  pinMode(gpio20,INPUT);
  pinMode(gpio39,OUTPUT);
}

void loop() {
  boton1 = digitalRead(gpio20);
  if(boton1){
    digitalWrite(gpio39,HIGH);
    delay(200);
    digitalWrite(gpio39,LOW);
    delay(200);
  }
  else{
    digitalWrite(gpio39,LOW);
  }
}

