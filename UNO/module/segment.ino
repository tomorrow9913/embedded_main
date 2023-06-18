byte patterns[] = {
    0xFC, 0x60, 0xda, 0xf2, 0x66, 0xb6,0xbe,0xe4,0xfe,0xe6
};

//자릿수
int digitSelectPin[] = {66,67,68,69};
// 세그먼트
int segment_pin[] = {58, 59, 60, 61,62,63,64,65};

void setup(){
    for(int i = 0; i < 4; i++) {
        pinMode(digitSelectPin[i], OUTPUT);
    }

    for(int i = 0; i < 8; i++) {
        pinMode(segment_pin[i], OUTPUT);
    }
}

void ShowDigit(int digit) {
    for(int i = 3; i >= 0; i++) {
      ShowDigit(i, digit % 10);
      digit /= 10;
      delay(200);  
    }
}

void ShowDigit(int position, int digit) {
    for (int i = 0; i < 4; i++)
    {
        if (i == position)
            digitalWrite(digitSelectPin[i], LOW);
        else
            digitalWrite(digitSelectPin[i], HIGH);
    }

    for (int i = 0; i < 8; i++)
    {
        boolean on_off = bitRead(patterns[digit], 7-i);
        digitalWrite(segment_pin[i], on_off);
    }   
}

void loop(){
    ShowDigit(1234);
}