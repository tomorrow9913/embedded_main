#include <openGLCD.h>
#include <openGLCD_Buildinfo.h>
#include <openGLCD_Config.h>
#include <String.h>

#define  BTN_UP    14
#define  BTN_DOWN  15
#define  BTN_LEFT  16
#define  BTN_RIGHT 17

struct Product {
  int num = 0;
  String name = "non";
  int price = 10000;
  int count = 10;
};

Product product[10];
int line = 0;
int menu = 1;


void ShowMenu() {
  // 첫 줄
  GLCD.CursorToXY(2, 0);
  GLCD.print("Help Me Pos");
  GLCD.DrawLine(0, 8, 127, 8);

  // 끝 줄
  GLCD.DrawLine(0, 53, 127, 53);
  GLCD.CursorToXY(0, 56);
  GLCD.print("  pay  lis  day  all");
}

void ShowList() {
  for(int i = 0; i < 4; i ++) {
    GLCD.CursorToXY(2, i * 11 + 11);
    GLCD.print(product[(i + line)% 10].num);

    GLCD.CursorToXY(20, i * 11 + 11);
    GLCD.print(product[(i + line)% 10].name);

    GLCD.CursorToXY(70, i * 11 + 11);
    GLCD.print(product[(i + line)% 10].price);

    GLCD.CursorToXY(110, i * 11 + 11);
    GLCD.print(product[(i + line)% 10].count);
  }
}


void setup() {
  Serial.begin(9600);
  for(int i = 14; i < 18; i ++) {
    pinMode(i, INPUT);
  }
  
  GLCD.Init();
  GLCD.SelectFont(System5x7);
  
  
  for(int i = 0; i < 10; i ++) {
    product[i].num = i + 1;
    product[i].name = String(product[i].name + (i + 1));
  }
}

void loop() {
  
  ShowMenu();
  ShowList();

  if(digitalRead(BTN_UP)) {
    line ++;
  }
  else if(digitalRead(BTN_DOWN)) {
    line --;
    if(line < 0) line = 9;
  }
  else if(!digitalRead(BTN_LEFT)) {
    menu --;
    if(menu < 0) menu = 3;
  }
  else if(!digitalRead(BTN_RIGHT)) {
    menu ++;
    menu %= 4;
  }
  GLCD.DrawRoundRect(8 + menu * 30, 55, 25, 9, 2);

  line %= 10;
  delay(100);

  GLCD.Init();
}