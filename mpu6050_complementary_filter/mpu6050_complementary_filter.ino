#include <Wire.h>


float gyroX, calibratedGyroX;
float accX, accY, accZ;

float angleX_gyro = 0.0;
float angleX_acc = 0.0;
float angleX = 0.0;

float alpha = 0.97;

int del = 50;

void setup() {

  Wire.setClock(400000);
  Wire.begin();
  delay(250);

  mpu_setup();


  Serial.begin(9600);

  gyro_calibration();
}

void loop() {
  gyro_calibrated_data();
  angleX_gyro += gyroX;

  acc_calibrated_data();
  angleX_acc = atan(accY / sqrt(accX * accX + accZ * accZ)) * 57.2956;

  angleX = alpha * (angleX + gyroX) + (1-alpha) * angleX_acc;

  Serial.print("GyroAngle:");
  Serial.print(angleX_gyro);
  Serial.print("    ");
  Serial.print("AccAngle:");
  Serial.print(angleX_acc);
  Serial.print("    ");
  Serial.print("Angle:");
  Serial.println(angleX);

  delay(del);
}

void mpu_setup() {

  //Power management
  Wire.beginTransmission(0x68);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();

  //low pass filter
  Wire.beginTransmission(0x68);
  Wire.write(0x1A);
  Wire.write(0x05);
  Wire.endTransmission();
}



