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

//---- 8x8のマトリクスとPINの対応 -----------------------------------------------
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

//---- 文字データパターン -------------------------------------------------------

static byte charA[] = {
  B00010000,  B00101000,  B00101000,  B00101000,  B01111100,  B01000100,  B10000010,  B10000010
};

static byte charT[] = {
  B11111110,  
  B00010000, 
  B00010000, 
  B00010000, 
  B00010000, 
  B00010000, 
  B00010000, 
  B00010000
};

static byte charO[] = {
  B00111000, 
  B01000100,  
  B10000010, 
  B10000010, 
  B10000010, 
  B10000010, 
  B01000100, 
  B00111000
};

static byte charD[] = {
  B11110000, 
  B10001100,  
  B10000010, 
  B10000010, 
  B10000010, 
  B10000010, 
  B10001100, 
  B11110000
};

static byte charR[] = {
  B11111000, 
  B10000100,  
  B10000010, 
  B10000100, 
  B11111000, 
  B10000100, 
  B10000010, 
  B10000001
};
static byte charK[] = {
  B10000010, 
  B10001100,  
  B10110000, 
  B11000000, 
  B11000000, 
  B10110000, 
  B10001100, 
  B10000010
};

static byte charI[] = {
  B00100000, 
  B00100000, 
  B00100000, 
  B00100000, 
  B00100000, 
  B00100000, 
  B00100000, 
  B00100000
};


//---- LEDのPIN使用範囲定数 -----------------------------------------------------

// ROW
const byte ROW_MIN = 1;
const byte ROW_MAX = 8;

// COL
const byte COL_MIN = 1;
const byte COL_MAX = 8;

//---- LED点灯の時間制御 --------------------------------------------------------

// 1列のLEDの点灯時間 msec
const int ROW_ENERGIZATION_MSEC = 2;

// ある状態のディスプレイパターンを表示する回数
const int REFRESH_MAX = 7;

//---- ハードウェア連携設定 -----------------------------------------------------
// ★ あんまり難しく考える必要なかった

// 配列要素 0=ROW1 ～ 7=ROW8 に対応するLEDPin
const byte anodeLedPinList[] = { 9, 14, 8, 12, 1, 7, 2, 5 };

// 配列要素 0=COL1 ～ 7=COL8 に対応するLEDPin
const byte cathodeLedPinList[] = { 13, 3, 4, 10, 6, 11, 15, 16 };

//---- ひょうじもじれつ ----------------------------------------------------------
byte* outputText[] = { charT, charO, charD, charO, charR, charO, charK, charI };
byte outputTextLength = 8;

//---- 主処理 --------------------------------------------------------------------
void loop()
{
  // -- 表示制御変数群 --

  // ディスプレイパターンを表示しつづけるための変数
  static int refreshCounter = REFRESH_MAX;
  
  // ディスプレイパターン(今ビットマトリクスLEDに表示するパターン)
  static byte dispPattern[8];
  // 取り込み中のパターン 8bit全部移したら次の文字を持ってくる
  static byte shiftingPattern[8];
  // 取り込み中のパターンをあと何bit取り込むか 7から0に変わっていくが初回起動時は0
  static byte shiftCounter = 0;
  
  // 現在フォーカスしている、ひょうじもじれつのindex値 初回起動時は-1
  static byte focusIndex = -1;
  
  // -- 主処理 --

  // フォーカス移動要否判定
  if (refreshCounter == REFRESH_MAX && shiftCounter == 0) {
    byte nextIndex;
    if (outputTextLength == 1) {
      focusIndex = 0;
      nextIndex = 0;
    } else if (focusIndex == outputTextLength - 2) {
      focusIndex++;
      nextIndex = 0;
    } else if (focusIndex == outputTextLength - 1) {
      focusIndex = 0;
      nextIndex = 1;
    } else {
      focusIndex++;    
      nextIndex = focusIndex + 1;
    }
    // 文字データを転記
    cloneTextData(outputText[focusIndex], dispPattern);
    cloneTextData(outputText[nextIndex], shiftingPattern);
    // カウンタを初期状態に戻す
    shiftCounter = 7;
    refreshCounter = 0;
  }

  // ビットシフト要否判定実施
  if (refreshCounter == REFRESH_MAX) {
    for (byte i=0 ; i<8 ; i++) {
      dispPattern[i] = dispPattern[i] << 1;
      if (shiftingPattern[i] > B01111111) {
        dispPattern[i]++;       
      }
      shiftingPattern[i] = shiftingPattern[i] << 1;
    }
    // シフトカウンタをデクリメント、リフレッシュカウンタは初期化
    //   シフトさせるのは7bit分でよい。7bitシフトした状態を表示しきったら、次はフォーカス移動
    shiftCounter--;
    refreshCounter = 0;
  }

  // 現在のディスプレイパターン内容を表示
  for (int row=ROW_MIN ; row <=ROW_MAX ; row++) {
    turnSpecifiedColsAsBitPattern(row, dispPattern[row -1]);
    delay(ROW_ENERGIZATION_MSEC);
  }

  // リフレッシュカウンタ
  refreshCounter++;
}

