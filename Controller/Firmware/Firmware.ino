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
*********************************************************************/

//TODO Comments && Serial.println(F("xxx"));
//TODO LED Timeout like the display

// LIBRARIES
#include <MemoryFree.h>
#include <Keyboard.h>
#include <Keyboard_de_DE.h>
#include <KeyboardLayout.h>
#include <Adafruit_NeoPixel.h>
#include <U8g2lib.h>
#include "00_Key.h"
#include <avr/pgmspace.h>

// Begin CONSTANTS

// Keys
#define ROW_COUNT 5       // number of rows
#define COL_COUNT 3       // number of columns
#define SPAM_SPEED 15     // how often is a key pressed while holding it down
#define HOLD_DELAY 100    // delay before a button is marked as held. max 255, otherwise change keyDownCounter to int
#define DEBOUNCE_TIME 10  // Limit how often the metric is scanned.

// Layers
#define LAYER_BACK_KEY 2     // define the back button led index
#define LAYER_HOME_KEY 1     // define the home button led index
#define LAYER_FORWARD_KEY 0  // define the forward button led index
#define MAX_LAYER 5
#define HOME_LAYER 0

// Button constants defining the led index
#define KEY_1 3
#define KEY_2 4
#define KEY_3 5
#define KEY_4 8
#define KEY_5 7
#define KEY_6 6
#define KEY_7 9
#define KEY_8 10
#define KEY_9 11
#define KEY_10 14
#define KEY_11 13
#define KEY_12 12

// Led
#define NUM_LEDS 15  // number of LEDs in the strip
#define LED_PIN 21

// Display
#define OLED_RESET -1           // use no extra reset pin
#define OLED_ADDR 0x3C          //i2c address display1
#define MAX_KEY_LENGTH 5        //max length for a key action name
#define LAYER_NAME_LENGTH 10    //max length for a layer name
#define SLEEP_DELAY_SECONDS 60  // delay in seconds before the display is going to sleep

// Positions
#define LEFT 0
#define VLINE1 41
#define CENTER 64
#define VLINE2 85
#define RIGHT 127
#define TOP 14
#define ROW0 2
#define HLINE1 14
#define ROW1 16
#define ROW2 28
#define ROW3 40
#define ROW4 52
#define BOTTOM 63

// End CONSTANTS

unsigned long debounceTime;    // debounce last check
unsigned long lastKeyPressed;  // debounce last check

const byte rows[ROW_COUNT] = { 5, 6, 7, 8, 9 };  // define the row pins
const byte cols[COL_COUNT] = { 10, 16, 14 };     // define the column pins
byte currentLayer = 0;

//TODO Move to key class?
byte keyDownCounter[COL_COUNT * ROW_COUNT];
bool keySpamMode[COL_COUNT * ROW_COUNT];  // defines if a key is in spam mode or not

const char layerNames[MAX_LAYER][LAYER_NAME_LENGTH] PROGMEM = {
  "Layer1",
  "Layer2",
  "Layer3",
  "Layer4",
  "Layer5",
};

// Uses as second index the key index which is the led index
const char layerButtonFunc[MAX_LAYER][12][MAX_KEY_LENGTH] PROGMEM = {
  // Last defines max amount of chars for a name of a key
  { "L0L1", "L0M1", "L0R1",  // name: Layer 0 Left Button Row 1; Layer 0 Middle Button Row 1; Layer 0 Middle Button Row 1
    "L0L2", "L0M2", "L0R2",
    "L0L3", "L0M3", "L0R3",
    "L0L4", "L0M4", "L0R4" },
  { "L1L1", "L1M1", "L1R1",
    "L1L2", "L1M2", "L1R2",
    "L1L3", "L1M3", "L1R3",
    "L1L4", "L1M4", "L1R4" },
  { "L2L1", "L2M1", "L2R1",
    "L2L2", "L2M2", "L2R2",
    "L2L3", "L2M3", "L2R3",
    "L2L4", "L2M4", "L2R4" },
  { "L3L1", "L3M1", "L3R1",
    "L3L2", "L3M2", "L3R2",
    "L3L3", "L3M3", "L3R3",
    "L3L4", "L3M4", "L3R4" },
  { "L4L1", "L4M1", "L4R1",
    "L4L2", "L4M2", "L4R2",
    "L4L3", "L4M3", "L4R3",
    "L4L4", "L4M4", "L4R4" },
};

