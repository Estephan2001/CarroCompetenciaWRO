/* Librería creada por: Estephan Portales
 * Basada en librerías existentes para MPU6050
 */

#include "FEPO.h"
#include <Wire.h>  // Comunicación I2C
#include <EEPROM.h>

float gyroXoffset, gyroYoffset, gyroZoffset;  // offsets del giroscopio
float temp, accX, accY, accZ, gyroX, gyroY, gyroZ;
float angleGyroX, angleGyroY, angleGyroZ;
float angleAccX, angleAccY;
float angleX, angleY, angleZ;
float roll, pitch, yaw;

// Para integración temporal
unsigned long preintervalo1 = 0;

// Prototipos
void inicioMPU();
void MPUwrite(byte reg, byte data);
byte MPUread(byte reg);
void calcoffset(bool console);
void mpuupdate();

void inicioMPU() {
  Wire.begin();          // iniciamos I2C
  MPUwrite(0x19, 0x00);  // SMPLRT_DIV
  MPUwrite(0x1a, 0x00);  // CONFIG
  MPUwrite(0x1b, 0x08);  // GYRO_CONFIG (+/- 500 deg/s)
  MPUwrite(0x1c, 0x00);  // ACCEL_CONFIG (+/- 2g)
  MPUwrite(0x6b, 0x01);  // PWR_MGMT_1
  
  preintervalo1 = millis();
  calcoffset(true);
}

void MPUwrite(byte reg, byte data) {
  Wire.beginTransmission(0x68);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission();
}

byte MPUread(byte reg) {
  Wire.beginTransmission(0x68);
  Wire.write(reg);
  Wire.endTransmission(true);
  Wire.requestFrom(0x68, 1);
  return Wire.read();
}

void calcoffset(bool console) {
  float x = 0, y = 0, z = 0;
  int16_t rx, ry, rz;

  delay(1000);

  for (int i = 1; i <= 3000; i++) {
    Wire.beginTransmission(0x68);
    Wire.write(0x43);  // registro giroscopio X
    Wire.endTransmission(false);
    Wire.requestFrom((int)0x68, 6);

    rx = Wire.read() << 8 | Wire.read();
    ry = Wire.read() << 8 | Wire.read();
    rz = Wire.read() << 8 | Wire.read();

    x += ((float)rx)/65.5;
    y += ((float)ry)/65.5;
    z += ((float)rz)/65.5;
  }

  gyroXoffset = x / 3000.0;
  gyroYoffset = y / 3000.0;
  gyroZoffset = z / 3000.0;

  if(console){
    Serial.println("Offsets calculados:");
    Serial.print("X: "); Serial.println(gyroXoffset);
    Serial.print("Y: "); Serial.println(gyroYoffset);
    Serial.print("Z: "); Serial.println(gyroZoffset);
  }
}

void mpuupdate() {
  int16_t rawAccX, rawAccY, rawAccZ, rawTemp;
  int16_t rawGyroX, rawGyroY, rawGyroZ;

  Wire.beginTransmission(0x68);
  Wire.write(0x3B);  // primer registro a leer
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
  temp = (rawTemp + 12412.0)/340.0;
  accX = ((float)rawAccX)/16384.0;
  accY = ((float)rawAccY)/16384.0;
  accZ = ((float)rawAccZ)/16384.0;

  // Ángulos acelerómetro
  angleAccX = atan2(accY, sqrt(accZ*accZ + accX*accX))*180.0/PI;
  angleAccY = atan2(accX, sqrt(accZ*accZ + accY*accY))*-180.0/PI;

  // Giroscopio
  gyroX = ((float)rawGyroX)/65.5 - gyroXoffset;
  gyroY = ((float)rawGyroY)/65.5 - gyroYoffset;
  gyroZ = ((float)rawGyroZ)/65.5 - gyroZoffset;

  // Delta tiempo en segundos
  static unsigned long lastMicros = micros();
  float dt = (micros() - lastMicros)/1000000.0;
  lastMicros = micros();

  // Integración giroscopio
  angleGyroX += gyroX * dt;
  angleGyroY += gyroY * dt;
  angleGyroZ += gyroZ * dt;

  // Filtro complementario para roll y pitch
  float alpha = 0.98;  // 0.98 giroscopio / 0.02 acelerómetro
  angleX = alpha * (angleX + gyroX*dt) + (1-alpha) * angleAccX;
  angleY = alpha * (angleY + gyroY*dt) + (1-alpha) * angleAccY;

  // Yaw acumulado (sin normalizar)
  static float yaw_suave = 0;
  yaw_suave = alpha * (yaw_suave + gyroZ*dt) + (1-alpha) * yaw_suave;
  yaw = yaw_suave;

  // Salida
  roll = angleX;
  pitch = angleY;
  angleZ = angleGyroZ; // para uso directo si quieres acumular giros
}
