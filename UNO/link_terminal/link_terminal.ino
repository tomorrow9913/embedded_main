// UNO

#include <SoftwareSerial.h>
#include <string.h>
#include <ArduinoJson.h>

#define BTN 2

SoftwareSerial ss(6, 7);

char* req = "http -a admin:admin get :3000/item/8808024031923 --pretty=none --print=b";

Typedef enum TypeOfCommand{
  none,

  CreateProduct,
  ReadProduct,
  UpdateProduct,
  DeleteProduct,

  CreateUser,
  ReadUser,
  UpdateUser,
  DeleteUser,

  Sync,

  ReadPurchase,
  DeletePurchase,
  DeletePurchaseInItem,
  SignPurchase,
  ReadLog,

  CreateEtcInfo,
  ReadEtcInfo,
  UpdateEtcInfo,
  DeleteEtcInfo
} TypeOfCommand;

class CmdManager {
  praivate:
    TypeOfCommand recentCmd = TypeOfCommand.none;

  public:
  /////// Item ///////
  String CreateProductCmdBuilder(String productID, int price, int count){
    // target: http -a {id}:{password} {method} :{port}/{path} id={itemId} price:={price} count:={count}
    this->recentCmd = TypeOfCommand.CreateProduct;
    
    char cmdBuffer[1024] = {0};
    sprintf(cmdBuffer, "http -a admin:admin post :3000/item id=%s price:=%d count:=%d --pretty=none --print=b", productID, price, count);
    String cmd = String(cmdBuffer);
    return cmd; 
  }

  String ReadProductCmdBuilder(String productID = ""){
    this->recentCmd = TypeOfCommand.ReadProduct;
    
    if (productID == ""){
      // target: http -a admin:admin get :3000/item --pretty=none --print=b
      char cmdBuffer[1024] = {0};
      sprintf(cmdBuffer, "http -a admin:admin get :3000/item --pretty=none --print=b");
      return String(cmdBuffer);
    }

    // target: http -a {id}:{password} {method} :{port}/{path}/{itemID} --pretty=none --print=b 
    char cmdBuffer[1024] = {0};
    sprintf(cmdBuffer, "http -a admin:admin get :3000/item/%s --pretty=none --print=b", productID);
    String cmd = String(cmdBuffer);
    return cmd;
  }

  String UpdateProductCmdBuilder(String productID, int price = NULL, int count = NULL){
    this->recentCmd = TypeOfCommand.UpdateProduct;

    if(price == NULL || count == NULL){
      // http -a admin:admin patch :3000/item id=${상품 고유번호} price:=${상품 가격} count:=${ 상품 개수}
      String cmd = "http -a admin:admin patch :3000/item";
      if (productID) cmd += " id=" + productID;
      if (price) cmd += " price:=" + price;
      if (count) cmd += " count:=" + count;
      return cmd + " --pretty=none --print=b";  
    }
    // target: http -a admin:admin put :3000/item id=${상품 고유번호} price:=${상품 가격} count:=${상품 개수}
    char cmdBuffer[1024] = {0};
    sprintf(cmdBuffer, "http -a admin:admin put :3000/item id=%s price:=%d count:=%d --pretty=none --print=b", productID, price, count);
    String cmd = String(cmdBuffer);
    return cmd;
  }

  String DeleteProductCmdBuilder(String productID){
    // target: http -a {id}:{password} {method} :{port}/{path}/{itemID} —pretty=none —print=b 
    this->recentCmd = TypeOfCommand.DeleteProduct;

    char cmdBuffer[1024] = {0};
    sprintf(cmdBuffer, "http -a admin:admin delete :3000/item/%s --pretty=none --print=b", productID);
    String cmd = String(cmdBuffer);
    return cmd;
  }

