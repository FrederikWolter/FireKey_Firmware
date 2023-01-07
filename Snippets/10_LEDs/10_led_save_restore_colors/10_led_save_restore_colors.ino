/*********************************************************************
* FIREKEY - PROJECT
*
* Code snippet to test the led rgb values save & restore
*
* Required Libs:
* - Adafruit NeoPixel (https://github.com/adafruit/Adafruit_NeoPixel) 
*    -> can be installed via Arduino IDE
*********************************************************************/
#include <Adafruit_NeoPixel.h>

// CONSTANTS
#define LED_PIN 21  // A3 - pin connected to DIN of the LED strip
#define NUM_LEDS 6  // number of LEDs in the strip

// VARIABLES
uint32_t lastLedValue[NUM_LEDS];  // holdes the saved led color values to restore the after wakeup

// LED strip object
Adafruit_NeoPixel ledStrip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  Serial.println("Starting ...");

  // initialize LED strip object
  ledStrip.begin();
  Serial.println("led strip started");
  ledStrip.setPixelColor(0, 50, 50, 50);
  ledStrip.setPixelColor(1, 50, 0, 50);
  ledStrip.setPixelColor(2, 50, 50, 0);
  ledStrip.setPixelColor(3, 0, 50, 50);
  ledStrip.setPixelColor(4, 70, 128, 50);
  ledStrip.setPixelColor(5, 80, 50, 150);
  ledStrip.show();
  saveRGBValues();
}

void loop() {
  delay(2000);
  ledStrip.clear();  //turn off the leds
  ledStrip.show();
  Serial.println("off");
  delay(2000);
  restoreRGBValues();
  Serial.println("restore");
}

/**
* Restores the saved led color values & turns the leds back on
*/
void restoreLEDValues() {
  for (int i = 0; i < NUM_LEDS; i++) {
    ledStrip.setPixelColor(i, lastLedValue[i]);
  }
  ledStrip.show();
}

/**
* Saves the current led color values
*/
void saveLEDValues() {
  for (int i = 0; i < NUM_LEDS; i++) {
    lastLedValue[i] = ledStrip.getPixelColor(i);
  }
}