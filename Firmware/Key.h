/*********************************************************************
* FIREKEY-PROJECT
* 
* Key handler
* 
* TODO
*********************************************************************/

class Key {
private:
  byte rowPin;                      // pin of row key belongs to 
  byte colPin;                      // pin of column key belongs to
  bool state;                       // TODO 
  byte ledIndex;                    // index of corresponding led
  byte downCounter;                 // how long is key pressed - used for hold and spam delays
  bool spamMode;                    // key is in spam mode or not
  byte lastColor[3];                // stores last rgb values of led

  Adafruit_NeoPixel *ledStrip;      // pointer to global led-strip object
  void (*keyPressedHandler)(Key *); // handler function called if key gets pressed

public:
  Key();
  Key(byte row, byte col, byte ledIndex, Adafruit_NeoPixel *ledStrip, void (*keyPressedHandler)(Key *));
  
  bool getState();
  // TODO state allways HIGH in handler???
  byte getIndex();
  void check();
  void setLedRGB(byte idx, byte red, byte green, byte blue);
  void setLedRGB(byte red, byte green, byte blue);
  void setLedRGB(String hexCode);
  void setLedOn();
  void setLedOff();
  void setLedDefault();
};


// ======== CONSTRUCTORS =========

/**
 * Empty constructor (required for array definition).
 */
Key::Key() { }

/**
 * Main constructor
 */
Key::Key(byte row, byte col, byte ledIndex, Adafruit_NeoPixel *ledStrip, void (*keyPressedHandler)(Key *)) {
  // save parameters
  this->rowPin = row;
  this->colPin = col;
  this->ledIndex = ledIndex;
  this->ledStrip = ledStrip;
  this->keyPressedHandler = keyPressedHandler;

  // initialize values
  this->state = false;
  this->spamMode = false;
  this->downCounter = 0;

  // setup matrix
  pinMode(colPin, INPUT_PULLUP);
  pinMode(rowPin, OUTPUT);
  digitalWrite(rowPin, HIGH);
}


// =========== GETTER ============

/**
 * Retruns the current state of the key (if it is pressed).
 */
bool Key::getState() {
  return this->state;
}

/**
 * Returns the led index of the key.
 */
byte Key::getIndex() {
  return this->ledIndex;
}


// ============= KEY =============

/**
 * Checks if key is pressed,handles press and updates the key state.
 */
void Key::check() {
  // pull output row to low
  digitalWrite(this->rowPin, LOW);

  // read key pressed
  this->state = (digitalRead(this->colPin) == LOW);

  // pull output row to high
  digitalWrite(this->rowPin, HIGH);

  // handle result
  // key pressed
  if (this->state) {
    // key was not pressed before
    if (this->downCounter == 0) {
      (*this->keyPressedHandler)(this);   // call handler function
    }
    // key helt
    else if (!this->spamMode && this->downCounter > HOLD_DELAY) {
      (*this->keyPressedHandler)(this);   // call handler function
      this->spamMode = true;  // enter spam mode
      this->downCounter = 0;  // reset counter
    }
    // key helt in spam mode
    else if (this->spamMode && this->downCounter > SPAM_DELAY) {
      (*this->keyPressedHandler)(this);   // call handler function
      this->downCounter = 0;  // reset counter
    } 
    this->downCounter++;

  // key not pressed, but was pressed before
  } else if (this->downCounter != 0) {
    this->spamMode = false;
    this->downCounter = 0;
  }
}


// ============= LED =============

/**
 * Sets the led color for a specific led index.
 */
void Key::setLedRGB(byte idx, byte red, byte green, byte blue) {
  // TODO problem with lastColor?
  
  (*this->ledStrip).setPixelColor(idx, red, green, blue);
  (*this->ledStrip).show();
}

/**
 * Sets the led color for this key.
 */
void Key::setLedRGB(byte red, byte green, byte blue) {
  lastColor[0] = red;
  lastColor[1] = green;
  lastColor[2] = blue;

  this->setLedRGB(this->ledIndex, red, green, blue);
}

/**
 * Sets the led color for this key using a hex code.
 */
void Key::setLedRGB(String hexCode) {
  int red, green, blue;
  hexToRGB(hexCode, red, green, blue);
  this->setLedRGB(red, green, blue);
}
// TODO necessary maybe done by configurator software?

/**
 * Turns LED on with last RGB value.
 */
void Key::setLedOn() {
  this->setLedRGB(lastColor[0], lastColor[1], lastColor[2]);
}

/**
 * Turns off LED.
 */
void Key::setLedOff() {
  (*this->ledStrip).setPixelColor(this->ledIndex, 0, 0, 0);
  (*this->ledStrip).show();
}

/**
 * Loads default rgb value and activates the led with it.
 */
void Key::setLedDefault() {
  this->setLedRGB(
    pgm_read_byte_near(&defaultLEDColors[currentLayer][this->ledIndex][0]),
    pgm_read_byte_near(&defaultLEDColors[currentLayer][this->ledIndex][1]),
    pgm_read_byte_near(&defaultLEDColors[currentLayer][this->ledIndex][2])
  );
}