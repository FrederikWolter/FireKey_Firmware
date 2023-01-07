/*************************************************************************
* FIREKEY-PROJECT
* Firmware Version 1.0.0
* 
* Required libs:
* - Keyboard          (https://github.com/arduino-libraries/Keyboard) 
* - USBHost           (https://github.com/arduino-libraries/USBHost)
* - Adafruit NeoPixel (https://github.com/adafruit/Adafruit_NeoPixel)
* - U8g2              (https://github.com/olikraus/u8g2)
* - avr/pgmspace      (part of Pro Micro Board - preinstalled)
* -> can all be installed via Arduino IDE library manager
*************************************************************************/

// TODO more DEBUG_PRINTLN(F("xxx"));
// TODO header split (.h & .cpp)
// TODO change USB Device information? (Nice to have)
// TODO Disable Onboard LEDs
// TODO fading LEDs?


// ===== LIBRARIES & CONFIG ======
#include "Config.h"


// ========== VARIABLES ==========
unsigned long lastRefresh;      // last checked matrix (for debouncing)
unsigned long lastKeyPress;     // last time a key pressed (for timeout functionality)
bool sleeping;                  // is display & led strip sleeping?

// LED strip object
Adafruit_NeoPixel ledStrip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// Key objects
Key keys[ROW_COUNT][COL_COUNT];

// OLED objects
U8G2_SH1106_128X64_NONAME_1_HW_I2C display(U8G2_R0, U8X8_PIN_NONE, 3, 2);
// TODO extract pint to config
// TODO add second display (software i2c)


// ============ SETUP ============
void setup() {
  // initialize serial print if debug
  DEBUG_BEGIN();
  DEBUG_PRINTLN("Startup with DEBUG enabled");

  // initialize display
  display.setFont(u8g2_font_6x10_tr);
  display.begin();
  DEBUG_PRINTLN("Display initialized");

  // initialize LED strip
  ledStrip.begin();
  ledStrip.setBrightness(LED_BRIGHT);
  ledStrip.show();
  DEBUG_PRINTLN("LED-Strip initialized");

  // initialize key matrix
  for (byte r = 0; r < ROW_COUNT; r++)
    for (byte c = 0; c < COL_COUNT; c++)
      keys[r][c] = Key(rows[r], cols[c], getLedIndex(r, c), &ledStrip, &handleKeyPress);
  DEBUG_PRINTLN("Matrix initialized");

  // TODO fix Key documentation?

  // initialize keyboard
  Keyboard.begin(KeyboardLayout_de_DE);
  DEBUG_PRINTLN("Keyboard initialized");

  // initialize start values
  lastKeyPress = millis();
  sleeping = false;
  refreshDisplay();
  setLedDefaultValues();
  DEBUG_PRINTLN("Values initialized");
}


// ============ LOOP =============
void loop() {
  // DEBUG_PRINTLN(freeMemory()); // TODO remove?

  // check if debounce time is over
  if ((millis() - lastRefresh) > DEBOUNCE_DELAY) {
    readMatrix();
    lastRefresh = millis();
  }

  // check if timout time is over
  if ((millis() - lastKeyPress) > ((long)SLEEP_DELAY * 1000)) {
    sleep();
  }
}


// ========== DISPLAY ============

/**
 * Updates display using @link setDisplayText, if @link sleeping is false.
 */
void refreshDisplay() {
  if (!sleeping) {
    display.firstPage();  // way of reducing RAM usage
    do {
      setDisplayText();
    } while (display.nextPage());
  }

  // TODO make sure this is efficient?
}

/**
 * Writes texts of corresponding layer on the display.
 * @see drawText
 * @see refreshDisplay
 * @see currentLayer
 * @see layerNames
 * @see layerButtonFunc
 */
void setDisplayText() {
  // set layer text
  char layerBuf[MAX_LAYER_LENGTH + 1];   // buffer to read layer name to
  getProgMemStr(layerNames[currentLayer], layerBuf);
  drawText(layerBuf, CENTER, ROW0);

  // set lines
  display.drawLine(LEFT, HLINE1, RIGHT, HLINE1);
  display.drawLine(VLINE1, TOP, VLINE1, BOTTOM);
  display.drawLine(VLINE2, TOP, VLINE2, BOTTOM);

  // set key texts
  char actionBuf[MAX_KEY_LENGTH + 1];   // buffer to read key name to
  getProgMemStr(layerButtonFunc[currentLayer][0], actionBuf);
  drawText(actionBuf, LEFT, ROW1);
  getProgMemStr(layerButtonFunc[currentLayer][1], actionBuf);
  drawText(actionBuf, CENTER, ROW1);
  getProgMemStr(layerButtonFunc[currentLayer][2], actionBuf);
  drawText(actionBuf, RIGHT, ROW1);

  getProgMemStr(layerButtonFunc[currentLayer][3], actionBuf);
  drawText(actionBuf, LEFT, ROW2);
  getProgMemStr(layerButtonFunc[currentLayer][4], actionBuf);
  drawText(actionBuf, CENTER, ROW2);
  getProgMemStr(layerButtonFunc[currentLayer][5], actionBuf);
  drawText(actionBuf, RIGHT, ROW2);

  getProgMemStr(layerButtonFunc[currentLayer][6], actionBuf);
  drawText(actionBuf, LEFT, ROW3);
  getProgMemStr(layerButtonFunc[currentLayer][7], actionBuf);
  drawText(actionBuf, CENTER, ROW3);
  getProgMemStr(layerButtonFunc[currentLayer][8], actionBuf);
  drawText(actionBuf, RIGHT, ROW3);

  getProgMemStr(layerButtonFunc[currentLayer][9], actionBuf);
  drawText(actionBuf, LEFT, ROW4);
  getProgMemStr(layerButtonFunc[currentLayer][10], actionBuf);
  drawText(actionBuf, CENTER, ROW4);
  getProgMemStr(layerButtonFunc[currentLayer][11], actionBuf);
  drawText(actionBuf, RIGHT, ROW4);

  // TODO dynamic way?
}

