#include <openGLCD.h>
#include <openGLCD_Buildinfo.h>
#include <openGLCD_Config.h>
#include <String.h>

#include <Wire.h>
#define SLAVE 4 // 슬레이브 주소

#define BTN_DOWN  14
#define BTN_UP  15
#define BTN_LEFT  16
#define BTN_RIGHT 17

#define MENU_CART  0
#define MENU_LIST  1
#define MENU_DAY   2
#define MENU_TOTAL 3 


struct Product {
  int id = 0;
  String name = "non";
  int price = 1000;
  int count = 10;
};


Product product[10];
int listNum = 0;
int listMax = 10;

int menu = MENU_CART;


int cart[10];
int cartNum = 0;
int cartMax = 5;


// 기본 데이터 설정
void InitData() {
  for(int i = 0; i < listMax; i ++) {
    product[i].id = i + 1;
    product[i].name = String(product[i].name + (i + 1));
  }
}

void InitCart() {
  for(int i = 0; i < cartMax; i ++) {
    cart[i] = (2 * i) % listMax;
  }
}

// GLCD 화면 출력
void InitGLCD() {
  Serial.begin(9600);
  for(int i = 14; i < 18; i ++) {
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
  GLCD.CursorToXY(0, 56);
  GLCD.print("  pay  lis  day  all");

  // 메뉴 선택
  GLCD.DrawRoundRect(8 + menu * 30, 55, 25, 9, 2);
}

void ShowList() {
  for(int i = 0; i < ((listMax < 4) ? listMax : 4); i ++ ) {
    GLCD.CursorToXY(2, i * 11 + 11);
    GLCD.print(product[i + listNum].id);

    GLCD.CursorToXY(20, i * 11 + 11);
    GLCD.print(product[i + listNum].name);

    GLCD.CursorToXY(70, i * 11 + 11);
    GLCD.print(product[i + listNum].price);

    GLCD.CursorToXY(110, i * 11 + 11);
    GLCD.print(product[i + listNum].count);
  }
}


void ShowCart() {
  for(int i = 0; i < ((cartMax < 4) ? cartMax : 4); i ++ ) {
    GLCD.CursorToXY(2, i * 11 + 11);
    GLCD.print(i + cartNum + 1);

    GLCD.CursorToXY(20, i * 11 + 11);
    GLCD.print(product[cart[i + cartNum]].name);

    GLCD.CursorToXY(70, i * 11 + 11);
    GLCD.print(product[cart[i + cartNum]].price);

    GLCD.CursorToXY(110, i * 11 + 11);
    GLCD.print(product[cart[i + cartNum]].count);
  }
}



// I2C 통신 (mega2560 - uno)
void InitI2C(){
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

  if(menu == MENU_CART) {
    ShowCart();

    // 버튼 처리
    if(digitalRead(BTN_UP) && cartNum < cartMax - 4) {
      cartNum ++;
    }
    else if(digitalRead(BTN_DOWN) && cartNum > 0) {
      cartNum --;
    }
  }
  else if(menu == MENU_LIST) {
    ShowList(); 

    // 버튼 처리
    if(digitalRead(BTN_UP) && listNum < listMax - 4) {
      listNum ++;
    }
    else if(digitalRead(BTN_DOWN) && listNum > 0) {
      listNum --;
    }
  }
  else if(menu == MENU_DAY) {
    
  }
  else if(menu == MENU_TOTAL) {
    
  }

  // 버튼 처리
  if(!digitalRead(BTN_LEFT)) {
    menu --;
    if(menu < 0) menu = 3;
  }
  else if(!digitalRead(BTN_RIGHT)) {
    menu ++;
    menu %= 4;
  }

  // I2C통신 READ
  if (Serial.available()) {
    sendWire(Serial.read());
  }

  delay(50);
}


// 시리얼로 들어온 메시지 처리
void serialEvent(){
    //Serial.write(Serial.read());
    sendWire(Serial.read());
    delay(10);
}