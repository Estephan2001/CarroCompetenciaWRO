/*Libreria creada por: Estephan Portales
* Basandose en librerias existentes para MPU6050
* 
*/
#include "FEPO.h"
#include <Wire.h>  // LIBRERIA PARA PODER USAR EL I2C
//#include "BluetoothSerial.h"
#include "Stdlib.h"
#include <EEPROM.h>



float gyroXoffset, gyroYoffset, gyroZoffset;  ///variables para los ca
float temp, accX, accY, accZ, gyroX, gyroY, gyroZ;
float angleGyroX, angleGyroY, angleGyroZ,
  angleAccX, angleAccY, angleAccZ;

float angleX, angleY, angleZ;
float roll, pitch, yaw;

unsigned long preintervalo1 = millis();
float intervalo1 = 0;


void inicioMPU() {
  Wire.begin();          // iniciamos comunicacion I2C
  MPUwrite(0x19, 0x00);  //MPU6050_SMPLRT_DIV
  MPUwrite(0x1a, 0x00);  //MPU6050_CONFIG
  MPUwrite(0x1b, 0x08);  //MPU6050_GYRO_CONFIG
  MPUwrite(0x1c, 0x00);  //MPU6050_ACCEL_CONFIG
  MPUwrite(0x6b, 0x01);  //MPU6050_PWR_MGMT_1
  //mpuupdate();
  //angleGyroX = 0;
  //angleGyroY = 0;
  preintervalo1 = millis();
  calcoffset(true);
}

void MPUwrite(byte reg, byte datas) {
  Wire.beginTransmission(0x68);  //MPU6050_ADDR
  Wire.write(reg);
  Wire.write(datas);
  Wire.endTransmission();
}

byte MPUread(byte reg) {
  Wire.beginTransmission(0x68);  //MPU6050_ADDR
  Wire.write(reg);
  Wire.endTransmission(true);
  Wire.requestFrom(0x68, 1);
  byte datas = Wire.read();
  return datas;
}

void calcoffset(bool console) {
  float x = 0, y = 0, z = 0;
  int16_t rx, ry, rz;

  delay(1000);

  if (console) {
    /* Serial.println();
    Serial.println("========================================");
    Serial.println("CALCULANDO OFFSETS");
    Serial.print("NO MOVER MPU6050");*/
  }

  for (int i = 0; i < 3000; i++) {
    if (console && i % 1000 == 0) {
      Serial.print(".");
    }
  }

  Wire.beginTransmission(0x68);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom((int)0x68, 6);
  rx = Wire.read() << 8 | Wire.read();
  ry = Wire.read() << 8 | Wire.read();
  rz = Wire.read() << 8 | Wire.read();

  x += ((float)rx) / 65.5;
  y += ((float)ry) / 65.5;
  z += ((float)rz) / 65.5;

  gyroXoffset = x / 3000;
  gyroYoffset = y / 3000;
  gyroZoffset = z / 3000;

  if (console) {
    /* Serial.println();
    Serial.println("CALCULADOS!");
    Serial.print("X : "); Serial.println(gyroXoffset);
    Serial.print("Y : "); Serial.println(gyroYoffset);
    Serial.print("Z : "); Serial.println(gyroZoffset);
    Serial.println("EL PROGRAMA DE INICIA EN 3 SEGUNDOS");
    Serial.print("========================================");*/
    delay(500);
  }
}

void mpuupdate() {
  int16_t rawAccX, rawAccY, rawAccZ, rawTemp,
    rawGyroX, rawGyroY, rawGyroZ;

  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom((int)0x68, 14);

  rawAccX = Wire.read() << 8 | Wire.read();
  rawAccY = Wire.read() << 8 | Wire.read();
  rawAccZ = Wire.read() << 8 | Wire.read();
  rawTemp = Wire.read() << 8 | Wire.read();
  rawGyroX = Wire.read() << 8 | Wire.read();
  rawGyroY = Wire.read() << 8 | Wire.read();
  rawGyroZ = Wire.read() << 8 | Wire.read();

  // Conversión
  temp = (rawTemp + 12412.0) / 340.0;
  accX = ((float)rawAccX) / 16384.0;
  accY = ((float)rawAccY) / 16384.0;
  accZ = ((float)rawAccZ) / 16384.0;

  angleAccX = atan2(accY, sqrt(accZ * accZ + accX * accX)) * 180.0 / PI;
  angleAccY = atan2(accX, sqrt(accZ * accZ + accY * accY)) * -180.0 / PI;

  gyroX = ((float)rawGyroX) / 65.5;
  gyroY = ((float)rawGyroY) / 65.5;
  gyroZ = ((float)rawGyroZ) / 65.5;

  // Corrección offsets
  gyroX -= gyroXoffset;
  gyroY -= gyroYoffset;
  gyroZ -= gyroZoffset;

  // Intervalo más preciso con micros()
  static unsigned long lastMicros = micros();
  float dt = (micros() - lastMicros) / 1000000.0;
  lastMicros = micros();

  // Integración giros
  angleGyroX += gyroX * dt;
  angleGyroY += gyroY * dt;
  angleGyroZ += gyroZ * dt;

  // Normalizar yaw a 0–360
  //if (angleGyroZ >= 360) angleGyroZ -= 360;
  //if (angleGyroZ < 0) angleGyroZ += 360;

  // Filtro complementario con más corrección del acelerómetro
  float accCoef = 0.03f;   // Corrección más agresiva del acelerómetro
  float gyroCoef = 0.95f;  // Menos dependencia del giroscopio

  angleX = gyroCoef * (angleX + gyroX * dt) + accCoef * angleAccX;
  angleY = gyroCoef * (angleY + gyroY * dt) + accCoef * angleAccY;
  angleZ = angleGyroZ;  // sigue viniendo solo del giroscopio (sin magnetómetro)


  roll = angleX;
  pitch = angleY;
  yaw = angleZ;
}
