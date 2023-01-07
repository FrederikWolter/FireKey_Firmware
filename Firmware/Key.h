/*********************************************************************
* FIREKEY-PROJECT
* 
* Key handler
* 
* TODO
*********************************************************************/

class Key {
private:
  byte rowPin;
  byte colPin;
  bool state;
  byte ledIndex;
  byte downCounter;                  // contains the counter how long the key got pressed. is reset after reaching SPAM_SPEED and after reaching HOLD_DELAY the key ist marked as spam mode.
  bool spamMode;                     // defines if the key is in spam mode or not
  Adafruit_NeoPixel *ledStrip;       // pointer to the global led-strip object
  void (*keyPressedHandler)(Key *);  // handler function which is called if the key gets pressed
  byte lastLEDColor[3];              // stores the last rgb values for the led

  void resetSpam();
  void setLastLEDColor();

public:
  Key();
  Key(byte row, byte col, byte ledIndex, Adafruit_NeoPixel *ledStrip, void (*keyPressedHandler)(Key *));
  bool getState();
  void checkPressed();
  void setLEDRGB(byte idx, byte red, byte green, byte blue);
  void setLEDRGB(byte red, byte green, byte blue);
  void setLEDRGB(String hexCode);
  byte getIndex();
  void ledOn();
  void ledOff();
  void ledDefault();
};

/**
* Default constructor (required for array def)
*/
Key::Key() {
  this->state = false;
  this->spamMode = false;
  this->downCounter = 0;
}

/**
* Main constructor
*/
Key::Key(byte row, byte col, byte ledIndex, Adafruit_NeoPixel *ledStrip, void (*keyPressedHandler)(Key *)) {
  this->rowPin = row;
  this->colPin = col;
  this->ledIndex = ledIndex;
  this->ledStrip = ledStrip;
  this->keyPressedHandler = keyPressedHandler;

  this->state = false;
  this->spamMode = false;
  this->downCounter = 0;

  //Setup matrix
  pinMode(colPin, INPUT_PULLUP);
  pinMode(rowPin, OUTPUT);
  digitalWrite(rowPin, HIGH);
}

/**
* Resets the key status when the key is released
* @see spamMode
* @see downCounter
*/
void Key::resetSpam() {
  this->spamMode = false;
  this->downCounter = 0;
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
    if (this->downCounter == 0) {
      // call the key pressed handler function
      (*this->keyPressedHandler)(this);
    } else if (this->spamMode && this->downCounter > SPAM_SPEED) {
      // call the key pressed handler function and reset the down counter
      (*this->keyPressedHandler)(this);
      this->downCounter = 0;
    } else if (this->downCounter > HOLD_DELAY) {
      this->spamMode = true;
    }
    if (this->downCounter < 255) {
      this->downCounter++;
    }

  } else if (this->downCounter != 0) {
    this->resetSpam();
  }
}

/**
* Sets the led color for a specific index
*/
void Key::setLEDRGB(byte idx, byte red, byte green, byte blue) {
  (*this->ledStrip).setPixelColor(idx, red, green, blue);
  (*this->ledStrip).show();
}

/**
* Sets the led color for the key
*/
void Key::setLEDRGB(byte red, byte green, byte blue) {
  lastLEDColor[0] = red;
  lastLEDColor[1] = green;
  lastLEDColor[2] = blue;
  this->setLEDRGB(this->ledIndex, red, green, blue);
}

/**
* Sets the led color for the key using a hex code
*/
void Key::setLEDRGB(String hexCode) {
  int red, green, blue;
  hexToRGB(hexCode, red, green, blue);
  this->setLEDRGB(red, green, blue);
}

/**
* Returns the (led) index of the key
*/
byte Key::getIndex() {
  return this->ledIndex;
}

/**
* Sets the last RGB value for the LED and activates it
*/
void Key::setLastLEDColor() {
  this->setLEDRGB(lastLEDColor[0], lastLEDColor[1], lastLEDColor[2]);
}

/**
* Sets the last RGB value for the LED and activates it
*/
void Key::ledOn() {
  this->setLastLEDColor();
}

/**
* Turns off the led
*/
void Key::ledOff() {
  (*this->ledStrip).setPixelColor(this->getIndex(), 0, 0, 0);
  (*this->ledStrip).show();
}

/**
* Loads the default rgb value and activates the led with it
*/
void Key::ledDefault() {
  lastLEDColor[0] = pgm_read_byte_near(&defaultLEDColors[currentLayer][this->ledIndex][0]);
  lastLEDColor[1] = pgm_read_byte_near(&defaultLEDColors[currentLayer][this->ledIndex][1]);
  lastLEDColor[2] = pgm_read_byte_near(&defaultLEDColors[currentLayer][this->ledIndex][2]);
  this->setLastLEDColor();
}