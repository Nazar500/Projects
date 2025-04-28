#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println("\nI2C Scanner");
}

void loop() {
  int error, address, Devices = 0;
  Serial.println("\nScanning...");
  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C Device found at 0x");
      if (address < 16)
        Serial.print('0');
      Serial.print(address, HEX);
      Serial.println("  !");
      Devices++;
    }
  }
  delay(1000);
}