/**
 * Print a text at a specific position on the display taking into account the text height and width.
 */
void drawText(const char *buf, byte xPosition, byte yPosition) {
  // get text dimensions
  int h = display.getFontAscent() - display.getFontDescent();
  int w = display.getStrWidth(buf);

  // get needed cursor position
  int xVal;
  if (xPosition == LEFT) {
    xVal = LEFT;
  } else if (xPosition == CENTER) {
    xVal = CENTER - w / 2;;
  } else if (xPosition == RIGHT) {
    xVal = RIGHT - w;
  }
  display.setCursor(xVal, yPosition + h);

  // draw text
  display.print(buf);

  // TODO use e.g. enum with custom values for text 'alignment'? and maybe ROWs too?
}


// ============ LEDs =============

/**
 * Turns all LEDs on and sets their default color according to the current layer.
 */
void setLedDefaultValues() {
  for (int rowIndex = 0; rowIndex < ROW_COUNT; rowIndex++) {
    for (int colIndex = 0; colIndex < COL_COUNT; colIndex++) {
      keys[rowIndex][colIndex].setLedDefault();
    }
  }
}

/**
 * Get the led index of the specified key row and column.
 * @return the led index as byte.
 */
byte getLedIndex(byte rowIdx, byte colIdx) {
  byte index = rowIdx * COL_COUNT;
  // account for S-shape of led strip
  index += rowIdx % 2 == 0 ? colIdx : COL_COUNT - 1 - colIdx;
  return index;
}


// ========== MATRIX =============

/**
 * Scans the key matrix to see if a key is pressed.
 * @see keys
 * @see keyPressed
 * @see resetKey
 */
void readMatrix() {
  // iterate through matrix
  for (int rowIndex = 0; rowIndex < ROW_COUNT; rowIndex++) {
    for (int colIndex = 0; colIndex < COL_COUNT; colIndex++) {
      // check if key is pressed and handle press if necessary
      keys[rowIndex][colIndex].check();
    }
  }
}

/**
 * Key-pressed event handler passed to key objects.
 * Calls the corresponding handler function of the pressed key.
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
void handleKeyPress(Key *key) {
  lastKeyPress = millis();
  wake();

  switch (key->getIndex()) {
    case KEY_LAYER_UP:
      currentLayer = (currentLayer + 1) % MAX_LAYER;
      refreshDisplay();
      setLedDefaultValues();
      break;
    case KEY_LAYER_HOME:
      currentLayer = HOME_LAYER;
      refreshDisplay();
      setLedDefaultValues();
      break;
    case KEY_LAYER_DOWN:
      currentLayer = (currentLayer - 1 + MAX_LAYER) % MAX_LAYER;
      refreshDisplay();
      setLedDefaultValues();
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
      DEBUG_PRINTLN("ERROR: Unknown key pressed");
      break;
  }

  Keyboard.releaseAll();  // protection against unwanted spam
}


// =========== SLEEP =============

/**
 * Sets components to sleep if @link sleeping is not already true.
 */
void sleep() {
  if (!sleeping) {
    sleeping = true;
    sleepDisplay();
    sleepLEDStrip();
  }
}

/**
 * Wakes components if @link sleeping is not already false.
 */
void wake() {
  if (sleeping) {
    sleeping = false;
    wakeDisplay();
    wakeLEDStrip();
  }
}

/**
 * Forces @link display into sleep.
 */
void sleepDisplay() {
  DEBUG_PRINTLN("Display going to sleep...");
  display.sleepOn();
}

/**
 * Wakes @link display from sleep.
 */
void wakeDisplay() {
  DEBUG_PRINTLN("Display waking up...");
  display.sleepOff();
  refreshDisplay();
}

/**
 * Forces @link ledStrip into sleep.
 */
void sleepLEDStrip() {
  DEBUG_PRINTLN("LEDs going to sleep...");
  for (int rowIndex = 0; rowIndex < ROW_COUNT; rowIndex++) {
    for (int colIndex = 0; colIndex < COL_COUNT; colIndex++) {
      keys[rowIndex][colIndex].setLedOff();
    }
  }
}

/**
 * Wakes @link ledStrip from sleep and sets each led with its last color.
 */
void wakeLEDStrip() {
  DEBUG_PRINTLN("LEDs waking up...");
  ledStrip.clear();
  ledStrip.show();
}
