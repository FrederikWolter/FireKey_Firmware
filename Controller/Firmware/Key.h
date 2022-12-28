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
  byte keyDownCounter;  // contains the counter how long the key got pressed. is reset after reaching SPAM_SPEED and after reaching HOLD_DELAY the key ist marked as spam mode.
  bool keySpamMode;     // defines if the key is in spam mode or not
  Adafruit_NeoPixel *ledStrip;
  void (*keyPressedHandler)(Key*);

  void resetSpam();

public:
  Key();
  Key(byte row, byte col, byte ledIndex, Adafruit_NeoPixel *ledStrip, void (*keyPressedHandler)(Key*));
  bool getState();
  void checkPressed();
  void setLEDRGB(byte idx, uint16_t red, uint16_t green, uint16_t blue);
  void setLEDRGB(uint16_t red, uint16_t green, uint16_t blue);
  void setLEDRGB(String hexCode);
  byte getIndex();
};

//TODO led on off default

/**
* Default constructor (required for array def)
*/
Key::Key() {
  this->state = false;
  this->keySpamMode = false;
  this->keyDownCounter = 0;
}

/**
* Main constructor
*/
Key::Key(byte row, byte col, byte ledIndex, Adafruit_NeoPixel *ledStrip, void (*keyPressedHandler)(Key*)) {
  this->rowPin = row;
  this->colPin = col;
  this->ledIndex = ledIndex;
  this->ledStrip = ledStrip;
  this->keyPressedHandler = keyPressedHandler;

  this->state = false;
  this->keySpamMode = false;
  this->keyDownCounter = 0;

  //Setup matrix
  pinMode(colPin, INPUT_PULLUP);
  pinMode(rowPin, OUTPUT);
  digitalWrite(rowPin, HIGH);
}

/**
* Resets the key status when the key is released
* @see keySpamMode
* @see keyDownCounter
*/
void Key::resetSpam() {
  this->keySpamMode = false;
  this->keyDownCounter = 0;
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
void Key::checkPressed() {
  //pull output row to low
  digitalWrite(this->rowPin, LOW);
  this->state = (digitalRead(this->colPin) == LOW);

  // pull output row high again
  digitalWrite(this->rowPin, HIGH);

  if (this->state) {
    if (this->keyDownCounter == 0) {
      // call the key pressed handler function
      (*this->keyPressedHandler)(this);
    } else if (this->keySpamMode && this->keyDownCounter > SPAM_SPEED) {
      // call the key pressed handler function and reset the down counter
      (*this->keyPressedHandler)(this);
      this->keyDownCounter = 0;
    } else if (this->keyDownCounter > HOLD_DELAY) {
      this->keySpamMode = true;
    }
    if (this->keyDownCounter < 255) {
      this->keyDownCounter++;
      Serial.println(this->keyDownCounter);
    }

  } else if (this->keyDownCounter != 0) {
    this->resetSpam();
  }
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