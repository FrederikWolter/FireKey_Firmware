/*************************************************************************
* FIREKEY-PROJECT
* Firmware Version 1.0.2
* 
* Required libs:
* - Keyboard          (https://github.com/arduino-libraries/Keyboard) 
* - USBHost           (https://github.com/arduino-libraries/USBHost)
* - Adafruit NeoPixel (https://github.com/adafruit/Adafruit_NeoPixel)
*   -> requires: Adafruit BusIO & Adafruit GFX Library
* - U8g2              (https://github.com/olikraus/u8g2)
* - avr/pgmspace      (part of Pro Micro Board - preinstalled)
* -> can all be installed via Arduino IDE library manager
*
* Firmware is tested on a Arduino Pro Micro.
* Required Board:
* - Arduino AVR Boards
*************************************************************************/

// TODO more DEBUG_PRINTLN(F("xxx"));
// TODO header split (.h & .cpp)?
// TODO change USB Device information? (Nice to have)
// TODO fading LEDs?


// ===== LIBRARIES & CONFIG ======
#include "Config.h"


// ========== VARIABLES ==========
unsigned long lastRefresh;   // last checked matrix (for debouncing)
unsigned long lastKeyPress;  // last time a key pressed (for timeout functionality)
bool sleeping;               // is display & led strip sleeping?

// LED strip object
Adafruit_NeoPixel ledStrip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// Key objects
Key keys[ROW_COUNT][COL_COUNT];

// OLED objects
U8G2_SH1106_128X64_NONAME_F_SW_I2C oled1(U8G2_R0, OLED1_SCL_PIN, OLED1_SDA_PIN, U8X8_PIN_NONE);
U8G2_SH1106_128X64_NONAME_F_HW_I2C oled2(U8G2_R0, U8X8_PIN_NONE, OLED2_SCL_PIN, OLED2_SDA_PIN);


