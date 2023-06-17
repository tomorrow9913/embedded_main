// MEGA2560

#include <SoftwareSerial.h>
#include <string.h>
#include <ArduinoJson.h>


void sendWire(char ch) {
  Serial.print(ch);
  Serial1.write(ch);
}

void sendWire(const char* str) {
  for (int i = 0; i < strlen(str); i++) {
    sendWire(str[i]);
  }
  sendWire('\n');
}


void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
}

void loop() {
  if (Serial1.available()) {
    // UNO -> MEGA2560
    Serial.write(Serial1.read());
  }

  if (Serial.available()) {
    // MEGA2560 -> UNO
    Serial1.write(Serial.read());
  }
}

