// MEGA2560

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
