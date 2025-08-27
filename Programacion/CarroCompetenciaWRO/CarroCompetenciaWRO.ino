// Librerías

#include "FEPO.h"
#include <Servo.h>
#include "UltraSonicoSM.h"
#include "ValorReferenciaSM.h"

// Constantes (Definimos en dónde conectamos todo)
#define ServoDireccionPin 9
#define ServoUltrasonicoAdelantePin 10
#define ServoUltrasonicoAtrasPin 11
// Pines de ultrasonicos
#define TriggerUltrasonicoAtras 2
#define EchoUltrasonicoAtras 3
#define TriggerUltrasonicoAdelante 6
#define EchoUltrasonicoAdelante 7
#define TriggerUltrasonicoObstaculo 5
#define EchoUltrasonicoObstaculo 4
#define TiempoEntrePosiciones 5  //ms
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

// Posiciones actuales Servo
int PosServoDireccion{ 90 };
int PosServoUltrasonicoAdelante{ 90 };
int PosServoUltrasonicoAtras{ 90 };
// Posiciones deseadas
int PosServoDireccionDeseada{ 90 };
int PosServoUltrasonicoAdelanteDeseada{ 90 };
int PosServoUltrasonicoAtrasDeseada{ 90 };
// Variables Sensores
char LadoGiro{};  // I C D (Izquierda Centro Derecha)

char Color{};  // R V A (Rojo Verde Azul)
int Rojo{};
int Verde{};
int Azul{};
int UMBRAL_DIF = 15;  // Diferencia mínima entre componentes para decidir
int MIN_VAL = 20;     // Valor mínimo válido para considerar un color
int MAX_VAL = 255;    // Valor máximo válido (dependerá de tu sensor)

char DireccionCarro{};  // A B P(Adeltante o Atras o Parar)
int AnguloActual{};     // Valor de MPU6050 o giroscopio
int DistanciaUltrasonicoDelante{};
int DistanciaUltrasonicoAtras{};
int DistanciaUltrasonicoObstaculo{};
int ValorLineaIzquierda{};
int ValorLineaDerecha{};

// Variables de pasos
int Acto{ 0 };


// Timers
unsigned long TiempoServosPasado{};
int IntervaloEntrePosiciones{ TiempoEntrePosiciones };

// Timer Mediciones
unsigned long TiempoPasadoMediciones{};
int IntervaloMediciones{ 20 };


// Prototipos de funciones
char detectarColor(int r, int g, int b);

void setup() {

  inicioMPU();
  Serial.begin(9600);

  ServoDireccion.attach(ServoDireccionPin);                      // Pin donde está conectado el servo (PWM)
  ServoUltrasonicoAdelante.attach(ServoUltrasonicoAdelantePin);  // Pin donde está conectado el servo (PWM)
  ServoUltrasonicoAtras.attach(ServoUltrasonicoAtrasPin);        // Pin donde está conectado el servo (PWM)
  //Posiciones iniciales
  ServoDireccion.write(PosServoDireccion);
  ServoUltrasonicoAdelante.write(PosServoUltrasonicoAdelante);
  ServoUltrasonicoAtras.write(PosServoUltrasonicoAtras);

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


  // Sentencia para escoger lado (Botones)
  LadoGiro = 'I';
  DireccionCarro = 'P';
  PosServoDireccionDeseada = 45;
}

