#include "00_Basics.h"

class Key {
private:
  byte rowPin;
  byte colPin;
  bool state;
  byte ledIndex;
  Adafruit_NeoPixel ledStrip;

public:
  Key(byte row, byte col, byte ledIndex, Adafruit_NeoPixel ledStrip);
  bool getState();
  void setState(bool state);
  void setLEDRGB(uint16_t red, uint16_t green, uint16_t blue);
  void setLEDRGB(String hexCode);
};

Key::Key(byte row, byte col, byte ledIndex, Adafruit_NeoPixel ledStrip) {
  this->rowPin = row;
  this->colPin = col;
  this->ledIndex = ledIndex;
  this->state = false;
  this->ledStrip = ledStrip;
}

bool Key::getState() {
  return this->state;
}

void Key::setState(bool state) {
  this->state = state;
}

void Key::setLEDRGB(uint16_t red, uint16_t green, uint16_t blue) {
  this->ledStrip.setPixelColor(this->ledIndex, red, green, blue);
}

void Key::setLEDRGB(String hexCode) {
	int red, green, blue;
	hexToRGB(hexCode, red, green, blue);
	this->ledStrip.setPixelColor(this->ledIndex, red, green, blue);
}
