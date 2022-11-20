/*********************************************************************
* FIREKEY - PROJECT
* 
* Code snippet to test LED fading
* 
* Required libs:
* - Adafruit NeoPixel (https://github.com/adafruit/Adafruit_NeoPixel) 
*    -> can be installed via Arduino IDE
*********************************************************************/

// LIBRARIES
#include <Adafruit_NeoPixel.h>

// PINS
#define LED_PIN 21  // A3 - pin connected to DIN of the LED strip

// CONSTANTS
#define NUM_LEDS 6                 // amount of LEDs in the LED strip
#define LED_BRIGHT_UPDATE_STEP 20  // LED brightness update setp

float lastUpdate;         // stores the last millies
byte ledUpdateRate = 50;  // time between a led gets updated
int ledBrightness;        // the current brightness of the led

bool fadeIn = false;

// LED strip object
Adafruit_NeoPixel ledStrip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  // initialize LED strip object
  ledStrip.begin();
}


void loop() {
  // turn all LEDs off
  ledStrip.clear();

  if (millis() - lastUpdate > ledUpdateRate) {
    if (fadeIn) {
      ledBrightness += LED_BRIGHT_UPDATE_STEP;
      if (ledBrightness >= 255) {
        ledBrightness = 255;
        fadeIn = false;
      }
    } else {
      ledBrightness -= LED_BRIGHT_UPDATE_STEP;
      if (ledBrightness <= 0) {
        ledBrightness = 0;
        fadeIn = true;
      }
    }

    ledStrip.setPixelColor(0, 0, ledBrightness, ledBrightness);
    // send the updated pixel colors to the hardware
    ledStrip.show();
    lastUpdate = millis();
  }
}