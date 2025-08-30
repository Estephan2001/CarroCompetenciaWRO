// Librerías

#include "FEPO.h"
#include <Servo.h>
#include "UltraSonicoSM.h"
#include "ValorReferenciaSM.h"

// Constantes (Definimos en dónde conectamos todo)
#define ServoDireccionPin 2
#define ServoUltrasonicoAdelantePin 10
#define ServoUltrasonicoAtrasPin 9
// Pines de ultrasonicos
#define TriggerUltrasonicoAtras 11
#define EchoUltrasonicoAtras 3
#define TriggerUltrasonicoAdelante 6
#define EchoUltrasonicoAdelante 7
#define TriggerUltrasonicoObstaculo 5
#define EchoUltrasonicoObstaculo 4
#define TiempoEntrePosiciones 20  //ms
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
const int MinServoDireccion{ 70 };
const int MaxServoDireccion{ 125 };
int PosServoUltrasonicoAdelante{ 90 };
int PosServoUltrasonicoAtras{ 90 };

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
// Variables donde se guarda la informacion obtenida de los sensores
int DistanciaUltrasonicoDelante{};
int DistanciaUltrasonicoAtras{};
int DistanciaUltrasonicoObstaculo{};
int ValorLineaIzquierda{};
int ValorLineaDerecha{};
float ContadorVueltas{};

// Variables de pasos
int Acto{ 0 };


// Timers
unsigned long TiempoServosPasado{};
int IntervaloEntrePosiciones{ TiempoEntrePosiciones };

// Timer Mediciones
unsigned long TiempoPasadoMediciones{};
int IntervaloMediciones{ 1 };

// Condiciones de inicio Robot
int DistanciaInicialParedUltrasonicoAdelante{};
int DistanciaInicialParedUltrasonicoAtras{};
int ErrorDistanciaAdelante{};
int ErrorDistanciaAtras{};

// Variables para contar vueltas
float AnguloPrevio = 0;     // Ángulo en la última medición
long AcumuladorAngulo = 0;  // Total de grados recorridos
//int ContadorVueltas = 0;    // Número de vueltas completadas


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
  analogWrite(InputMotor1, 0);
  analogWrite(InputMotor2, 0);

  // Aquí decidimos con los botones a dónde irá el robot (Izquierda o derecha)


  // Sentencia para escoger lado (Botones)
  LadoGiro = 'I';
  DireccionCarro = 'P';
  PosServoDireccion = 97;
  Acto = 0;  //Calibraciones iniciales
}