void loop() {

  // Poner posicion deseada de los servos según el giro
  if (LadoGiro == 'I') {
    PosServoUltrasonicoAdelanteDeseada = 180;
    PosServoUltrasonicoAtrasDeseada = 0;
  } else if (LadoGiro == 'D') {
    PosServoUltrasonicoAdelanteDeseada = 0;
    PosServoUltrasonicoAtrasDeseada = 180;
  } /*else if (LadoGiro == 'C') {
    PosServoUltrasonicoAdelanteDeseada = 90;
    PosServoUltrasonicoAtrasDeseada = 90;
  }*/
  ServoUltrasonicoAtras.write(45);
  // Posicionar servos según sea la necesidad
  if (millis() >= TiempoServosPasado + IntervaloEntrePosiciones) {
    PosicionServo(PosServoUltrasonicoAtras, PosServoUltrasonicoAtrasDeseada, 0, 180);
    //PosicionServo(PosServoDireccion, PosServoDireccionDeseada, 0, 180);
    //PosicionServo(PosServoUltrasonicoAdelante, PosServoUltrasonicoAdelanteDeseada, 0, 180);


    PosServoUltrasonicoAtrasDeseada > 180 ? PosServoUltrasonicoAtrasDeseada = 180 : PosServoUltrasonicoAtrasDeseada = PosServoUltrasonicoAtrasDeseada;
    PosServoUltrasonicoAtrasDeseada < 0 ? PosServoUltrasonicoAtrasDeseada = 0 : PosServoUltrasonicoAtrasDeseada = PosServoUltrasonicoAtrasDeseada;
    PosServoUltrasonicoAtrasDeseada > PosServoUltrasonicoAtras ? PosServoUltrasonicoAtras++ : PosServoUltrasonicoAtras = PosServoUltrasonicoAtras;
    PosServoUltrasonicoAtrasDeseada < PosServoUltrasonicoAtras ? PosServoUltrasonicoAtras-- : PosServoUltrasonicoAtras = PosServoUltrasonicoAtras;


    //ServoDireccion.write(PosServoDireccion);
    //ServoUltrasonicoAdelante.write(PosServoUltrasonicoAdelante);
    ServoUltrasonicoAtras.write(PosServoUltrasonicoAtras);
    TiempoServosPasado = millis();
  }

  // El Robot Se mueva a adelante, atras o pare.
  if (DireccionCarro == 'A') {
    digitalWrite(InputMotor1, HIGH);
    digitalWrite(InputMotor2, LOW);
  } else if (DireccionCarro == 'B') {
    digitalWrite(InputMotor1, LOW);
    digitalWrite(InputMotor2, HIGH);
  } else if (DireccionCarro == 'P') {
    digitalWrite(InputMotor1, LOW);
    digitalWrite(InputMotor2, LOW);
  }

  // Mediciones
  // Angulo de carro

  if (millis() >= TiempoPasadoMediciones + IntervaloMediciones) {
    //UltraSonico

    DistanciaUltrasonicoDelante = Distancia_UltraSonico(TriggerUltrasonicoAdelante, EchoUltrasonicoAdelante);
    DistanciaUltrasonicoAtras = Distancia_UltraSonico(TriggerUltrasonicoAtras, EchoUltrasonicoAtras);
    DistanciaUltrasonicoObstaculo = Distancia_UltraSonico(TriggerUltrasonicoObstaculo, EchoUltrasonicoObstaculo);

    // Medicion de Color
    digitalWrite(PinS2, LOW);
    digitalWrite(PinS3, LOW);
    Rojo = pulseIn(PinSalidaColor, digitalRead(PinSalidaColor) == HIGH ? LOW : HIGH);
    digitalWrite(PinS3, HIGH);
    Azul = pulseIn(PinSalidaColor, digitalRead(PinSalidaColor) == HIGH ? LOW : HIGH);
    digitalWrite(PinS2, HIGH);
    Verde = pulseIn(PinSalidaColor, digitalRead(PinSalidaColor) == HIGH ? LOW : HIGH);
    // Determinar Color
    Color = detectarColor(Rojo, Verde, Azul);

    // Medir Angulos
    mpuupdate();
    AnguloActual = yaw;

    TiempoPasadoMediciones = millis();
  }
}

// Funciones EXTRA

char detectarColor(int r, int g, int b) {
  // Si los valores son muy bajos, consideramos que no hay color válido
  if (r < MIN_VAL && g < MIN_VAL && b < MIN_VAL) {
    return '?';  // Sin detección
  }

  // Diferencias entre componentes
  int difRG = r - g;
  int difRB = r - b;
  int difGB = g - b;

  // --- Clasificación básica ---
  if ((r > b + UMBRAL_DIF) && (r > g)) {
    return 'A';  // Rojo
  } else if ((b > r + UMBRAL_DIF) && (b > g)) {
    return 'V';  // Azul
  } else if ((g > r + UMBRAL_DIF) && (g > b)) {
    return 'R';  // Verde (extra ejemplo)
  } else {
    return '?';  // Indefinido o mezcla
  }
}
