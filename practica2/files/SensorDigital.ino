#define boton 33
#define led 32

// Variable para guardar el estado del boton
int status_boton=0;

void setup() {
    pinMode(led, OUTPUT);
    pinMode(boton, INPUT);
}

void loop() {
    //Checar el estado del boton
    status_boton = digitalRead(boton);

    //Si presionado encender el led, en caso contrario apagarlo
    if(status_boton == 1) {
        digitalWrite(led, HIGH);
    } else {
        digitalWrite(led, LOW);
    }
}




