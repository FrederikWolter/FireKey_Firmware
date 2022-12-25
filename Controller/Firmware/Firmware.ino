/*********************************************************************
* FIREKEY - PROJECT
* 
* Firmware Version 0.1V
* 
* Required libs:
* - Keyboard(https://github.com/arduino-libraries/Keyboard)
*   -> can be installed via arduino ide (defauled installed)
* - USBHost
*   -> can be installed via arduino ide
* - Adafruit NeoPixel (https://github.com/adafruit/Adafruit_NeoPixel) 
*    -> can be installed via Arduino IDE
* - Adafruit SH1106 Lib(https://github.com/wonho-maker/Adafruit_SH1106)
*    -> is located in the 20_Display folder
* - Adafruit GFX Lib(https://github.com/adafruit/Adafruit-GFX-Library)
*    -> can be installed via Arduino IDE
*********************************************************************/

// LIBRARIES
#include <MemoryFree.h>
#include <Keyboard.h>
#include <Keyboard_de_DE.h>
#include <KeyboardLayout.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
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
#define NUM_LEDS 6  // number of LEDs in the strip
#define LED_PIN 21

// Display
#define OLED_RESET -1     // use no extra reset pin
#define OLED_ADDR 0x3C    //i2c address
#define MAX_KEY_LENGTH 5  //max length for a key action name

// Positions
#define LEFT 0
#define VLINE1 41
#define CENTER 64
#define VLINE2 85
#define RIGHT 127
#define TOP 0
#define ROW0 2
#define HLINE1 11
#define ROW1 16
#define ROW2 28
#define ROW3 40
#define ROW4 52
#define BOTTOM 63
#define OFFSET 32

// End CONSTANTS

unsigned long startTime;  // debounce last check

const byte rows[ROW_COUNT] PROGMEM = { 5, 6, 7, 8, 9 };  // define the row pins
const byte cols[COL_COUNT] PROGMEM = { 10, 16, 14 };     // define the column pins
byte currentLayer = 0;

//TODO Move to key class?
byte keyDownCounter[COL_COUNT * ROW_COUNT];
bool keySpamMode[COL_COUNT * ROW_COUNT];  // defines if a key is in spam mode or not

// Uses as second index the key index which is the led index
const char layerButtonFunc[MAX_LAYER][12][MAX_KEY_LENGTH] = {
  // Last defines max amount of chars for a name of a key
  { "L0L1", "L0M1", "L0R1",  // name: Layer 0 Left Button Row 1; Layer 0 Middle Button Row 1; Layer 0 Middle Button Row 1
    "L0R2", "L0M2", "L0L2",
    "L0L3", "L0M3", "L0R3",
    "L0R4", "L0M4", "L0L4" },
  { "L1L1", "L1M1", "L1R1",
    "L1R2", "L1M2", "L1L2",
    "L1L3", "L1M3", "L1R3",
    "L1R4", "L1M4", "L1L4" },
  { "L2L1", "L2M1", "L2R1",
    "L2R2", "L2M2", "L2L2",
    "L2L3", "L2M3", "L2R3",
    "L2R4", "L2M4", "L2L4" },
  { "L3L1", "L3M1", "L3R1",
    "L3R2", "L3M2", "L3L2",
    "L3L3", "L3M3", "L3R3",
    "L3R4", "L3M4", "L3L4" },
  { "L4L1", "L4M1", "L4R1",
    "L4R2", "L4M2", "L4L2",
    "L4L3", "L4M3", "L4R3",
    "L4R4", "L4M4", "L4L4" },
};

const char layerNames[MAX_LAYER][10] = {
  "Layer1",
  "Layer2",
  "Layer3",
  "Layer4",
  "Layer5",
};

// LED strip object
Adafruit_NeoPixel ledStrip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

Key keys[ROW_COUNT][COL_COUNT];

// OLED object
Adafruit_SH1106 display(OLED_RESET);

void setup() {
  Serial.begin(9600);

  display.begin(SH1106_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();

  // initialize LED strip object
  ledStrip.begin();

  // Setup key matrix
  for (byte r = 0; r < ROW_COUNT; r++) {
    for (byte c = 0; c < COL_COUNT; c++) {
      keys[r][c] = Key(pgm_read_byte_near(rows + r), pgm_read_byte_near(cols + c), getLedIndex(r, c), ledStrip);
    }
  }
}

void loop() {
  Serial.println(freeMemory());
  delay(1000);
  if ((millis() - startTime) > DEBOUNCE_TIME) {
    readMatrix();
    startTime = millis();
  }
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
}
