/*Libreria creada por: Estephan Portales
* Basandose en librerias existentes para MPU6050
* 
*/
#ifndef FEPO
#define FEPO
#include <Arduino.h>

void inicioMPU();
void MPUwrite(byte reg, byte datas);
byte MPUread(byte reg);
void calcoffset(bool console);
void mpuupdate();


extern float roll, pitch, yaw, gyroX, gyroY, angleZ;



#endif
