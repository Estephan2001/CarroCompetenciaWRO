#include "ValorReferenciaSM.h"

void PosicionServo(int &PosServo, int Referencia, int LimInferior, int LimSuperior){

    Referencia > LimSuperior ? Referencia = LimSuperior : Referencia = Referencia;
    Referencia < LimInferior ? Referencia = LimInferior : Referencia = Referencia;

    Referencia > PosServo ? PosServo++ : PosServo = PosServo;
    Referencia < PosServo ? PosServo-- : PosServo = PosServo;


}