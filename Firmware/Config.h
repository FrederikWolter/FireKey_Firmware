/*************************************************************************
* FIREKEY-PROJECT
* 
* CONFIG.h
* File defining the most important configoration data.
*************************************************************************/


// ========= DEFINITIONS =========

// debugging
#define DEBUG

// Matrix
#define ROW_COUNT 3           // number of rows
#define COL_COUNT 5           // number of columns

// Timing
#define SPAM_DELAY 15         // delay between actions while holding key pressed in spam mode
#define HOLD_DELAY 100        // delay before button enters spam mode (max. 255)
#define DEBOUNCE_DELAY 10     // delay between matrix scans (prevent bouncing artifacts)
#define SLEEP_DELAY 60        // delay in seconds before display & led strip is going to sleep

// Layers
#define MAX_LAYER 5           // number of available layers
#define HOME_LAYER 0          // default layer after home button is pressed

// Keys (led index - see schematic)
#define KEY_LAYER_UP 0
#define KEY_LAYER_HOME 9
#define KEY_LAYER_DOWN 10
#define KEY_1 4
#define KEY_2 3
#define KEY_3 2
#define KEY_4 1
#define KEY_5 5
#define KEY_6 6
#define KEY_7 7
#define KEY_8 8
#define KEY_9 14
#define KEY_10 13
#define KEY_11 12
#define KEY_12 11

// LED strip
#define LED_COUNT 15          // number of LEDs in the strip
#define LED_PIN 21            // pin connected to DIN of LED strip (A3)
#define LED_BRIGHT 64         // brightness 0-255 (brightness = (percentage / 100) * 255)

// Display
#define OLED1_SDA_PIN 4       // SDA pin for OLED 1 (software I2C)
#define OLED1_SCL_PIN 15      // SCL pin for OLED 1 (software I2C)
#define OLED2_SDA_PIN 2       // SDA pin for OLED 2 (hardware I2C)
#define OLED2_SCL_PIN 3       // SCL pin for OLED 2 (hardware I2C)
#define MAX_KEY_LENGTH 8      // max length for a key name
#define MAX_LAYER_LENGTH 10   // max length for a layer name
#define NUM_OF_COLS 2         // number of columns on the display
#define NUBMER_OF_OLED_KEYS 6 // number of keys on one display

// Positions on the display
#define LEFT 0                // text alignment left & start of horizontal line
#define CENTER 64             // text alignment center
#define RIGHT 127             // text alignment right & end of horizontal line
#define TOP 18                // start of vertical lines
#define BOTTOM 63             // end of vertical lines

#define VLINE1 64             // y position of vertical line
#define HLINE1 16             // x position of horizontal line

#define COL_WIDTH 64          // display col width in pixel
#define ROW_HEIGHT 16         // display row hight in pixel


// ========== CONSTANTS ==========
const byte rows[ROW_COUNT] = { 10, 16, 14 };    // row pins - row0, row1, ...
const byte cols[COL_COUNT] = { 9, 8, 7, 6, 5 }; // column pins - col0, col1, ...

byte currentLayer = HOME_LAYER;   // current selected layer

// display-names of layers
const char layerNames[MAX_LAYER][MAX_LAYER_LENGTH + 1] PROGMEM = {
  "Layer1",
  "Layer2",
  "Layer3",
  "Layer4",
  "Layer5",
};

// TODO change this with new layout
// display-names of keys for each layer (uses key index)
const char layerButtonFunc[MAX_LAYER][12][MAX_KEY_LENGTH + 1] PROGMEM = {
  { "D0L0L1", "D0L0R1",  // name: Display 0 Layer 0 Left Button Row 1; Display 0 Layer 0 Right Button Row 1
    "D0L0L2", "D0L0R2",
    "D0L0L3", "D0L0R3",
    "D1L0L1", "D1L0R1",  // name: Display 1 Layer 0 Left Button Row 1; Display 0 Layer 0 Right Button Row 1
    "D1L0L2", "D1L0R2",
    "D1L0L3", "D1L0R3" },
  { "D0L1L1", "D0L1R1",
    "D0L1L2", "D0L1R2",
    "D0L1L3", "D0L1R3",
    "D1L1L1", "D1L1R1",
    "D1L1L2", "D1L1R2",
    "D1L1L3", "D1L1R3" },
  { "D0L2L1", "D0L2R1",
    "D0L2L2", "D0L2R2",
    "D0L2L3", "D0L2R3",
    "D1L2L1", "D1L2R1",
    "D1L2L2", "D1L2R2",
    "D1L2L3", "D1L2R3" },
  { "D0L3L1", "D0L3R1",
    "D0L3L2", "D0L3R2",
    "D0L3L3", "D0L3R3",
    "D1L3L1", "D1L3R1",
    "D1L3L2", "D1L3R2",
    "D1L3L3", "D1L3R3" },
  { "D0L4L1", "D0L4R1",
    "D0L4L2", "D0L4R2",
    "D0L4L3", "D0L4R3",
    "D1L4L1", "D1L4R1",
    "D1L4L2", "D1L4R2",
    "D1L4L3", "D1L4R3" },
};

// led rgb colors for each layer and key (led index)
const byte defaultLEDColors[MAX_LAYER][15][3] PROGMEM = {
  {
    { 0, 0, 0 },  //red, green, blue
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


// ============ LIBS =============
#include <avr/pgmspace.h>
// #include <MemoryFree.h>  //TODO remove?
#include <Keyboard.h>
#include <Keyboard_de_DE.h>
#include <KeyboardLayout.h>
#include <Adafruit_NeoPixel.h>
#include <U8g2lib.h>

#include "Debug.h"
#include "Basics.h"
#include "Key.h"

/**
 * Handles the key 1 actions for each layer.
 */
void keyOnePressed(Key *key) {
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
  }
}

/**
 * Handles the key 2 actions for each layer.
 */
void keyTwoPressed(Key *key) {
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
  }
}

/**
 * Handles the key 3 actions for each layer
 */
void keyThreePressed(Key *key) {
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
  }
}

/**
 * Handles the key 4 actions for each layer
 */
void keyFourPressed(Key *key) {
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
  }
}

/**
 * Handles the key 5 actions for each layer
 */
void keyFivePressed(Key *key) {
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
  }
}

/**
 * Handles the key 6 actions for each layer
 */
void keySixPressed(Key *key) {
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
  }
}

/**
 * Handles the key 7 actions for each layer
 */
void keySevenPressed(Key *key) {
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
  }
}

/**
 * Handles the key 8 actions for each layer
 */
void keyEightPressed(Key *key) {
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
  }
}

/**
 * Handles the key 9 actions for each layer
 */
void keyNinePressed(Key *key) {
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
  }
}

/**
 * Handles the key 10 actions for each layer
 */
void keyTenPressed(Key *key) {
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
  }
}

/**
 * Handles the key 11 actions for each layer
 */
void keyElevenPressed(Key *key) {
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
  }
}

/**
 * Handles the key 12 actions for each layer
 */
void keyTwelvePressed(Key *key) {
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
  }
}