  /////// User ///////
  String CreateUserCmdBuilder(String cardId, int balance){
    // target: http -a admin:admin post :3000/user id=${사용자 카드번호} balance:=${잔액}
    this->recentCmd = TypeOfCommand.CreateUser;

    char cmdBuffer[1024] = {0};
    sprintf(cmdBuffer, "http -a admin:admin post :3000/user id=%s balance:=%d --pretty=none --print=b", cardId, balance);
    String cmd = String(cmdBuffer);
    return cmd;
  }

  String ReadUserCmdBuilder(String cardId){
    // target: http -a admin:admin get :3000/user/${사용자 카드번호}
    this->recentCmd = TypeOfCommand.ReadUser;

    char cmdBuffer[1024] = {0};
    sprintf(cmdBuffer, "http -a admin:admin get :3000/user/%d --pretty=none --print=b", cardId);
    String cmd = String(cmdBuffer);
    return cmd;
  }

  String UpdateUserCmdBuilder(String cardId, int balance){
    // target: http -a admin:admin patch :3000/user id=${사용자 카드번호} balance:=${잔액}
    this->recentCmd = TypeOfCommand.UpdateUser;

    char cmdBuffer[1024] = {0};
    sprintf(cmdBuffer, "http -a admin:admin patch :3000/user id=%s balance:=%d --pretty=none --print=b", cardId, balance);
    String cmd = String(cmdBuffer);
    return cmd;
  }

  String DeleteUserCmdBuilder(String cardId){
    // target: http -a admin:admin delete :3000/user/${사용자 카드번호}
    this->recentCmd = TypeOfCommand.DeleteUser;

    char cmdBuffer[1024] = {0};
    sprintf(cmdBuffer, "http -a admin:admin delete :3000/user/%s --pretty=none --print=b", cardId);
    String cmd = String(cmdBuffer);
    return cmd;
  }

  ///////// Sync /////////
  String SyncCmdBuilder(String PurchaseId){
    // target: http get :3000/sync/${고유번호}
    this->recentCmd = TypeOfCommand.Sync;

    char cmdBuffer[1024] = {0};
    sprintf(cmdBuffer, "http get :3000/sync/%s --session=./session.json --pretty=none --print=b", PurchaseId);
    String cmd = String(cmdBuffer);
    return cmd;
  }

  ///////// Purchase /////////
  String ReadPurchaseCmdBuilder(){
    // target: http --session=./session.json get :3000/purchase --pretty=none --print=b
    this->recentCmd = TypeOfCommand.ReadPurchase;
    char cmdBuffer[1024] = {0};
    sprintf(cmdBuffer, "http --session=./session.json get :3000/purchase --pretty=none --print=b");
    String cmd = String(cmdBuffer);
    return cmd;
  }

  String DeletePurchaseCmdBuilder(){
    // target: http --session=./session.json delete :3000/purchase --pretty=none --print=b
    this->recentCmd = TypeOfCommand.DeletePurchase;

    char cmdBuffer[1024] = {0};
    sprintf(cmdBuffer, "http --session=./session.json delete :3000/purchase --pretty=none --print=b");
    String cmd = String(cmdBuffer);
    return cmd;
  }

  String DeletePurchaseInItemCmdBuilder(String productID){
    // target: http --session=./session.json delete :3000/purchase/item/${상품 고유번호} --pretty=none --print=b
    this->recentCmd = TypeOfCommand.DeletePurchaseInItem;

    char cmdBuffer[1024] = {0};
    sprintf(cmdBuffer, "http --session=./session.json delete :3000/purchase/item/%s --pretty=none --print=b", productID);
    String cmd = String(cmdBuffer);
    return cmd;
  }

  String SignPurchaseCmdBuilder(String cardId){
    // target: http --session=./session.json get :3000/purchase/sign/${사용자 카드번호} --pretty=none --print=b
    this->recentCmd = TypeOfCommand.SignPurchase;

    char cmdBuffer[1024] = {0};
    sprintf(cmdBuffer, "http --session=./session.json get :3000/purchase/sign/%s --pretty=none --print=b", cardId);
    String cmd = String(cmdBuffer);
    return cmd;
  }

