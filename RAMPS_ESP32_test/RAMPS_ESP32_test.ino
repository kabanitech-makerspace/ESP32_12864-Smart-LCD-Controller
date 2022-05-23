// Kabani Tech #makerspace
// www.kabanitech.com
#include <Arduino.h>
#include <U8g2lib.h>
#include <ESP32Encoder.h>

//U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, EN , R/W, DS/RS, [RST]); //function will work without RST value initialisation; RST Default value is U8X8_PIN_NONE;
U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0,  27,  13, 14);

ESP32Encoder encoder;
#define ENCB 21
#define Buzz 22
#define Kill 23

int steps = 0;
int PresState, PrevState;
unsigned long lastBtnPress = 0;
int BtnState, line = 30;

void displayOnLCD(int posY, int RposX, int value);
void setup() {
  Serial.begin(112500);
  u8g2.begin();
  encoder.attachHalfQuad(19, 18);
  encoder.setCount(0);
  pinMode (ENCB, INPUT_PULLUP);
  pinMode (Kill, INPUT_PULLUP);
  pinMode (Buzz, OUTPUT);
  lastBtnPress = millis();
  PrevState = encoder.getCount();
}
void loop() {
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0, 15, "RAMPS Controller test");
  PresState = encoder.getCount();
  Serial.println(PresState);
  if (digitalRead(ENCB) == LOW) {
    if (millis() - lastBtnPress > 50) {
      digitalWrite (Buzz, HIGH);
      delay(75);
      digitalWrite (Buzz, LOW);
      displayOnLCD(100, 10, PresState);
    }
    lastBtnPress = millis();
  }
  else displayOnLCD(100, 0, PresState);
  u8g2.sendBuffer();
  if (digitalRead(Kill) == LOW) {
    if (millis() - lastBtnPress > 50) {
      digitalWrite (Buzz, HIGH);
      delay(100);
      digitalWrite (Buzz, LOW);
      encoder.setCount(0);
      line = 30;
      u8g2.clearBuffer();
    }
    lastBtnPress = millis();
  }
  delay(50);
}

void displayOnLCD(int posY, int RposX, int value) {
  if (value<0) posY -=10;
  u8g2_uint_t y = posY;
  u8g2_uint_t x = line + RposX;
  if(x>64) x=30;
  u8g2.drawStr(0, x , "Val : ");
  u8g2.setCursor(y, x);
  u8g2.print(value);
  line = x;
}
