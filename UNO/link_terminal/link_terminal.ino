// UNO

#include <SoftwareSerial.h>

SoftwareSerial ss(6, 7);

void setup() {
  Serial.begin(9600);
  ss.begin(9600);
}

void loop() {
  if (Serial.available()) {
    // UNO -> MEGA2560
    ss.write(Serial.read());
  }

  if (ss.available()) {
    // MEGA2560 -> UNO
    Serial.write(ss.read());
  }
}