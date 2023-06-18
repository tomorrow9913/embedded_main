// mega2560
#include <openGLCD.h>
#include <openGLCD_Buildinfo.h>
#include <openGLCD_Config.h>
#include <String.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

// 버튼 번호
#define BTN_DOWN 14
#define BTN_UP 15
#define BTN_LEFT 16
#define BTN_RIGHT 17

// 메뉴 번호
#define MENU_CART 0
#define MENU_LIST 1
#define MENU_TOTAL 2

bool dataInit = false;

char* reqItem = "http -a admin:admin get :3000/item --pretty=none --print=b";

// Server 통신용
void sendServer(char ch) {
  // Serial.print(ch);
  // ss.write(ch);
  Serial.write(ch);
}

void sendServer(const char* str) {
  for (int i = 0; i < strlen(str); i++) {
    sendServer(str[i]);
  }
  sendServer('\n');
}

// Uno 통신용 
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


struct Product {
  String id = "";
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


// 기본 데이터 설정
void InitData() {
  String json;
  // 수신된 데이터를 String으로 수집
  while (Serial.available()) {
    char ch = Serial.read();
    json += ch;
    delay(1);
  }

  //Serinal.println(json);
  const size_t CAPACITY = JSON_ARRAY_SIZE(200);
  StaticJsonDocument<CAPACITY> doc;

  DeserializationError error = deserializeJson(doc, json);

  if (error) {
    Serial.print("Deserialization error: ");
    Serial.println(error.c_str());
    return;
  }

  int i = 0;
  
  JsonArray data = doc["data"];
  for(const JsonObject& item : data) {
    const char* id = item["Id"];
    const char* name = item["Name"];
    int price = item["Price"];
    int count = item["Count"];

    product[i].id = String(id);
    product[i].name = String(name);
    product[i].price = price;
    product[i].count = count;
    i ++;
  }
  listMax = i;
  //sendWire(Serial.read());
}




void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  InitGLCD();
  //InitData();
  InitCart();

  sendServer(reqItem);
}

void loop() {
  GLCD.Init();

  ShowMenu();

  if(Serial.available()) {
    if(!dataInit) {
      InitData();
      dataInit = true;
    }
    // sendWire(Serial.read());
    Serial1.write(Serial.read());
  }
  if (Serial1.available()) {
    // UNO -> MEGA2560
    Serial.write(Serial1.read());
  }

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

  delay(50);
}
