/*********************************************************************
* FIREKEY - PROJECT
* 
* Firmware Version 0.5.1V
* 
* Required libs:
* - Keyboard(https://github.com/arduino-libraries/Keyboard)
*   -> can be installed via arduino ide (defauled installed)
* - USBHost
*   -> can be installed via arduino ide
* - Adafruit NeoPixel (https://github.com/adafruit/Adafruit_NeoPixel) 
*    -> can be installed via Arduino IDE
* - U8g2 Lib(https://github.com/olikraus/u8g2)
*    -> can be installed via Arduino IDE
* - avr/pgmspace lib
*    -> is installed (arduino pro micro)
*********************************************************************/

//TODO move const to config file
//TODO move led & keypress to key
//TODO IFDEF DEBUG for serial print (debug.h)
//TODO DEBUG_PRINTLN(F("xxx"));
//TODO header split

// LIBRARIES
#include "Config.h"

unsigned long debounceTime;    // debounce last check
unsigned long lastKeyPressed;  // last time a key got pressed (for the timeout functionality)

//TODO Move to key class!
byte keyDownCounter[COL_COUNT * ROW_COUNT];  // contains the counter how long key got pressed. Is reset after reaching SPAM_SPEED and after reaching HOLD_DELAY the key ist marked as spam mode.
bool keySpamMode[COL_COUNT * ROW_COUNT];     // defines if a key is in spam mode or not

bool sleeping;  // check if the display & the led strip is sleeping

// LED strip object
Adafruit_NeoPixel ledStrip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Key objects
Key keys[ROW_COUNT][COL_COUNT];

// OLED object
U8G2_SH1106_128X64_NONAME_1_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);

void setup() {
  DEBUG_BEGIN();

  // initialize the display
  display.setFont(u8g2_font_6x10_tr);
  //display.setI2CAddress(OLED_ADDR);
  display.begin();

  DEBUG_PRINTLN(F("Display initialized"));

  // initialize LED strip
  ledStrip.begin();
  // set the led brightness
  ledStrip.setBrightness(LED_BRIGHT);
  ledStrip.show();
  DEBUG_PRINTLN(F("LED-Strip initialized"));

  // Setup key matrix
  for (byte r = 0; r < ROW_COUNT; r++) {
    for (byte c = 0; c < COL_COUNT; c++) {
      keys[r][c] = Key(rows[r], cols[c], getLedIndex(r, c), &ledStrip);
    }
  }
  DEBUG_PRINTLN(F("Matrix initialized"));

  // initialize keyboard
  Keyboard.begin(KeyboardLayout_de_DE);
  DEBUG_PRINTLN(F("Keyboard initialized"));

  // setup default values
  lastKeyPressed = millis();
  sleeping = false;
  refreshDisplay();
  setLayerRGB();
}

void loop() {
  //DEBUG_PRINTLN(freeMemory());

  // debounced check of the key matrix
  if ((millis() - debounceTime) > DEBOUNCE_TIME) {
    readMatrix();
    debounceTime = millis();
  }

  if ((millis() - lastKeyPressed) > ((long)SLEEP_DELAY_SECONDS * 1000)) {
    sleepComponents();
  }
}

/**
* Sets the components to the sleeping state if @see sleeping is false
*/
void sleepComponents() {
  if (!sleeping) {
    sleeping = true;
    sleepDisplay();
    sleepLEDStrip();
  }
}

/**
* Wakes the components when @link sleeping is true
*/
void wakeComponents() {
  if (sleeping) {
    sleeping = false;
    wakeDisplay();
    wakeLEDStrip();
  }
}

/**
* Updates the diplay texts using @link setDisplayText, if @link sleeping is false
*/
void refreshDisplay() {
  if (!sleeping) {
    display.firstPage();
    do {
      setDisplayText();
    } while (display.nextPage());
  }
}

/**
* Forces the @link display into the sleeping state
*/
void sleepDisplay() {
  DEBUG_PRINTLN(F("Display is going to sleep..."));
  display.sleepOn();
}

/**
* Wakes the @link display from the sleeping state
*/
void wakeDisplay() {
  DEBUG_PRINTLN(F("Waking up display..."));
  display.sleepOff();
  refreshDisplay();
}