//---- 初期処理 ------------------------------------------------------------------
void setup() {
  // Arduino0～15PINを「お前はデジタルOUTだ」と任命する
  for (byte i=0 ; i<=13 ; i++) {
    pinMode(i, OUTPUT);
  }
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);

  for (byte row=ROW_MIN; row<=ROW_MAX ; row++) {
    turnOffSpecifiedRow(row);
  }
  for (byte col=COL_MIN; col<=COL_MAX ; col++) {
    turnOffSpecifiedCol(col);
  }
}

//---- ハードウェア連携処理 -----------------------------------------------------
// 実装については、row側がAnodeであることの影響を受けている。

byte getAnodeLedPinFromRow(byte row) {
  return anodeLedPinList[row -1];
}

byte getCathodeLedPinFromCol(byte col) {
  return cathodeLedPinList[col -1];
}

// ArduinoのPinとLEDPinの対応 難しくせずD0から順に対応させる
byte getArduinoPinFromLedPin(byte ledPin) {
  // ArduinoPIN'0' = LEDPin'1'
  return ledPin - 1;
}

//---- 点灯制御 ------------------------------------------------------------------

// 指定されたrow/colのLEDをひとつ点灯させる。
void turnOnSpecifiedLED(byte row, byte col) {
  turnOnOnlySpecifiedCol(col);
  turnOnOnlySpecifiedRow(row);
}

// 指定されたrowについて、指定colを点灯/それ以外のcolを消灯させる。
void turnSpecifiedColsOn(byte row, byte cols[], byte colLength) {
  // Onlyを使うと最後のひとつ以外消えてしまうのでOnlyは使わない。
  // 簡単に作る。一旦全消灯して指定colsのみ点灯。
  for (byte settingCol=COL_MIN ; settingCol<=COL_MAX ; settingCol++) {
    turnOffSpecifiedCol(settingCol);
  }
  for (byte i=0 ; i<colLength ; i++) {
    turnOnSpecifiedCol(cols[i]);
  }
  turnOnOnlySpecifiedRow(row);
}

// 指定されたrowについて、指定colを点灯/それ以外のcolを消灯させる（bit判定）。
//   このメソッドは1row8bit前提とする。
void turnSpecifiedColsAsBitPattern(byte row, byte bitPattern) {
  // bitPatternをコピーしてシフトしながら点灯消灯を制御する。
  byte bitPatternWork = bitPattern;
  for (byte col=COL_MIN ; col<=COL_MAX ; col++) {
    if (bitPatternWork > B01111111) {
      turnOnSpecifiedCol(col);
    } else {
      turnOffSpecifiedCol(col);
    }
    bitPatternWork = bitPatternWork << 1;
  }
  turnOnOnlySpecifiedRow(row);
}

//---- 点灯制御部品 --------------------------------------------------------------

// 指定されたcolを点灯準備し、それ以外を消灯させる = Cathode側をHIGHに変える。
void turnOnOnlySpecifiedCol(byte col) {
  // 対象colの前まで消灯
  for(byte settingCol=COL_MIN ; settingCol < col ; settingCol++) {
    turnOffSpecifiedCol(settingCol);
  }
  // 対象col点灯
  turnOnSpecifiedCol(col);
  // 対象colの後を消灯
  for(int settingCol=col+1 ; settingCol <= COL_MAX ; settingCol++) {
    turnOffSpecifiedCol(settingCol);
  }
}

// 指定されたcolを点灯準備する。
void turnOnSpecifiedCol(byte col) {
  digitalWrite(getArduinoPinFromLedPin(getCathodeLedPinFromCol(col)), LOW);
}

// 指定されたcolを消灯させる = Cahtode側をHIGHに変える。
void turnOffSpecifiedCol(byte col) {
  digitalWrite(getArduinoPinFromLedPin(getCathodeLedPinFromCol(col)), HIGH);
}

// 指定されたrowを点灯準備し、それ以外を消灯させる = Anode側をLOWに変える。
void turnOnOnlySpecifiedRow(byte row) {
  // 対象rowの前まで消灯
  for(byte settingRow=ROW_MIN ; settingRow < row ; settingRow++) {
    turnOffSpecifiedRow(settingRow);
  }
  // 対象row点灯
  turnOnSpecifiedRow(row);
  // 対象rowの後を消灯
  for(byte settingRow=row+1 ; settingRow <= ROW_MAX ; settingRow++) {
    turnOffSpecifiedRow(settingRow);
  }
}

// 指定されたrowを点灯準備する。
void turnOnSpecifiedRow(byte row) {
  digitalWrite(getArduinoPinFromLedPin(getAnodeLedPinFromRow(row)), HIGH);
}

// 指定されたrowを消灯させる = Anode側をLOWに変える。
void turnOffSpecifiedRow(byte row) {
  digitalWrite(getArduinoPinFromLedPin(getAnodeLedPinFromRow(row)), LOW);
}

//---- テキストデータ制御 ------------------------------------------------------------

// 1文字分のテキストデータ(8bit*8=8byte)を、指定された領域に転記する
//  from,toともにbyte[8]相当の領域であること 
void cloneTextData(byte* from, byte* to) {
  for (int i=0 ; i<8 ; i++) {
    to[i] = from[i];
  }
}

