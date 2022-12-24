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
#include <Keyboard.h>
#include <Keyboard_de_DE.h>
#include <KeyboardLayout.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include "00_Key.h"

// Begin CONSTANTS
#define ROW_COUNT 5  // number of rows
#define COL_COUNT 3  // number of columns

// Layers
#define LAYER_BACK_KEY 2     // define the back button led index
#define LAYER_HOME_KEY 1     // define the home button led index
#define LAYER_FORWARD_KEY 0  // define the forward button led index
#define MAX_LAYER 5
#define HOME_LAYER 0

// Led
#define NUM_LEDS 6  // number of LEDs in the strip
#define LED_PIN 21

// End CONSTANTS

// LED strip object
Adafruit_NeoPixel ledStrip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

Key keys[1][1];

byte getLedIndex(byte rowIdx, byte colIdx) {
  // calculate led index out of row and col index
  byte index = rowIdx * COL_COUNT;
  index += rowIdx % 2 == 0 ? colIdx : COL_COUNT - 1 - colIdx;
  return index;
}


void setup() {
  // put your setup code here, to run once:
  for (byte r = 0; r < ROW_COUNT; r++) {
    for (byte c = 0; c < COL_COUNT; c++) {
      keys[r][c] = Key(r, c, getLedIndex(r, c), ledStrip);
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}
