// PINの位置を示す定数
const int BLUE = 13;
const int RED = 11;
const int GREEN = 9;
// 点灯/消灯のWAITを規定する定数ms
const int SPAN = 250;

// 最初に一度だけ実行される処理
void setup() {
  // デジタル9,11,13番ピンをOUTPUTに設定
  pinMode(BLUE, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
}

void loop() {
  // arduino fuzz-buzz!
  for (int i=1 ; i<=15 ; i++) {
    if (i % 15 == 0) {
      for (int j=0 ; j<5 ; j++) {
        digitalWrite(BLUE, HIGH);
        delay(50);
        digitalWrite(GREEN, HIGH);
        delay(50);
        digitalWrite(RED, HIGH);
        delay(50);
        digitalWrite(BLUE, LOW);
        delay(50);
        digitalWrite(GREEN, LOW);
        delay(50);
        digitalWrite(RED, LOW);
        delay(50);
      }
    } else if (i % 3 == 0) {
        digitalWrite(BLUE, HIGH);
        delay(SPAN);
        digitalWrite(BLUE, LOW);
        delay(SPAN);
    } else if (i % 5 == 0) {
        digitalWrite(GREEN, HIGH);
        delay(SPAN);
        digitalWrite(GREEN, LOW);
        delay(SPAN);
    } else {
        digitalWrite(RED, HIGH);
        delay(SPAN);
        digitalWrite(RED, LOW);
        delay(SPAN);
    }
  }
}
