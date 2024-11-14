#include <Wire.h>


float accX, accY, accZ;

float angleX = 0.0;

int del = 50;

void setup() {

  Wire.setClock(400000);
  Wire.begin();
  delay(250);

  mpu_setup();


  Serial.begin(9600);
}

void loop() {
  calibrated_acc();

  angleX = atan(accY / sqrt(accX * accX + accZ * accZ)) * 57.2956;

  Serial.print("AccX(g):");
  Serial.print(accX);
  Serial.print("  ");
  Serial.print("AccY(g):");
  Serial.print(accY);
  Serial.print("  ");
  Serial.print("AccZ(g):");
  Serial.print(accZ);
  Serial.print("  ");
  Serial.print("AngleX:");
  Serial.print(angleX);
  Serial.print("  ");
  Serial.println();


  delay(del);
}

void mpu_setup() {

  //Power management
  Wire.beginTransmission(0x68);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();
}

void raw_acc_data() {

  //low pass filter
  Wire.beginTransmission(0x68);
  Wire.write(0x1A);
  Wire.write(0x05);
  Wire.endTransmission();

  //acc settings
  Wire.beginTransmission(0x68);
  Wire.write(0x1C);
  Wire.write(0x10);  // LSB sensitivity 4096 or 8g
  Wire.endTransmission();

  //Reading acc data
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission();

  Wire.requestFrom(0x68, 6);
  int16_t raw_acc_x = Wire.read() << 8 | Wire.read();
  int16_t raw_acc_y = Wire.read() << 8 | Wire.read();
  int16_t raw_acc_z = Wire.read() << 8 | Wire.read();

  accX = (float)raw_acc_x;
  accY = (float)raw_acc_y;
  accZ = (float)raw_acc_z;
}

void calibrated_acc() {
  raw_acc_data();
  accX = (accX / 4096) - 0.02;  //require manual calibration. Set each value to 0.0 and then adjust it according to the result
  accY = (accY / 4096) + 0.01;
  accZ = (accZ / 4096) + 0.01;
}
