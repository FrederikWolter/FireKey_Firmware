/*********************************************************************
* FIREKEY - PROJECT
* 
* Firmware Version 0.5V
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

//TODO Serial.println(F("xxx"));
//TODO move const to config file
//TODO move led & keypress to key
//TODO IFDEF DEBUG for serial print (debug.h)

// LIBRARIES
#include <avr/pgmspace.h>
#include "config.h"
#include "Basics.h"

#include <MemoryFree.h>  //TODO remove
#include <Keyboard.h>
#include <Keyboard_de_DE.h>
#include <KeyboardLayout.h>
#include <Adafruit_NeoPixel.h>
#include <U8g2lib.h>
#include "Key.h"

unsigned long debounceTime;    // debounce last check
unsigned long lastKeyPressed;  // last time a key got pressed (for the timeout functionality)

byte currentLayer = 0;  //the current selected layer

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
  Serial.begin(9600);

  // initialize the display
  display.setFont(u8g2_font_6x10_tr);
  //display.setI2CAddress(OLED_ADDR);
  display.begin();

  Serial.println(F("Display initialized"));

  // initialize LED strip
  ledStrip.begin();
  // set the led brightness
  ledStrip.setBrightness(LED_BRIGHT);
  ledStrip.show();
  Serial.println(F("LED-Strip initialized"));

  // Setup key matrix
  for (byte r = 0; r < ROW_COUNT; r++) {
    for (byte c = 0; c < COL_COUNT; c++) {
      keys[r][c] = Key(rows[r], cols[c], getLedIndex(r, c), &ledStrip);
    }
  }
  Serial.println(F("Matrix initialized"));

  // initialize keyboard
  Keyboard.begin(KeyboardLayout_de_DE);
  Serial.println(F("Keyboard initialized"));

  // setup default values
  lastKeyPressed = millis();
  sleeping = false;
  refreshDisplay();
  setLayerRGB();
}

void loop() {
  //Serial.println(freeMemory());

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
  Serial.println(F("Display is going to sleep..."));
  display.sleepOn();
}

/**
* Wakes the @link display from the sleeping state
*/
void wakeDisplay() {
  Serial.println(F("Waking up display..."));
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
  Serial.println("Leds are going to sleep...");
  ledStrip.clear();
  ledStrip.show();
}

/**
* Wakes the @link ledStrip from the sleeping state
*/
void wakeLEDStrip() {
  Serial.println("Waking up leds...");
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
      if (key.update()) { //TODO Rework
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

/**
* Handles the key 1 actions for each layer
*/
void keyOnePressed(Key key) {
  switch (currentLayer) {
    case 0:
      //K1L0
      break;
    case 1:
      //K1L1
      break;
    case 2:
      //K1L2
      break;
    case 3:
      //K1L3
      break;
    case 4:
      //K1L4
      break;
    case 5:
      //K1L5
      break;
  }
  Keyboard.releaseAll();
}

/**
* Handles the key 2 actions for each layer
*/
void keyTwoPressed(Key key) {
  switch (currentLayer) {
    case 0:
      //K2L0
      break;
    case 1:
      //K2L1
      break;
    case 2:
      //K2L2
      break;
    case 3:
      //K2L3
      break;
    case 4:
      //K2L4
      break;
    case 5:
      //K2L5
      break;
  }
  Keyboard.releaseAll();
}

/**
* Handles the key 3 actions for each layer
*/
void keyThreePressed(Key key) {
  switch (currentLayer) {
    case 0:
      //K3L0
      break;
    case 1:
      //K3L1
      break;
    case 2:
      //K3L2
      break;
    case 3:
      //K3L3
      break;
    case 4:
      //K3L4
      break;
    case 5:
      //K3L5
      break;
  }
  Keyboard.releaseAll();
}

/**
* Handles the key 4 actions for each layer
*/
void keyFourPressed(Key key) {
  switch (currentLayer) {
    case 0:
      //K4L0
      break;
    case 1:
      //K4L1
      break;
    case 2:
      //K4L2
      break;
    case 3:
      //K4L3
      break;
    case 4:
      //K4L4
      break;
    case 5:
      //K4L5
      break;
  }
  Keyboard.releaseAll();
}

/**
* Handles the key 5 actions for each layer
*/
void keyFivePressed(Key key) {
  switch (currentLayer) {
    case 0:
      //K5L0
      break;
    case 1:
      //K5L1
      break;
    case 2:
      //K5L2
      break;
    case 3:
      //K5L3
      break;
    case 4:
      //K5L4
      break;
    case 5:
      //K5L5
      break;
  }
  Keyboard.releaseAll();
}

/**
* Handles the key 6 actions for each layer
*/
void keySixPressed(Key key) {
  switch (currentLayer) {
    case 0:
      //K6L0
      break;
    case 1:
      //K6L1
      break;
    case 2:
      //K6L2
      break;
    case 3:
      //K6L3
      break;
    case 4:
      //K6L4
      break;
    case 5:
      //K6L5
      break;
  }
  Keyboard.releaseAll();
}

/**
* Handles the key 7 actions for each layer
*/
void keySevenPressed(Key key) {
  switch (currentLayer) {
    case 0:
      //K7L0
      break;
    case 1:
      //K7L1
      break;
    case 2:
      //K7L2
      break;
    case 3:
      //K7L3
      break;
    case 4:
      //K7L4
      break;
    case 5:
      //K7L5
      break;
  }
  Keyboard.releaseAll();
}

/**
* Handles the key 8 actions for each layer
*/
void keyEightPressed(Key key) {
  switch (currentLayer) {
    case 0:
      //K8L0
      break;
    case 1:
      //K8L1
      break;
    case 2:
      //K8L2
      break;
    case 3:
      //K8L3
      break;
    case 4:
      //K8L4
      break;
    case 5:
      //K8L5
      break;
  }
  Keyboard.releaseAll();
}

/**
* Handles the key 9 actions for each layer
*/
void keyNinePressed(Key key) {
  switch (currentLayer) {
    case 0:
      //K9L0
      break;
    case 1:
      //K9L1
      break;
    case 2:
      //K9L2
      break;
    case 3:
      //K9L3
      break;
    case 4:
      //K9L4
      break;
    case 5:
      //K9L5
      break;
  }
  Keyboard.releaseAll();
}

/**
* Handles the key 10 actions for each layer
*/
void keyTenPressed(Key key) {
  switch (currentLayer) {
    case 0:
      //K10L0
      break;
    case 1:
      //K10L1
      break;
    case 2:
      //K10L2
      break;
    case 3:
      //K10L3
      break;
    case 4:
      //K10L4
      break;
    case 5:
      //K10L5
      break;
  }
  Keyboard.releaseAll();
}

/**
* Handles the key 11 actions for each layer
*/
void keyElevenPressed(Key key) {
  switch (currentLayer) {
    case 0:
      //K11L0
      break;
    case 1:
      //K11L1
      break;
    case 2:
      //K11L2
      break;
    case 3:
      //K11L3
      break;
    case 4:
      //K11L4
      break;
    case 5:
      //K11L5
      break;
  }
  Keyboard.releaseAll();
}

/**
* Handles the key 12 actions for each layer
*/
void keyTwelvePressed(Key key) {
  switch (currentLayer) {
    case 0:
      //K12L0
      break;
    case 1:
      //K12L1
      break;
    case 2:
      //K12L2
      break;
    case 3:
      //K12L3
      break;
    case 4:
      //K12L4
      break;
    case 5:
      //K12L5
      break;
  }
  Keyboard.releaseAll();
}