/**
* Print a string at a specific position on the display taking into account the text height and width
*/
void drawStringAtPosition(const char *buf, byte xPosition, byte yPosition) {
  int h = display.getFontAscent() - display.getFontDescent();
  int w = display.getStrWidth(buf);

  if (xPosition == LEFT) {
    display.setCursor(LEFT, yPosition + h);
  } else if (xPosition == CENTER) {
    int xVal = CENTER - w / 2;
    display.setCursor(xVal, yPosition + h);
  } else if (xPosition == RIGHT) {
    int xVal = RIGHT - w;
    display.setCursor(xVal, yPosition + h);
  }
  display.print(buf);
}

/**
* Writes the texts of the corresponding layer on the display
* @see drawStringAtPosition
* @see refreshDisplay
* @see currentLayer
* @see layerNames
* @see layerButtonFunc
*/
void setDisplayText() {
  char layerBuf[LAYER_NAME_LENGTH + 1];
  progMemStrCpy(layerNames[currentLayer], layerBuf);
  drawStringAtPosition(layerBuf, CENTER, ROW0);

  display.drawLine(LEFT, HLINE1, RIGHT, HLINE1);
  display.drawLine(VLINE1, TOP, VLINE1, BOTTOM);
  display.drawLine(VLINE2, TOP, VLINE2, BOTTOM);

  char actionBuf[MAX_KEY_LENGTH + 1];

  progMemStrCpy(layerButtonFunc[currentLayer][0], actionBuf);
  drawStringAtPosition(actionBuf, LEFT, ROW1);
  progMemStrCpy(layerButtonFunc[currentLayer][1], actionBuf);
  drawStringAtPosition(actionBuf, CENTER, ROW1);
  progMemStrCpy(layerButtonFunc[currentLayer][2], actionBuf);
  drawStringAtPosition(actionBuf, RIGHT, ROW1);

  progMemStrCpy(layerButtonFunc[currentLayer][3], actionBuf);
  drawStringAtPosition(actionBuf, LEFT, ROW2);
  progMemStrCpy(layerButtonFunc[currentLayer][4], actionBuf);
  drawStringAtPosition(actionBuf, CENTER, ROW2);
  progMemStrCpy(layerButtonFunc[currentLayer][5], actionBuf);
  drawStringAtPosition(actionBuf, RIGHT, ROW2);

  progMemStrCpy(layerButtonFunc[currentLayer][6], actionBuf);
  drawStringAtPosition(actionBuf, LEFT, ROW3);
  progMemStrCpy(layerButtonFunc[currentLayer][7], actionBuf);
  drawStringAtPosition(actionBuf, CENTER, ROW3);
  progMemStrCpy(layerButtonFunc[currentLayer][8], actionBuf);
  drawStringAtPosition(actionBuf, RIGHT, ROW3);

  progMemStrCpy(layerButtonFunc[currentLayer][9], actionBuf);
  drawStringAtPosition(actionBuf, LEFT, ROW4);
  progMemStrCpy(layerButtonFunc[currentLayer][10], actionBuf);
  drawStringAtPosition(actionBuf, CENTER, ROW4);
  progMemStrCpy(layerButtonFunc[currentLayer][11], actionBuf);
  drawStringAtPosition(actionBuf, RIGHT, ROW4);
}

/**
* Sets the @link ledStrip leds to the current layer colors.
* @see currentLayer
*/
void setLayerRGB() {
  for (byte k = 0; k < NUM_LEDS; k++) {
    ledStrip.setPixelColor(k, pgm_read_byte_near(&layerRGB[currentLayer][k][0]), pgm_read_byte_near(&layerRGB[currentLayer][k][1]), pgm_read_byte_near(&layerRGB[currentLayer][k][2]));
  }
  ledStrip.show();
}

/**
* Forces the @link ledStrip into sleeping mode
*/
void sleepLEDStrip() {
  DEBUG_PRINTLN("Leds are going to sleep...");
  ledStrip.clear();
  ledStrip.show();
}

/**
* Wakes the @link ledStrip from the sleeping state
*/
void wakeLEDStrip() {
  DEBUG_PRINTLN("Waking up leds...");
  setLayerRGB();
}

