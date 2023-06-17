// UNO

#include <SoftwareSerial.h>
#include <string.h>
#include <ArduinoJson.h>

#define BTN 2

SoftwareSerial ss(6, 7);

char* req = "http -a admin:admin get :3000/item/8808024031923 --pretty=none --print=b";

char* Id = "admin";
char* pass = "admin";
int port = 3000;


/////// Item ///////
String CreateProductCmdBuilder(String productID, int price, int count){
  // target: http -a {id}:{password} {method} :{port}/{path} id={itemId} price:={price} count:={count}
  char cmdBuffer[1024] = {0};
  sprintf(cmdBuffer, "http -a admin:admin post :3000/item id=%s price:=%d count:=%d",, productID, price, count);
  String cmd = String(cmdBuffer);
  return cmd; 
}

String ReadProductCmdBuilder(String productID = NULL){
  if (productID == NULL){
    // target: http -a admin:admin get :3000/item --pretty=none --print=b
    char cmdBuffer[1024] = {0};
    sprintf(cmdBuffer, "http -a admin:admin get :3000/item --pretty=none --print=b");
    String cmd = String(cmdBuffer);
    return cmd;
  }

  // target: http -a {id}:{password} {method} :{port}/{path}/{itemID} --pretty=none --print=b 
  char cmdBuffer[1024] = {0};
  sprintf(cmdBuffer, "http -a admin:admin get :3000/item/%s --pretty=none --print=b", itemId);
  String cmd = String(cmdBuffer);
  return cmd;
}

String UpdateProductCmdBuilder(String productID, int price = NULL, int count = NULL){
  if(price == NULL || count == NULL){
    // http -a admin:admin patch :3000/item id=${상품 고유번호} price:=${상품 가격} count:=${ 상품 개수}
    String cmd = "http -a admin:admin patch :3000/item";
    if (productID) cmd += " id=" + productID;
    if (price) cmd += " price:=" + price;
    if (count) cmd += " count:=" + count;
    cmd += " --pretty=none --print=b"
      
    return cmd;
  }
  // target: http -a admin:admin put :3000/item id=${상품 고유번호} price:=${상품 가격} count:=${상품 개수}
  char cmdBuffer[1024] = {0};
  sprintf(cmdBuffer, "http -a admin:admin put :3000/item id=%s price:=%d count:=%d --pretty=none --print=b", productID, price, count);
  String cmd = String(cmdBuffer);
  return cmd;
}

String DeleteProductCmdBuilder(String productID){
  // target: http -a {id}:{password} {method} :{port}/{path}/{itemID} --pretty=none --print=b 
  char cmdBuffer[1024] = {0};
  sprintf(cmdBuffer, "http -a admin:admin delete :3000/item/%s", itemId);
  String cmd = String(cmdBuffer);
  return cmd;
}

/////// User ///////
String CreateUserCmdBuilder(){
  char cmdBuffer[1024] = {0};

}

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