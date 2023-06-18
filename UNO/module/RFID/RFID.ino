#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <deprecated.h>
#include <require_cpp11.h>

#define RST_PIN   9
#define SS_PIN    10
#define LED1   4
#define LED2   3

#define LED_PIN 7
#define BTN     2

MFRC522 rfid(SS_PIN, RST_PIN); // 'rfid' 이름으로 클래스 객체 선언
MFRC522::MIFARE_Key key; 
byte nuidPICC[4];   // 카드 ID들을 저장(비교)하기 위한 배열(변수)선언

bool btn_on = true;

bool success = false;

int code = 999;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);


  SPI.begin();
  rfid.PCD_Init();

  for(int i = 0; i < 6; i ++) {
    key.keyByte[i] = 0xFF;
  }

  pinMode(LED_PIN, OUTPUT);
  pinMode(LED1, OUTPUT);    
  pinMode(LED2, OUTPUT);    
  pinMode(BTN, INPUT);

  code = random(10000, 99999);
  Serial.println(code);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  if(btn_on) {
    digitalWrite(LED1, LOW);   
    digitalWrite(LED2, HIGH);

    if(rfid.PICC_IsNewCardPresent()){
      if(rfid.PICC_ReadCardSerial()) {
        Serial.print("Card ID: ");
        for(int i = 0; i < 4; i ++) {
          Serial.print(rfid.uid.uidByte[i], HEX);
          Serial.print(" ");
        }
        Serial.println();
        btn_on = false;  
      }
    }
  }
  else {
    digitalWrite(LED1, HIGH);   
    digitalWrite(LED2, LOW);
  }

  if(digitalRead(BTN)) {
    btn_on = !btn_on;
    delay(500);
  }
}
