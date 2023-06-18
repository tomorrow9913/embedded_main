#include <LiquidCrystal.h>

#define RS      44
#define E       45
#define DB4     46
#define DB5     47
#define DB6     48
#define DB7     49

LiquidCrystal lcd(RS, E, DB4, DB5, DB6, DB7);

void setup() {
    lcd.begin(16, 2);
}

void PrintTotalPrice(int Price){
    char buffer[16];
    lcd.setCursor(0, 0);
    lcd.print("Total Price");
    lcd.setCursor(0, 1);
    sprintf(buffer, "%d", Price);
    lcd.print(buffer);
    lcd.setCursor(13, 1);
    lcd.print("won");
}

void loop() {
    PrintTotalPrice(99999)
}

