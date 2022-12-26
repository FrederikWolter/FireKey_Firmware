#include "00_Basics.h"

class Key {
private:
  byte rowPin;
  byte colPin;
  bool state;
  byte ledIndex;
  Adafruit_NeoPixel ledStrip;

public:
  Key();
  Key(byte row, byte col, byte ledIndex, Adafruit_NeoPixel ledStrip);
  bool getState();
  bool update();
  void setLEDRGB(uint16_t red, uint16_t green, uint16_t blue);
  void setLEDRGB(String hexCode);
  byte getIndex();
};

Key::Key() {
  this->state = false;
}

Key::Key(byte row, byte col, byte ledIndex, Adafruit_NeoPixel ledStrip) {
  this->rowPin = row;
  this->colPin = col;
  this->ledIndex = ledIndex;
  this->state = false;
  this->ledStrip = ledStrip;

  //Setup matrix (TODO Move out of key class?)
  pinMode(colPin, INPUT_PULLUP);
  pinMode(rowPin, OUTPUT);
  digitalWrite(rowPin, HIGH);
}

bool Key::getState() {
  return this->state;
}

bool Key::update() {
  //pull output row to low
  digitalWrite(this->rowPin, LOW);

  this->state = (digitalRead(this->colPin) == LOW);

  // pull output row high again
  digitalWrite(this->rowPin, HIGH);

  return this->state;
}

void Key::setLEDRGB(uint16_t red, uint16_t green, uint16_t blue) {
  this->ledStrip.setPixelColor(this->ledIndex, red, green, blue);
}

void Key::setLEDRGB(String hexCode) {
  int red, green, blue;
  hexToRGB(hexCode, red, green, blue);
  this->ledStrip.setPixelColor(this->ledIndex, red, green, blue);
}

byte Key::getIndex(){
  return this->ledIndex;
}