const byte layerRGB[MAX_LAYER][15][3] PROGMEM = {
  {
    { 0, 0, 0 },
    { 0, 0, 1 },
    { 0, 0, 2 },
    { 0, 0, 3 },
    { 0, 0, 4 },
    { 0, 0, 5 },
    { 0, 0, 6 },
    { 0, 0, 7 },
    { 0, 0, 8 },
    { 0, 0, 9 },
    { 0, 0, 10 },
    { 0, 0, 11 },
    { 0, 0, 12 },
    { 0, 0, 13 },
    { 0, 0, 14 },
  },
  {
    { 0, 1, 0 },
    { 0, 1, 1 },
    { 0, 1, 2 },
    { 0, 1, 3 },
    { 0, 1, 4 },
    { 0, 1, 5 },
    { 0, 1, 6 },
    { 0, 1, 7 },
    { 0, 1, 8 },
    { 0, 1, 9 },
    { 0, 1, 10 },
    { 0, 1, 11 },
    { 0, 1, 12 },
    { 0, 1, 13 },
    { 0, 1, 14 },
  },
  {
    { 0, 2, 0 },
    { 0, 2, 1 },
    { 0, 2, 2 },
    { 0, 2, 3 },
    { 0, 2, 4 },
    { 0, 2, 5 },
    { 0, 2, 6 },
    { 0, 2, 7 },
    { 0, 2, 8 },
    { 0, 2, 9 },
    { 0, 2, 10 },
    { 0, 2, 11 },
    { 0, 2, 12 },
    { 0, 2, 13 },
    { 0, 2, 14 },
  },
  {
    { 0, 3, 0 },
    { 0, 3, 1 },
    { 0, 3, 2 },
    { 0, 3, 3 },
    { 0, 3, 4 },
    { 0, 3, 5 },
    { 0, 3, 6 },
    { 0, 3, 7 },
    { 0, 3, 8 },
    { 0, 3, 9 },
    { 0, 3, 10 },
    { 0, 3, 11 },
    { 0, 3, 12 },
    { 0, 3, 13 },
    { 0, 3, 14 },
  },
  {
    { 0, 4, 0 },
    { 0, 4, 1 },
    { 0, 4, 2 },
    { 0, 4, 3 },
    { 0, 4, 4 },
    { 0, 4, 5 },
    { 0, 4, 6 },
    { 0, 4, 7 },
    { 0, 4, 8 },
    { 0, 4, 9 },
    { 0, 4, 10 },
    { 0, 4, 11 },
    { 0, 4, 12 },
    { 0, 4, 13 },
    { 0, 4, 14 },
  },
};

bool sleeping;  // check if the display is sleeping

// LED strip object
Adafruit_NeoPixel ledStrip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

Key keys[ROW_COUNT][COL_COUNT];

// OLED object
U8G2_SH1106_128X64_NONAME_1_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);

void setup() {
  Serial.begin(9600);

  display.setFont(u8g2_font_6x10_tr);
  //display.setI2CAddress(OLED_ADDR);
  display.begin();

  Serial.println(F("Display initialized"));

  // initialize LED strip object
  ledStrip.begin();
  Serial.println(F("LED-Strip initialized"));

  // Setup key matrix
  for (byte r = 0; r < ROW_COUNT; r++) {
    for (byte c = 0; c < COL_COUNT; c++) {
      keys[r][c] = Key(rows[r], cols[c], getLedIndex(r, c), &ledStrip);
    }
  }
  Serial.println(F("Matrix initialized"));

  Keyboard.begin(KeyboardLayout_de_DE);
  Serial.println(F("Keyboard initialized"));

  lastKeyPressed = millis();
  sleeping = false;
  refreshDisplay();
  setLayerRGB();
}