void loop() {

  // Poner posicion deseada de los servos según el giro
  if (LadoGiro == 'I') {

    PosServoUltrasonicoAdelante = 180;
    PosServoUltrasonicoAtras = 0;
    ServoUltrasonicoAdelante.write(PosServoUltrasonicoAdelante);
    ServoUltrasonicoAtras.write(PosServoUltrasonicoAtras);
    delay(1500);
    LadoGiro = 0;
  } else if (LadoGiro == 'D') {

    PosServoUltrasonicoAdelante = 0;
    PosServoUltrasonicoAtras = 180;
    ServoUltrasonicoAdelante.write(PosServoUltrasonicoAdelante);
    ServoUltrasonicoAtras.write(PosServoUltrasonicoAtras);
    delay(1500);
    LadoGiro = 0;
  } else if (LadoGiro == 'C') {
    PosServoUltrasonicoAdelante = 90;
    PosServoUltrasonicoAtras = 90;
    ServoUltrasonicoAdelante.write(PosServoUltrasonicoAdelante);
    ServoUltrasonicoAtras.write(PosServoUltrasonicoAtras);
    delay(1500);
    LadoGiro = 0;
  }

  // Posicionar servos según sea la necesidad
  if (millis() >= TiempoServosPasado + IntervaloEntrePosiciones) {
    ServoDireccion.write(PosServoDireccion);
    TiempoServosPasado = millis();
  }

  // El Robot Se mueva a adelante, atras o pare.
  if (DireccionCarro == 'A') {
    analogWrite(InputMotor1, 150);
    analogWrite(InputMotor2, 0);
  } else if (DireccionCarro == 'B') {
    analogWrite(InputMotor1, 0);
    analogWrite(InputMotor2, 150);
  } else if (DireccionCarro == 'P') {
    analogWrite(InputMotor1, 0);
    analogWrite(InputMotor2, 0);
  }

  // Mediciones
  // Angulo de carro
  // Medir Angulos
  mpuupdate();
  AnguloActual = yaw;

  if (millis() >= TiempoPasadoMediciones + IntervaloMediciones) {
    //UltraSonico

    DistanciaUltrasonicoDelante = Distancia_UltraSonico(TriggerUltrasonicoAdelante, EchoUltrasonicoAdelante);
    //DistanciaUltrasonicoAtras = Distancia_UltraSonico(TriggerUltrasonicoAtras, EchoUltrasonicoAtras);
    //DistanciaUltrasonicoObstaculo = Distancia_UltraSonico(TriggerUltrasonicoObstaculo, EchoUltrasonicoObstaculo);

    // Medicion de Color
    /*
    digitalWrite(PinS2, LOW);
    digitalWrite(PinS3, LOW);
    Rojo = pulseIn(PinSalidaColor, digitalRead(PinSalidaColor) == HIGH ? LOW : HIGH);
    digitalWrite(PinS3, HIGH);
    Azul = pulseIn(PinSalidaColor, digitalRead(PinSalidaColor) == HIGH ? LOW : HIGH);
    digitalWrite(PinS2, HIGH);
    Verde = pulseIn(PinSalidaColor, digitalRead(PinSalidaColor) == HIGH ? LOW : HIGH);
    // Determinar Color
    Color = detectarColor(Rojo, Verde, Azul);
*/

    TiempoPasadoMediciones = millis();
  }


  //////////////////////////////////////////////////////////////////////////////////////////// Procedimiento para RETO 1 ///////////////////////////////////////////////////////////////////////////



  /*

  int DistanciaUltrasonicoDelante{};
int DistanciaUltrasonicoAtras{};
int DistanciaUltrasonicoObstaculo{};
int ValorLineaIzquierda{};
int ValorLineaDerecha{};

int DistanciaParedUltrasonicoAdelante{};
int DistanciaParedUltrasonicoAtras{};

ContadorVueltas

int ErrorDistanciaAdelante{};
int ErrorDistanciaAtras{};
*/
  if (Acto == 0) {  // Registras las condiciones iniciales
    //Serial.println("Acto 1");
    DireccionCarro = 'A';
    DistanciaUltrasonicoDelante = Distancia_UltraSonico(TriggerUltrasonicoAdelante, EchoUltrasonicoAdelante) + 10;  ////////////MODIFICAR SI SE QUIERE SEPARAR DE LA PARED
    DistanciaInicialParedUltrasonicoAdelante = DistanciaUltrasonicoDelante;
    DistanciaInicialParedUltrasonicoAtras = DistanciaUltrasonicoAtras;
    ServoDireccion.write(97);
    Acto = 1;
  }
  ////////////////**************************************************************************************************************************************************************************************
  else if (Acto == 1) {
    // Contar las vueltas

    ContadorVueltas = abs(angleZ / 360);
    // Serial.println(angleZ);
    if (ContadorVueltas >= 7.8) {  ////MODIFICAR SI LO COMPLETA LAS VUELTAS
      DireccionCarro = 'P';
      Acto = 2;
    }
    //////////////////////////////////**********************************************************************************************************************************************************************
    // Que se mantenga a cierta distancia


    float errorDistancia{};
    errorDistancia = (DistanciaUltrasonicoDelante - DistanciaInicialParedUltrasonicoAdelante) * 20;  //// 20 - 30 SI REACCIONA MUY LENTO
    int PosServoError = map(errorDistancia, 20, -20, MaxServoDireccion, MinServoDireccion);
    PosServoError > MaxServoDireccion ? PosServoError = MaxServoDireccion : PosServoError = PosServoError;
    PosServoError < MinServoDireccion ? PosServoError = MinServoDireccion : PosServoError = PosServoError;
    ///////////**********************************************************************************************************************************************************
    PosServoDireccion = PosServoError;
    ServoDireccion.write(PosServoDireccion);
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