// ============ SETUP ============
void setup() {
  // initialize serial print if debug
  DEBUG_BEGIN();
  DEBUG_PRINTLN("Startup with DEBUG enabled");

  // disable onboard RX/TX LEDs
  pinMode(LED_BUILTIN_TX, INPUT);
  pinMode(LED_BUILTIN_RX, INPUT);
  DEBUG_PRINTLN("RX/TX LEDs disabled");

  // initialize display
  oled1.setFont(u8g2_font_7x14_tr);
  oled1.begin();
  oled2.setFont(u8g2_font_7x14_tr);
  oled2.begin();
  DEBUG_PRINTLN("Displays initialized");

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
  //Serial.println(freeMemory()); // TODO remove?

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
 * Updates display using @link setDisplayText1 & @link setDisplayText2, if @link sleeping is false.
 */
void refreshDisplay() {
  if (!sleeping) {
    DEBUG_PRINTLN("Refreshing display 1...");
    setDisplayText1();
    DEBUG_PRINTLN("Refreshing display 2...");
    setDisplayText2();
  }
}

/**
 * Writes texts of corresponding layer on the display first oled display.
 * @see drawText
 * @see refreshDisplay
 * @see currentLayer
 * @see layerNames
 * @see layerButtonFunc
 */
void setDisplayText1() {
  oled1.clearBuffer();
  // set layer text
  char layerBuf[MAX_LAYER_LENGTH + 1];  // buffer to read layer name to
  getProgMemStr(layerNames[currentLayer], layerBuf);
  drawText(layerBuf, CENTER, 0, &oled1);

  // set lines
  oled1.drawLine(LEFT, HLINE1, RIGHT, HLINE1);
  oled1.drawLine(VLINE1, TOP, VLINE1, BOTTOM);

  // set key texts
  char actionBuf[MAX_KEY_LENGTH + 1];  // buffer to read key name to
  for (int i = 0; i < NUBMER_OF_OLED_KEYS; i++) {
    int row = i / NUM_OF_COLS;
    int col = i % NUM_OF_COLS;
    getProgMemStr(layerButtonFunc[currentLayer][i], actionBuf);
    drawText(actionBuf, col * COL_WIDTH + (COL_WIDTH / 2), row * ROW_HEIGHT + TOP, &oled1);
  }
  oled1.sendBuffer();
}

/**
 * Writes texts of corresponding layer on the display second oled display.
 * @see drawText
 * @see refreshDisplay
 * @see currentLayer
 * @see layerNames
 * @see layerButtonFunc
 */
void setDisplayText2() {
  oled2.clearBuffer();
  // set lines
  oled2.drawLine(LEFT, HLINE1, RIGHT, HLINE1);
  oled2.drawLine(VLINE1, TOP, VLINE1, BOTTOM);

  char actionBuf[MAX_KEY_LENGTH + 1];  // buffer to read key name to
  for (int i = 0; i < NUBMER_OF_OLED_KEYS; i++) {
    int row = i / NUM_OF_COLS;
    int col = i % NUM_OF_COLS;
    getProgMemStr(layerButtonFunc[currentLayer][i + NUBMER_OF_OLED_KEYS], actionBuf);
    drawText(actionBuf, col * COL_WIDTH + (COL_WIDTH / 2), row * ROW_HEIGHT + TOP, &oled2);
  }
  oled2.sendBuffer();
}

/**
 * Print a text at a specific position on the display taking into account the text height and width.
 */
void drawText(const char *buf, byte xPosition, byte yPosition, U8G2 *oled) {
  // get text dimensions
  int h = oled->getFontAscent() - oled->getFontDescent();
  int w = oled->getStrWidth(buf);

  // draw text
  oled->drawStr(xPosition - w / 2, yPosition + h, buf);
}


// ============ LEDs =============

/**
 * Turns all LEDs on and sets their default color according to the current layer.
 */
void setLedDefaultValues() {
  DEBUG_PRINTLN("Setting default led colors...");
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
      refreshDisplay();  // TODO performance -> order change?
      setLedDefaultValues();
      DEBUG_PRINTLN("Layer up pressed");
      break;
    case KEY_LAYER_HOME:
      currentLayer = HOME_LAYER;
      refreshDisplay();
      setLedDefaultValues();
      DEBUG_PRINTLN("Layer home pressed");
      break;
    case KEY_LAYER_DOWN:
      currentLayer = (currentLayer - 1 + MAX_LAYER) % MAX_LAYER;
      refreshDisplay();
      setLedDefaultValues();
      DEBUG_PRINTLN("Layer down pressed");
      break;
    case KEY_1:
      keyOnePressed(key);
      DEBUG_PRINTLN("Key one pressed");
      break;
    case KEY_2:
      keyTwoPressed(key);
      DEBUG_PRINTLN("Key two pressed");
      break;
    case KEY_3:
      keyThreePressed(key);
      DEBUG_PRINTLN("Key three pressed");
      break;
    case KEY_4:
      keyFourPressed(key);
      DEBUG_PRINTLN("Key four pressed");
      break;
    case KEY_5:
      keyFivePressed(key);
      DEBUG_PRINTLN("Key five pressed");
      break;
    case KEY_6:
      keySixPressed(key);
      DEBUG_PRINTLN("Key six pressed");
      break;
    case KEY_7:
      keySevenPressed(key);
      DEBUG_PRINTLN("Key seven pressed");
      break;
    case KEY_8:
      keyEightPressed(key);
      DEBUG_PRINTLN("Key eight pressed");
      break;
    case KEY_9:
      keyNinePressed(key);
      DEBUG_PRINTLN("Key nine pressed");
      break;
    case KEY_10:
      keyTenPressed(key);
      DEBUG_PRINTLN("Key ten pressed");
      break;
    case KEY_11:
      keyElevenPressed(key);
      DEBUG_PRINTLN("Key eleven pressed");
      break;
    case KEY_12:
      keyTwelvePressed(key);
      DEBUG_PRINTLN("Key twelve pressed");
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
  oled1.sleepOn();
  oled2.sleepOn();
}

/**
 * Wakes @link display from sleep.
 */
void wakeDisplay() {
  DEBUG_PRINTLN("Display waking up...");
  oled1.sleepOff();
  oled2.sleepOff();
  refreshDisplay();
}

/**
 * Forces @link ledStrip into sleep.
 */
void sleepLEDStrip() {
  DEBUG_PRINTLN("LEDs going to sleep...");
  ledStrip.clear();
  ledStrip.show();
}

/**
 * Wakes @link ledStrip from sleep and sets each led with its last color.
 */
void wakeLEDStrip() {
  DEBUG_PRINTLN("LEDs waking up...");
  for (int rowIndex = 0; rowIndex < ROW_COUNT; rowIndex++) {
    for (int colIndex = 0; colIndex < COL_COUNT; colIndex++) {
      keys[rowIndex][colIndex].setLedOn();
    }
  }
}
