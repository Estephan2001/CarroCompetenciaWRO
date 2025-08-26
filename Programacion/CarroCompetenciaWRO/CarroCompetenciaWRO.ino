// Librerías

#include "FEPO.h"
#include <Servo.h>
#include "UltraSonicoSM.h"

// Constantes (Definimos en dónde conectamos todo)
#define ServoDireccionPin 9
#define ServoUltrasonicoAdelantePin 10
#define ServoUltrasonicoAtrasPin 11
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
// Pines Motor propulsor
#define InputMotor1 12
#define InputMotor2 13


// Definir Objetos
Servo ServoDireccion;            // Creamos un objeto servo
Servo ServoUltrasonicoAdelante;  // Creamos un objeto servo
Servo ServoUltrasonicoAtras;     // Creamos un objeto servo

// Variables Robot

// Posiciones actuales
int PosServoDireccion{ 90 };
int PosServoUltrasonicoAdelante{ 90 };
int PosServoUltrasonicoAtrasPin{ 90 };
// Posiciones deseadas
int PosServoDireccionDeseada{ 90 };
int PosServoUltrasonicoAdelanteDeseada{ 90 };
int PosServoUltrasonicoAtrasPinDeseada{ 90 };


void setup() {
  inicioMPU();
  Serial.begin(9600);

  ServoDireccion.attach(ServoDireccionPin);                      // Pin donde está conectado el servo (PWM)
  ServoUltrasonicoAdelante.attach(ServoUltrasonicoAdelantePin);  // Pin donde está conectado el servo (PWM)
  ServoUltrasonicoAtras.attach(ServoUltrasonicoAtrasPin);        // Pin donde está conectado el servo (PWM)
  //Posiciones iniciales
  ServoDireccion.write(PosServoDireccion);
  ServoUltrasonicoAdelante.write(PosServoUltrasonicoAdelante);
  ServoUltrasonicoAtras.write(PosServoUltrasonicoAtrasPin);

  //Inicializar Ultrasónicos
  init_UltraSonico(TriggerUltrasonicoAtras, EchoUltrasonicoAtras);
  init_UltraSonico(TriggerUltrasonicoAdelante, EchoUltrasonicoAdelante);
  init_UltraSonico(TriggerUltrasonicoObstaculo, EchoUltrasonicoObstaculo);

  //Inicializar Seguidores de línea
  pinMode(SeguidorIzquierda, INPUT);
  pinMode(SeguidorDerecha, INPUT);

  // Sensor Color
  pinMode(PinS0, OUTPUT);
  pinMode(PinS1, OUTPUT);
  pinMode(PinS2, OUTPUT);
  pinMode(PinS3, OUTPUT);
  pinMode(PinSalidaColor, INPUT);
  digitalWrite(PinS0, HIGH);
  digitalWrite(PinS1, HIGH);

  // Inicializar Motor
  pinMode(InputMotor1, OUTPUT);
  pinMode(InputMotor2, OUTPUT);
  digitalWrite(InputMotor1, LOW);
  digitalWrite(InputMotor2, LOW);
}

void loop() {
  // mpuupdate();

  // Distancia_UltraSonico(triggerPin, echoPin);


  /* COLOR
    digitalWrite(s2, LOW);  
  digitalWrite(s3, LOW);   
  rojo = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);  
  digitalWrite(s3, HIGH);   
  azul = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);  
  digitalWrite(s2, HIGH);    
  verde = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);  
  */
}
