#include <Wire.h>
#define SLAVE 4 // 슬레이브 주소

#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <deprecated.h>
#include <require_cpp11.h>

#include <string.h>

#include <SoftwareSerial.h>

#define RST_PIN   9
#define SS_PIN    10
#define LED1   4
#define LED2   3

#define LED_PIN 7
#define BTN     12

MFRC522 rfid(SS_PIN, RST_PIN); // 'rfid' 이름으로 클래스 객체 선언
MFRC522::MIFARE_Key key; 
byte nuidPICC[4];   // 카드 ID들을 저장(비교)하기 위한 배열(변수)선언

bool btn_on = true;

bool success = false;

// MFRC522 rc522(SS_PIN, RST_PIN);
SoftwareSerial s(5,6);


void sendWire(char ch) {
  Serial.print("SEND: ");
  Serial.println(ch);
  
  Wire.beginTransmission(SLAVE);
  Wire.write(ch);
  Wire.endTransmission();
}

void sendWire(const char* str) {
  for (; str != '\0'; ++str) {
    sendWire(*str);
  }
}

void receiveEvent(int howMany) {
  Serial.print("RECV: ");
  
  while (1 < Wire.available()) {
    char c = Wire.read();
    Serial.print(c);
  }
  
  char x = Wire.read();
  Serial.println(x);
}

// 16진 값으로 변환 해주는 함수 정의

void printHex(byte *buffer, byte bufferSize) {

  for (byte i = 0; i < bufferSize; i++) {

    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);

  }

}

// 10진 값으로 변환 해주는 함수 정의

void printDec(byte *buffer, byte bufferSize) {
  sendWire((char*) buffer);
  for (byte i = 0; i < bufferSize; i++) {
    sendWire(" ");
    if(buffer[i] < 0x10) sendWire("0");
    sendWire((char)(buffer[i]+'0'));

  }
}


void setup() {
  s.begin(9600);
  Serial.begin(9600);
  
  Wire.begin(LED1);
  Wire.onReceive(receiveEvent);

  SPI.begin();
  rfid.PCD_Init();

  for(int i = 0; i < 6; i ++) {
    key.keyByte[i] = 0xFF;
  }

  pinMode(LED_PIN, OUTPUT);
  pinMode(LED1, OUTPUT);    
  pinMode(LED2, OUTPUT);    
  pinMode(BTN, INPUT);      
}

void loop() {
  // i2c 통신 Mega2560 
  if (Serial.available()) {
    sendWire(Serial.read());
  }
   // 새카드 접촉이 있을 때만 다음 단계로 넘어감
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

 // 카드 읽힘이 제대로 되면 다음으로 넘어감
  if ( ! rfid.PICC_ReadCardSerial())

    return;

  // 현재 접촉 되는 카드 타입을 읽어와 모니터에 표시함

  Serial.print(F("PICC type: "));

  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

  Serial.println(rfid.PICC_GetTypeName(piccType));

  // MIFARE 방식의 카드인지 확인 루틴

  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI && 

    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&

    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {

    Serial.println(F("Your tag is not of type MIFARE Classic."));

    //return;

  }

   // 이전 인식된 카드와 다른, 혹은 새카드가 인식되면

  if (rfid.uid.uidByte[0] != nuidPICC[0] ||

    rfid.uid.uidByte[1] != nuidPICC[1] ||

    rfid.uid.uidByte[2] != nuidPICC[2] ||

    rfid.uid.uidByte[3] != nuidPICC[3] ) {

    sendWire(F("A new card has been detected."));

 // 고유아이디(UID) 값을 저장한다.

    for (byte i = 0; i < 4; i++) {

      nuidPICC[i] = rfid.uid.uidByte[i];

    }

 // 그 UID 값을 16진값으로 출력 한다.

    Serial.println(F("The NUID tag is:"));

    Serial.print(F("In hex: "));

    printHex(rfid.uid.uidByte, rfid.uid.size);

    Serial.println();

 // 그 UID 값을 10진값으로 출력 한다.

    Serial.print(F("In dec: "));

    printDec(rfid.uid.uidByte, rfid.uid.size);

    Serial.println();

    success=true;
  }

  else {
    Serial.println(F("Card read previously."));
    success = false;
  }

  rfid.PICC_HaltA();

  rfid.PCD_StopCrypto1();


  if (s.available()) {
    Serial.print(s.read());
  }

  if(success) {
    digitalWrite(LED1, LOW);   
    digitalWrite(LED2, HIGH);
  }
  else {
    digitalWrite(LED1, HIGH);   
    digitalWrite(LED2, LOW);
  }
  if(digitalRead(BTN)) {
    btn_on = true;

  }

  delay(500);
}
