#ifndef UltraSonicoSM
#define UltraSonicoSM

#include <Arduino.h>

// Prototipo de funciones
void init_UltraSonico(int Trigger, int Echo);
float Distancia_UltraSonico(int Trigger, int Echo);

#endif