/**
* Get the led index of the current key row and col
* @return the led index as byte
*/
byte getLedIndex(byte rowIdx, byte colIdx) {
  // calculate led index out of row and col index
  byte index = rowIdx * COL_COUNT;
  index += rowIdx % 2 == 0 ? colIdx : COL_COUNT - 1 - colIdx;
  return index;
}

/**
* Scans the key matrix, if a key got pressed.
* @see keys
* @see keyPressed
* @see resetKey
*/
void readMatrix() {
  // scan matrix
  for (int rowIndex = 0; rowIndex < ROW_COUNT; rowIndex++) {

    // is any column pulled to low due to a button being pressed?
    for (int colIndex = 0; colIndex < COL_COUNT; colIndex++) {
      // Get key object
      Key key = keys[rowIndex][colIndex];

      // Check if the key is pressed
      if (key.update()) {  //TODO Rework
        keyPressed(key);
      } else if (keyDownCounter[key.getIndex()] != 0) {
        resetKey(key.getIndex());
      }
    }
  }
}

/**
* Resets the key status when a key is released
* @see keySpamMode
* @see keyDownCounter
*/
void resetKey(byte keyIndex) {
  keySpamMode[keyIndex] = false;
  keyDownCounter[keyIndex] = 0;
}

/**
* Is called after a key got pressed.
* Handles the spam mode.
* @see keySpamMode
* @see keyDownCounter
*/
void keyPressed(Key key) {
  lastKeyPressed = millis();
  wakeComponents();
  //TODO Move to key class?
  if (keyDownCounter[key.getIndex()] == 0) {
    handleKeyPress(key);
  } else if (keySpamMode[key.getIndex()] && keyDownCounter[key.getIndex()] > SPAM_SPEED) {
    handleKeyPress(key);
    keyDownCounter[key.getIndex()] = 0;
  } else if (keyDownCounter[key.getIndex()] > HOLD_DELAY) {
    keySpamMode[key.getIndex()] = true;
  }
  if (keyDownCounter[key.getIndex()] < 255)
    keyDownCounter[key.getIndex()]++;
}

/**
* Calls the corresponding function of the key
* @see handleLayerKeyPress
* @see keyOnePressed
* @see keyTwoPressed
* @see keyThreePressed
* @see keyFourPressed
* @see keyFivePressed
* @see keySixPressed
* @see keySevenPressed
* @see keyEightPressed
* @see keyNinePressed
* @see keyTenPressed
* @see keyElevenPressed
* @see keyTwelvePressed
*/
void handleKeyPress(Key key) {  //pass this fnct to key
  switch (key.getIndex()) {
    case LAYER_BACK_KEY:
    case LAYER_HOME_KEY:
    case LAYER_FORWARD_KEY:
      handleLayerKeyPress(key);
      break;
    case KEY_1:
      keyOnePressed(key);
      break;
    case KEY_2:
      keyTwoPressed(key);
      break;
    case KEY_3:
      keyThreePressed(key);
      break;
    case KEY_4:
      keyFourPressed(key);
      break;
    case KEY_5:
      keyFivePressed(key);
      break;
    case KEY_6:
      keySixPressed(key);
      break;
    case KEY_7:
      keySevenPressed(key);
      break;
    case KEY_8:
      keyEightPressed(key);
      break;
    case KEY_9:
      keyNinePressed(key);
      break;
    case KEY_10:
      keyTenPressed(key);
      break;
    case KEY_11:
      keyElevenPressed(key);
      break;
    case KEY_12:
      keyTwelvePressed(key);
      break;
    default:
      break;
  }
}

/**
* Handles the layer key functionalities and refreshed the @link display texts and @link ledStrip colors. (Layer: up, down, home)
*/
void handleLayerKeyPress(Key key) {
  switch (key.getIndex()) {
    case LAYER_BACK_KEY:
      currentLayer = (currentLayer - 1 + MAX_LAYER) % MAX_LAYER;
      break;
    case LAYER_FORWARD_KEY:
      currentLayer = (currentLayer + 1) % MAX_LAYER;
      break;
    case LAYER_HOME_KEY:
    default:
      currentLayer = HOME_LAYER;
  }
  refreshDisplay();
  setLayerRGB();
}