void loop() {
  //Serial.println(freeMemory());

  if ((millis() - debounceTime) > DEBOUNCE_TIME) {
    readMatrix();
    debounceTime = millis();
  }

  if ((millis() - lastKeyPressed) > ((long)SLEEP_DELAY_SECONDS * 1000)) {
    sleepDisplay();
  }
}

void refreshDisplay() {
  if (!sleeping) {
    display.firstPage();
    do {
      setDisplayText();
    } while (display.nextPage());
  }
}

void sleepDisplay() {
  if (!sleeping) {
    sleeping = true;
    Serial.println(F("Display is going to sleep..."));
    display.sleepOn();
  }
}

void wakeDisplay() {
  if (sleeping) {
    sleeping = false;
    Serial.println(F("Waking up display..."));
    display.sleepOff();
    refreshDisplay();
  }
}

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

void setLayerRGB() {
  for (byte k = 0; k < NUM_LEDS; k++) {
    ledStrip.setPixelColor(k, pgm_read_byte_near(&layerRGB[currentLayer][k][0]), pgm_read_byte_near(&layerRGB[currentLayer][k][1]), pgm_read_byte_near(&layerRGB[currentLayer][k][2]));
  }
  ledStrip.show();
}

byte getLedIndex(byte rowIdx, byte colIdx) {
  // calculate led index out of row and col index
  byte index = rowIdx * COL_COUNT;
  index += rowIdx % 2 == 0 ? colIdx : COL_COUNT - 1 - colIdx;
  return index;
}

void readMatrix() {
  // scan matrix
  for (int rowIndex = 0; rowIndex < ROW_COUNT; rowIndex++) {

    // is any column pulled to low due to a button being pressed?
    for (int colIndex = 0; colIndex < COL_COUNT; colIndex++) {
      // Get key object
      Key key = keys[rowIndex][colIndex];

      // Check if the key is pressed
      if (key.update()) {
        keyPressed(key);
      } else if (keyDownCounter[key.getIndex()] != 0) {
        resetKey(key.getIndex());
      }
    }
  }
}

void resetKey(byte keyIndex) {
  keySpamMode[keyIndex] = false;
  keyDownCounter[keyIndex] = 0;
}

void keyPressed(Key key) {
  lastKeyPressed = millis();
  wakeDisplay();
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

void handleKeyPress(Key key) {
  Serial.println(key.getIndex());
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

void keyOnePressed(Key key) {
  switch (currentLayer) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
  }
  Keyboard.releaseAll();
}

void keyTwoPressed(Key key) {
  switch (currentLayer) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
  }
  Keyboard.releaseAll();
}

void keyThreePressed(Key key) {
  switch (currentLayer) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
  }
  Keyboard.releaseAll();
}

void keyFourPressed(Key key) {
  switch (currentLayer) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
  }
  Keyboard.releaseAll();
}

void keyFivePressed(Key key) {
  switch (currentLayer) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
  }
  Keyboard.releaseAll();
}

void keySixPressed(Key key) {
  switch (currentLayer) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
  }
  Keyboard.releaseAll();
}

void keySevenPressed(Key key) {
  switch (currentLayer) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
  }
  Keyboard.releaseAll();
}

void keyEightPressed(Key key) {
  switch (currentLayer) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
  }
  Keyboard.releaseAll();
}

void keyNinePressed(Key key) {
  switch (currentLayer) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
  }
  Keyboard.releaseAll();
}

void keyTenPressed(Key key) {
  switch (currentLayer) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
  }
  Keyboard.releaseAll();
}

void keyElevenPressed(Key key) {
  switch (currentLayer) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
  }
  Keyboard.releaseAll();
}

void keyTwelvePressed(Key key) {
  switch (currentLayer) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
  }
  Keyboard.releaseAll();
}
