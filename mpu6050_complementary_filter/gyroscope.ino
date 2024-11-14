void raw_gyro_data(){
  
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

void gyro_calibrated_data() {
  raw_gyro_data();
  gyroX = (gyroX - calibratedGyroX) * ((del / 1000.0) / 65.5);
  // gyroX = (gyroX - calibratedGyroX)/65.5;
}

void gyro_calibration() {
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