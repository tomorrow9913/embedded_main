// UNO

#include <SoftwareSerial.h>
#include <string.h>
#include <ArduinoJson.h>

#define BTN 2

SoftwareSerial ss(6, 7);

char* req = "http -a admin:admin get :3000/item/8808024031923 --pretty=none --print=b";


void sendWire(char ch) {
  Serial.print(ch);
  ss.write(ch);
}

void sendWire(const char* str) {
  for (int i = 0; i < strlen(str); i++) {
    sendWire(str[i]);
  }
  sendWire('\n');
}

void setup() {
  Serial.begin(9600);
  ss.begin(9600);

  pinMode(BTN, INPUT);
}

void loop() {
  if (Serial.available()) {
    // UNO -> MEGA2560
    ss.write(Serial.read());
  }

  if(digitalRead(BTN)) {
    Serial.println("btn");
    sendWire(req);
    delay(100);
  }

  if (ss.available()) {
    // MEGA2560 -> UNO
    String json;
    
    // 수신된 데이터를 String으로 수집
    while (ss.available()) {
      json += (char)ss.read();
      delay(1);
    }

    const size_t CAPACITY = JSON_OBJECT_SIZE(100);
    StaticJsonDocument<CAPACITY> doc;
    DeserializationError error = deserializeJson(doc, json);

    if (error) {
      Serial.print("Deserialization error: ");
      Serial.println(error.c_str());
      return;
    }
    const char* id = doc["Id"];
    int price = doc["Price"];
    int count = doc["Count"];

    Serial.print("Id: ");
    Serial.println(id);
    Serial.print("Price: ");
    Serial.println(price);
    Serial.print("Count: ");
    Serial.println(count);
  }
}