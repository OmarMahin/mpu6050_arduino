#include <Wire.h>


float gyroX, calibratedGyroX;

float angleX = 0.0;

int del = 50;

void setup() {

  Wire.setClock(400000);
  Wire.begin();
  delay(250);

  mpu_setup();


  Serial.begin(9600);

  calibration();
}

void loop() {
  calibrated_gyro();

  angleX += gyroX;

  Serial.print("GyroX(deg/s):");
  Serial.print(gyroX);
  Serial.print("  ");
  Serial.print("AngleX(deg):");
  Serial.println(angleX);

  delay(del);
}

void mpu_setup() {

  //Power management
  Wire.beginTransmission(0x68);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();
}

void raw_gyro_data(){
  //low pass filter
  Wire.beginTransmission(0x68);
  Wire.write(0x1A);
  Wire.write(0x05);
  Wire.endTransmission();

  //gyro settings
  Wire.beginTransmission(0x68);
  Wire.write(0x1B);
  Wire.write(0x8);  // LSB sensitivity 65.5
  Wire.endTransmission();

  //Reading gyro data
  Wire.beginTransmission(0x68);
  Wire.write(0x43);
  Wire.endTransmission();

  Wire.requestFrom(0x68, 6);
  int16_t raw_gyro_x = Wire.read() << 8 | Wire.read();

  gyroX = (float) raw_gyro_x;
}

void calibrated_gyro() {
  raw_gyro_data();
  gyroX = (gyroX - calibratedGyroX) * ((del / 1000.0) / 65.5);
}

void calibration() {
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  Serial.print("Calibrating");
  long sum = 0;
  for (int i = 0; i < 2000; i++) {
    if (i % 125 == 0){
      Serial.print(".");
    }
    raw_gyro_data();
    sum += gyroX;
    delay(1);
  }
  calibratedGyroX = sum / 2000;
  Serial.println();
  digitalWrite(13,LOW);
}
