#include <Arduino.h>

#include <Wire.h>

void I2CScanning() {
  Serial.println("I2C Scanning will start after 1 sec.");
  delay(1000);
  
  Serial.println("I2C scanner. Scanning...");
  byte count = 0;

  for (byte i = 1; i < 200; i++) {
    Wire.beginTransmission(i);
    if (Wire.endTransmission() == 0) {
      Serial.print("Found address: ");
      Serial.print(i, DEC);
      Serial.print(" (0x");
      Serial.print(i, HEX);
      Serial.println(")");
      count++;
      delay(5); // maybe unneeded?
    }
  }

  Serial.println("I2C scanning done.");
  Serial.print("Found ");
  Serial.print(count, DEC);
  Serial.println(" device(s).");

  int i2c_scanning_delay = 3; /* in sec */
  Serial.print("next scanning will delay after "); Serial.print(i2c_scanning_delay); Serial.println(" secs.");
  delay(i2c_scanning_delay * 1000);
}