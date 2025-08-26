// Librerías

#include "FEPO.h"
#include <Servo.h>

// Constantes (Definimos en dónde conectamos todo)
#define ServosRuedas

// Definir Objetos
Servo ServoDireccion;            // Creamos un objeto servo
Servo ServoUltrasonicoAdelante;  // Creamos un objeto servo
Servo ServoUltrasonicoAtras;     // Creamos un objeto servo

void setup() {
  inicioMPU();
  Serial.begin(9600);

  ServoDireccion.attach(9);  // Pin donde está conectado el servo (PWM)
  ServoUltrasonicoAdelante.attach(10);  // Pin donde está conectado el servo (PWM)
  ServoUltrasonicoAtras.attach(11);  // Pin donde está conectado el servo (PWM)
  //Posiciones iniciales
  ServoDireccion.write(90);
  ServoUltrasonicoAdelante.write(90);
  ServoUltrasonicoAtras.write(90);
}

void loop() {
  mpuupdate();
  Serial.println((int)yaw);
  delay(10);
}
