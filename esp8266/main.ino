#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h> 
#include <SoftwareSerial.h>

// wifi id, pw 정보
#ifndef STASSID
#define STASSID "DoD_2.4G"
#define STAPSK ""
#endif

// 기존의 자료에서는 따로 핀 번호를 설정하지 않고 D5, D6로 되어있기 때문에 오류 발생
// 아두이노와 통신하는 핀 번호 
#define D5 14
#define D6 12

// wifi 연결 정보 
const char* ssid = STASSID;
const char* password = STAPSK; 

// PORT
ESP8266WebServer    server(80);


// 아두이노와 통신을 하기위한 
SoftwareSerial s(D6, D5);

void setup() {
  s.begin(9600);
  Serial.begin(9600);
  
  // wifi 연결
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status( ) != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP( ));
  
  // 웹브라우저에서 ip 주소로 접속했을 때 보여주는 문구
  server.on("/", [ ]( ){
    server.send(200, "text/html", "<a href='/on'>on</a><br><a href='/off'>off</a>");
  });

  // LED 켜기
  server.on("/on", [ ]( ){
    s.write(1);
    server.send(200, "text/html", "<a href='/on'>[on]</a><br><a href='/off'>off</a>");
  });
  
  // LED 끄기
  server.on("/off", [ ]( ){
    s.write(2);
    server.send(200, "text/html", "<a href='/on'>on</a><br><a href='/off'>[off]</a>");
  });
  
  //웹서버 시작
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient( );//클라이언트 접속을 관리합니다.
}