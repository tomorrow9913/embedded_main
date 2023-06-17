#include <openGLCD.h>
#include <openGLCD_Buildinfo.h>
#include <openGLCD_Config.h>
#include <String.h>
#include <ArduinoJson.h>
#include <Wire.h>
#define SLAVE 4  // 슬레이브 주소

#define BTN_DOWN 14
#define BTN_UP 15
#define BTN_LEFT 16
#define BTN_RIGHT 17

#define MENU_CART 0
#define MENU_LIST 1
#define MENU_TOTAL 2


struct Product {
  String name = "non";
  int price = 1000;
  int count = 10;
};

struct Cart {
  int productItemId = 0;
};

struct Transaction {
  Cart* cart;
};


Product product[10];
int listNum = 0;
int listMax = 10;

int menu = MENU_CART;

Cart mycart[10];
int cartNum = 0;
int cartMax = 5;

Transaction total[10];
int totalNum = 0;
int totalMax = 3;

// 기본 데이터 설정
void InitData() {
  for (int i = 0; i < listMax; i++) {
    product[i].name = String(product[i].name + (i + 1));
  }
}

void InitCart() {
  for (int i = 0; i < cartMax; i++) {
    mycart[i].productItemId = (2 * i) % listMax;
  }
}

void InitTotal() {
  for (int i = 0; i < totalMax; i++) {
    total[i].cart = mycart;
  }
}

// GLCD 화면 출력
void InitGLCD() {
  Serial.begin(9600);
  for (int i = 14; i < 18; i++) {
    pinMode(i, INPUT);
  }

  GLCD.Init();
  GLCD.SelectFont(System5x7);
}

void ShowMenu() {
  // 첫 줄
  GLCD.CursorToXY(2, 0);
  GLCD.print("Help Me Pos");
  GLCD.DrawLine(0, 8, 127, 8);

  // 끝 줄
  GLCD.DrawLine(0, 53, 127, 53);
  GLCD.CursorToXY(10, 56);
  GLCD.print("CART");
  GLCD.CursorToXY(52, 56);
  GLCD.print("LIST");
  GLCD.CursorToXY(91, 56);
  GLCD.print("TOTAL");

  // 메뉴 선택
  GLCD.DrawRoundRect(5 + menu * 42, 55, 33, 9, 2);
}

void ShowList() {
  for (int i = 0; i < ((listMax < 4) ? listMax : 4); i++) {
    GLCD.CursorToXY(2, i * 11 + 11);
    GLCD.print(i + listNum + 1);

    GLCD.CursorToXY(20, i * 11 + 11);
    GLCD.print(product[i + listNum].name);

    GLCD.CursorToXY(70, i * 11 + 11);
    GLCD.print(product[i + listNum].price);

    GLCD.CursorToXY(110, i * 11 + 11);
    GLCD.print(product[i + listNum].count);
  }
}


void ShowCart() {
  for (int i = 0; i < ((cartMax < 4) ? cartMax : 4); i++) {
    GLCD.CursorToXY(2, i * 11 + 11);
    GLCD.print(i + cartNum + 1);

    GLCD.CursorToXY(20, i * 11 + 11);
    GLCD.print(product[mycart[i + cartNum].productItemId].name);

    GLCD.CursorToXY(70, i * 11 + 11);
    GLCD.print(product[mycart[i + cartNum].productItemId].price);

    GLCD.CursorToXY(110, i * 11 + 11);
    GLCD.print(product[mycart[i + cartNum].productItemId].count);
  }
}



// I2C 통신 (mega2560 - uno)
void InitI2C() {
  Wire.begin(4);
  Wire.onReceive(receiveEvent);

  Serial.println("Serial Port Connected!");
}

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



void setup() {
  InitGLCD();
  InitData();
  InitCart();
  InitI2C();
}

void loop() {
  GLCD.Init();

  ShowMenu();

  // 각각의 메뉴별 처리항목
  if (menu == MENU_CART) {
    ShowCart();

    // 버튼 처리
    if (digitalRead(BTN_UP) && cartNum < cartMax - 4) {
      cartNum++;
    } else if (digitalRead(BTN_DOWN) && cartNum > 0) {
      cartNum--;
    }
  } else if (menu == MENU_LIST) {
    ShowList();

    // 버튼 처리
    if (digitalRead(BTN_UP) && listNum < listMax - 4) {
      listNum++;
    } else if (digitalRead(BTN_DOWN) && listNum > 0) {
      listNum--;
    }
  } else if (menu == MENU_TOTAL) {
  }

  // 버튼 처리
  if (!digitalRead(BTN_LEFT) && menu > 0) {
    menu--;
  } else if (!digitalRead(BTN_RIGHT) && menu < 2) {
    menu++;
  }

  // I2C통신 READ
  if (Serial.available()) {
    sendWire(Serial.read());
  }

  delay(50);
}


// 시리얼로 들어온 메시지 처리
void serialEvent() {
  //Serial.write(Serial.read());
  sendWire(Serial.read());
  delay(10);
}