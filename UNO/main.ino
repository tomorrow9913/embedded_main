#include <SoftwareSerial.h>

#define LED_PIN 7
// 기존 자료는 12번으로 되어있는데 12번 핀에는 아무것도 연결이 되어있지 않음.
// 따라서 led와 연결되어있는 7번 핀으로 선택해줌.

SoftwareSerial s(5,6);

void setup() {
  s.begin(9600);
  Serial.begin(9600);

  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  if(s.available()>0) {
    int result = 0;

    result = s.read();
    Serial.println(result);

    if(result == 1) { 
      digitalWrite(LED_PIN, HIGH);
    } 
		else if(result == 2) { 
      digitalWrite(LED_PIN, LOW);
    }
  }
  delay(100);
}