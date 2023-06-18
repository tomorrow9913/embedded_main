void SendSerial(const char* str) {
  for (char* ch = str; *ch != '\0'; ++ch) {
    Serial.write(*ch);
  }
  Serial.write('\n');
}

char* RecvSerial() {
  int index = 0;
  char* buf = (char*)malloc(sizeof(char) * 1024);
  buf[0] = ' ';

  while (buf[index-1] != 10) {
    if (Serial.available()) {
      buf[index++] = Serial.read();
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
  Serial1.begin(9600);


  SendRequest("get :3000/item");
  const char* result = RecvSerial();
  Serial.print("RECV: ");
  Serial.println(result);
  InitData(result);
  free(result);
}

void loop() {
  if (Serial1.available()) {
    Serial.write(Serial1.read());
  }

  if (Serial.available()) {
    Serial1.write(Serial.read());
  }
}