  ///////// Log /////////
  String ReadLog(){
    // target: http -a admin:admin get :3000/log --pretty=none --print=b
    this->recentCmd = TypeOfCommand.ReadLog;

    char cmdBuffer[1024] = {0};
    sprintf(cmdBuffer, "http -a admin:admin get :3000/log --pretty=none --print=b");
    String cmd = String(cmdBuffer);
    return cmd;
  }

  String ReadLog(int Id){
    this->recentCmd = TypeOfCommand.ReadLog;

    char cmdBuffer[1024] = {0};
    sprintf(cmdBuffer, "http -a admin:admin get :3000/log/%d --pretty=none --print=b", Id);
    String cmd = String(cmdBuffer);
    return cmd;
  }

  String ReadLog(String cardId){
    this->recentCmd = TypeOfCommand.ReadLog;

    char cmdBuffer[1024] = {0};
    sprintf(cmdBuffer, "http -a admin:admin get :3000/log/user/%s --pretty=none --print=b", cardId);
    String cmd = String(cmdBuffer);
    return cmd;
  }

  ///////// Etc. /////////
  String CreateEtcInfo(String key, String value){
    this->recentCmd = TypeOfCommand.CreateEtcInfo;

    char cmdBuffer[1024] = {0};
    sprintf(cmdBuffer, "http -a admin:admin post :3000/info id=%s value=%s --pretty=none --print=b", key, value);
    String cmd = String(cmdBuffer);
    return cmd;
  }

  String ReadEtcInfo(String key){
    this->recentCmd = TypeOfCommand.ReadEtcInfo;

    char cmdBuffer[1024] = {0};
    sprintf(cmdBuffer, "http -a admin:admin get :3000/info/%s --pretty=none --print=b", key);
    String cmd = String(cmdBuffer);
    return cmd;
  }

  String UpdateEtcInfo(String key, String value){
    this->recentCmd = TypeOfCommand.UpdateEtcInfo;

    char cmdBuffer[1024] = {0};
    sprintf(cmdBuffer, "http -a admin:admin patch :3000/info id=%s value=$%s --pretty=none --print=b", key, value);
    String cmd = String(cmdBuffer);
    return cmd;
  }

  String DeleteEtcInfo(String key){
    this->recentCmd = TypeOfCommand.DeleteEtcInfo;
    
    char cmdBuffer[1024] = {0};
    sprintf(cmdBuffer, "http -a admin:admin delete :3000/info/%s --pretty=none --print=b", key);
    String cmd = String(cmdBuffer);
    return cmd;
  }

}
//////////////////////////////////////////////////////////////////

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

// Main ////////////////////////////////////////////////////////////////
void loop() {
  if (Serial.available()) {
    // UNO -> MEGA2560
    ss.write(Serial.read());
  }

  // Request /////////////////
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

    switch(recentCmd){
      case TypeOfCmd.none: break;

      case TypeOfCmd.CreateProduct: break;
      case TypeOfCmd.ReadProduct: break;
      case TypeOfCmd.UpdateProduct: break;
      case TypeOfCmd.DeleteProduct: break;

      case TypeOfCmd.CreateUser: break;
      case TypeOfCmd.ReadUser: break;
      case TypeOfCmd.UpdateUser: break;
      case TypeOfCmd.DeleteUser: break;

      case TypeOfCmd.Sync: break;

      case TypeOfCmd.ReadPurchase: break;
      case TypeOfCmd.DeletePurchase: break;
      case TypeOfCmd.DeletePurchaseInItem: break;
      case TypeOfCmd.SignPurchase: break;

      case TypeOfCmd.ReadLog: break;

      case TypeOfCmd.CreateEtcInfo: break;
      case TypeOfCmd.ReadEtcInfo: break;
      case TypeOfCmd.UpdateEtcInfo: break;
      case TypeOfCmd.DeleteEtcInfo: break;
      
      default: 
      recentCmd = TypeofCmd.none;
      break;    
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