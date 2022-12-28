/*********************************************************************
* FIREKEY - PROJECT
* 
* Key handler
* 
* Required libs:
*********************************************************************/

class Key {
private:
  byte rowPin;
  byte colPin;
  bool state;
  byte ledIndex;
  Adafruit_NeoPixel *ledStrip;

public:
  Key();
  Key(byte row, byte col, byte ledIndex, Adafruit_NeoPixel *ledStrip);
  bool getState();
  bool update();
  void setLEDRGB(byte idx, uint16_t red, uint16_t green, uint16_t blue);
  void setLEDRGB(uint16_t red, uint16_t green, uint16_t blue);
  void setLEDRGB(String hexCode);
  byte getIndex();
};

//TODO led on off default

/**
* Default constructor
*/
Key::Key() {
  this->state = false;
}

/**
* Main constructor
*/
Key::Key(byte row, byte col, byte ledIndex, Adafruit_NeoPixel *ledStrip) {
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

/**
* Retruns the current state of the key, if it is pressed
*/
bool Key::getState() {
  return this->state;
}

/**
* Checks if the key is pressed or not and updates the key state
*/
bool Key::update() {
  //pull output row to low
  digitalWrite(this->rowPin, LOW);

  this->state = (digitalRead(this->colPin) == LOW);

  // pull output row high again
  digitalWrite(this->rowPin, HIGH);

  return this->state;
}

/**
* Sets the led color for a specific index
*/
void Key::setLEDRGB(byte idx, uint16_t red, uint16_t green, uint16_t blue) {
  (*this->ledStrip).setPixelColor(idx, red, green, blue);
  (*this->ledStrip).show();
}

/**
* Sets the led color for the key
*/
void Key::setLEDRGB(uint16_t red, uint16_t green, uint16_t blue) {
  (*this->ledStrip).setPixelColor(this->ledIndex, red, green, blue);
  (*this->ledStrip).show();
}

/**
* Sets the led color for the key using a hex code
*/
void Key::setLEDRGB(String hexCode) {
  int red, green, blue;
  hexToRGB(hexCode, red, green, blue);
  (*this->ledStrip).setPixelColor(this->ledIndex, red, green, blue);
  (*this->ledStrip).show();
}

/**
* Returns the (led) index of the key
*/
byte Key::getIndex() {
  return this->ledIndex;
}