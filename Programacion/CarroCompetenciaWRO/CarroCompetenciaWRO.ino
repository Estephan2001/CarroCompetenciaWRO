// Librerías

#include "FEPO.h"
#include <Servo.h>
#include "UltraSonicoSM.h"

// Constantes (Definimos en dónde conectamos todo)
#define ServoDireccionPin 9
#define ServoUltrasonicoAdelantePin 10
#define ServoUltrasonicoAtrasPin  11
// Pines de ultrasonicos
#define TriggerUltrasonicoAtras 2
#define EchoUltrasonicoAtras 3
#define TriggerUltrasonicoAdelante 2
#define EchoUltrasonicoAdelante 3
#define TriggerUltrasonicoObstaculo 6
#define EchoUltrasonicoObstaculo 7
//Pines de Seguidores de línea analogos
#define SeguidorIzquierda A6
#define SeguidorDerecha A7
//Pines sensor color
#define PinSalidaColor 8
#define PinS0 A0
#define PinS1 A1
#define PinS2 A2
#define PinS3 A3



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

  //Inicializar Ultrasónicos
  init_UltraSonico(TriggerUltrasonicoAtras, EchoUltrasonicoAtras);
  init_UltraSonico(TriggerUltrasonicoAdelante, EchoUltrasonicoAdelante);
  init_UltraSonico(TriggerUltrasonicoObstaculo, EchoUltrasonicoObstaculo);

  //Inicializar Seguidores de línea
  pinMode(SeguidorIzquierda,INPUT);
  pinMode(SeguidorDerecha,INPUT);
}

void loop() {
  mpuupdate();
 // Distancia_UltraSonico(triggerPin, echoPin);
 
}
