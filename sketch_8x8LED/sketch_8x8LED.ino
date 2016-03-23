// OSL641505-BRA ピン配置 サイトのドキュメントから
// 何でこんなバラバラなんだぜ？？

// A-K対応表 便宜上よこむきにする
// A 09 -- 08 A
// K 10 -- 07 A
// K 11 -- 06 K
// A 12 -- 05 A
// K 13 -- 04 K
// A 14 -- 03 K
// K 15 -- 02 A
// K 16 -- 01 A 

// 初期設定
void setup() {
  pinMode( 0, OUTPUT);
  pinMode( 1, OUTPUT);
  pinMode( 2, OUTPUT);
  pinMode( 3, OUTPUT);
  pinMode( 4, OUTPUT);
  pinMode( 5, OUTPUT);
  pinMode( 6, OUTPUT);
  pinMode( 7, OUTPUT);
  pinMode( 8, OUTPUT);
  pinMode( 9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
}

// 8x8のマトリクスとPINの対応
//       C1 C2 C3 C4 C5 C6 C7 C8   
//       13 03 04 10 06 11 15 16
// R1 09 -^ -^ -^ -^ -^ -^ -^ -^
// R2 14 -^  
// R3 08 -^  
// R4 12 -^  
// R5 01 -^  
// R6 07 -^  
// R7 02 -^  
// R8 05 -^  

// LED点灯の時間制御
// 1LEDの点灯時間 msec
const int ONE_LED_ON_SPAN = 4;
// 全部点灯させた後、またリフレッシ開始までのWAIT
const int WORKOUT_WAIT = 10;

// ArduinoのPINが、LEDのPINのどれに該当するか
// このスケッチではArd0=LED1、で順に(Analog含めて)対応させる前提なので、単に-1した値を返す
int getArduinoPinFromLedPin(int ledPin) {
  return ledPin - 1;
}

int getLedAnodePinFromRow(int row) {
  switch (row) {
    case 1:
      return 9;
    case 2:
      return 14;
    case 3:
      return 8;
    case 4:
      return 12;
    case 5:
      return 1;
    case 6:
      return 7;
    case 7:
      return 2;
    default:
      return 5;
  }
}

int getLedCathodePinFromCol(int col) {
  switch (col) {
    case 1:
      return 13;
    case 2:
      return 3;
    case 3:
      return 4;
    case 4:
      return 10;
    case 5:
      return 6;
    case 6:
      return 11;
    case 7:
      return 15;
    default:
      return 16;
  }
}

void setup()
{
  // Arduino0～15PINを「お前はデジタルOUTだ」と任命する
  for( int ardPin = 0; ardPin < 16; ardPin++ )
  {
    pinMode(ardPin, OUTPUT);
  }
  // Arduino0～7PINがAnoになるので、初期化時点では全員LOWでおとなしくさせる
  for( int ardPin = 0; ardPin < 8; ardPin++ )
  {
    digitalWrite(ardPin, LOW);
  }
  // Arduino8～15PINがCatになるので、初期化時点では全員HIGH
  for( int ardPin = 0; ardPin < 16; ardPin++ )
  {
    pinMode(ardPin, OUTPUT);
    digitalWrite(ardPin, LOW);
  }
}

void loop()
{
  // サンプルね。
  int targetRow = 5;
  int targetCol = 2;

  int targetLedAnoPin = getLedAnodePinFromRow(targetRow);
  int targetArdAnoPin = getArduinoPinFromLedPin(targetLedAnoPin);

  int targetLedCatPin = getLedCathodePinFromCol(targetCol);
  int targetArdCatPin = getArduinoPinFromLedPin(targetLedCatPin);

  turnOn(targetArdAnoPin, targetArdCatPin);
  waitOneLed();
  turnOff(targetArdAnoPin, targetArdCatPin);

}

void turnOn(


