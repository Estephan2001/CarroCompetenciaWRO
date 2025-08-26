#include "UltraSonicoSM.h"

void init_UltraSonico(int Trigger, int Echo){
pinMode(Trigger, OUTPUT);
pinMode(Echo,INPUT);

}

float Distancia_UltraSonico(int Trigger, int Echo){
    long duracion{};
    float distancia{};

    // Aseguramos que el trigger esté en LOW
    digitalWrite(Trigger, LOW);
    delayMicroseconds(2);

    // Enviamos pulso de 10us
    digitalWrite(Trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(Trigger, LOW);

    // Medimos la duración del pulso en echo
    duracion = pulseIn(Echo, HIGH);

    // Calculamos la distancia (cm)
    distancia = duracion * 0.0343 / 2;

    return distancia;
}