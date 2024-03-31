#include <Arduino.h>
#include "M5Unified.h"

// x1, y1, x2, y2, ...
uint8_t posA[] = { 26,  32,  41,  49, 106,  49, 124,  32, 119,  27,  31,  27};
uint8_t posB[] = { 22,  35,  16,  40,   9, 116,  20, 116,  34, 104,  39,  52};
uint8_t posC[] = {  9, 120,   2, 197,   7, 202,  26, 185,  31, 132,  19, 120};
uint8_t posD[] = { 10, 205,  15, 210, 103, 210, 109, 205,  93, 188,  28, 188};
uint8_t posE[] = {115, 120, 101, 132,  96, 185, 112, 202, 119, 196, 125, 120};
uint8_t posF[] = {108,  52, 104, 105, 115, 116, 126, 116, 133,  40, 127,  35};
uint8_t posG[] = { 36, 107,  23, 119,  34, 130,  98, 130, 111, 119, 100, 107};
uint8_t segValue[] = {0b0111111, 0b0110000, 0b1101101, 0b1111001, 0b1110010, 0b1011011, 0b1011111, 0b0110011, 0b1111111, 0b1111011, 0b1110111, 0b1011110, 0b0001111, 0b1111100, 0b1001111, 0b1000111};

#define COLOR GREEN
#define COLOR_OFF DARKGREEN

void drawSegmentPart(uint8_t *pos, uint8_t p1, uint8_t p2, uint8_t p3, uint16_t color)
{
  M5.Lcd.fillTriangle(pos[p1*2], pos[p1*2 + 1], pos[p2*2], pos[p2*2 + 1], pos[p3*2], pos[p3*2 + 1], color);
}

void drawSegment(uint8_t seg, uint8_t color)
{
  uint8_t *pos;
  switch (seg) {
    case 0:
      pos = posA;
      break;
    case 1:
      pos = posB;
      break;
    case 2:
      pos = posC;
      break;
    case 3:
      pos = posD;
      break;
    case 4:
      pos = posE;
      break;
    case 5:
      pos = posF;
      break;
    case 6:
      pos = posG;
      break;
  }
  if (color == 0){
/*
    for (uint8_t i = 0; i < 6; i++){
      uint8_t p1 = i * 2, p2 = (i * 2 + 2) % 12;      
      M5.Lcd.drawLine(pos[p1], pos[p1 + 1], pos[p2], pos[p2 + 1], COLOR);
    }
*/
    drawSegmentPart(pos, 0, 1, 2, COLOR_OFF);
    drawSegmentPart(pos, 2, 3, 4, COLOR_OFF);
    drawSegmentPart(pos, 4, 5, 0, COLOR_OFF);
    drawSegmentPart(pos, 0, 2, 4, COLOR_OFF);
  }
  else{
    drawSegmentPart(pos, 0, 1, 2, COLOR);
    drawSegmentPart(pos, 2, 3, 4, COLOR);
    drawSegmentPart(pos, 4, 5, 0, COLOR);
    drawSegmentPart(pos, 0, 2, 4, COLOR);
  }
}

void drawNumber(uint8_t value)
{
  if (value < 16){
    uint8_t seg = segValue[value];
    for (uint8_t i = 0; i < 7; i++){
      drawSegment(i, seg & (1 << i) ? 1 : 0);
    }
  }
}

void setup() {
  M5.begin();
  pinMode(36, INPUT);
  Serial1.begin(115200, SERIAL_8N1, 25, 26); // RX, TX
  M5.Lcd.fillScreen(BLACK);
  drawNumber(0);
}

uint8_t num = 0;

void loop() {
  M5.update();
  if (M5.BtnA.wasClicked())
  {
    M5.Lcd.fillScreen(BLACK);
    drawNumber(num);
    num++;
    if (num > 15)
      num = 0;
  }
  if (Serial1.available()){
    uint8_t c = Serial1.read();
    if (c >= 0x00 && c <= 0x0f){
      M5.Lcd.fillScreen(BLACK);
      drawNumber(c);
    }
    else
    if (c >= '0' && c <= '9'){
      M5.Lcd.fillScreen(BLACK);
      drawNumber(c - '0');
    }
    else if (c >= 'a' && c <= 'f'){
      M5.Lcd.fillScreen(BLACK);
      drawNumber(c - 'a' + 10);
    }
    else if (c >= 'A' && c <= 'F'){
      M5.Lcd.fillScreen(BLACK);
      drawNumber(c - 'A' + 10);
    }
  }
}
