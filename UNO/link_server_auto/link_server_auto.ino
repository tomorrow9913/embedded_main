#include <SoftwareSerial.h>

SoftwareSerial ss(6, 7);



void SendSerial(const char* str) {
  for (char* ch = str; *ch != '\0'; ++ch) {
    ss.write(*ch);
  }
  ss.write('\n');
}

char* RecvSerial() {
  int index = 0;
  char* buf = (char*)malloc(sizeof(char) * 1024);
  buf[0] = ' ';

  while (buf[index-1] != 10) {
    if (ss.available()) {
      buf[index++] = ss.read();
    }
  }
  
  buf[index] = '\0';
  return buf;
}

void SendRequest(const char* cmd, ...) {
  char* buf = (char*)malloc(sizeof(char) * 1024);

  strcpy(buf, "http ");
  strcat(buf, "-a admin:admin ");
  strcat(buf, "--session=./session.json --pretty=none --print=b ");

  va_list args;
  va_start(args, cmd);
  vsprintf(buf + strlen(buf), cmd, args);

  Serial.println(buf);
  SendSerial(buf);

  free(buf);
  va_end(args);
}














void setup() {
  Serial.begin(9600);
  ss.begin(9600);

  SendRequest(":3000/item id=%s name=%s count:=%d price:=%d", "100", "test", 1, 100);

  const char* result = RecvSerial();
  Serial.print("RECV: ");
  Serial.println(result);
  free(result);
}

void loop() {
}
