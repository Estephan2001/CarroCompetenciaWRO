// Librerías

#include "FEPO.h"
#include <Servo.h>
#include "UltraSonicoSM.h"

// Constantes (Definimos en dónde conectamos todo)
#define ServoDireccionPin 9
#define ServoUltrasonicoAdelantePin 10
#define ServoUltrasonicoAtrasPin  11

// Definir Objetos
Servo ServoDireccion;            // Creamos un objeto servo
Servo ServoUltrasonicoAdelante;  // Creamos un objeto servo
Servo ServoUltrasonicoAtras;     // Creamos un objeto servo

void setup() {
  inicioMPU();
  Serial.begin(9600);

  ServoDireccion.attach(ServoDireccionPin);  // Pin donde está conectado el servo (PWM)
  ServoUltrasonicoAdelante.attach(ServoUltrasonicoAdelantePin);  // Pin donde está conectado el servo (PWM)
  ServoUltrasonicoAtras.attach(ServoUltrasonicoAtrasPin);  // Pin donde está conectado el servo (PWM)
  //Posiciones iniciales
  ServoDireccion.write(90);
  ServoUltrasonicoAdelante.write(90);
  ServoUltrasonicoAtras.write(90);
}

void loop() {
  mpuupdate();
 
}
