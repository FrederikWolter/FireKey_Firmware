/*********************************************************************
* FIREKEY - PROJECT
* 
* Code snippet to test the LED strip hardware component.
* The program iterates over each LED and switches one LED more (green) on.
* 
* Requried libs:
* - Adafruit NeoPixel (https://github.com/adafruit/Adafruit_NeoPixel) 
*    -> can be installed via Arduino IDE
*********************************************************************/

#include <Adafruit_NeoPixel.h>

// PINS
#define LED_PIN     21   // A3 - pin connected to DIN of the LED strip

// CONSTANTS
#define NUM_LEDS     6   // amount of LEDs in the LED strip
#define LED_BRIGHT  20   // LED brightness

// LED strip object
Adafruit_NeoPixel ledStrip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  // initialize LED strip object
  ledStrip.begin();
}


void loop() {
  // turn all LEDs off
  ledStrip.clear();

  for (int i = 0; i < NUM_LEDS; i++) {
    // set LED color to green
    ledStrip.setPixelColor(i, 0, LED_BRIGHT, 0);

    // send the updated pixel colors to the hardware
    ledStrip.show();

    delay(1000);
  }

  for (int i = 0; i < NUM_LEDS; i++) {
    // set LED color to red
    ledStrip.setPixelColor(i, LED_BRIGHT, 0, 0);

    // send the updated pixel colors to the hardware
    ledStrip.show();

    delay(1000);
  }

  for (int i = 0; i < NUM_LEDS; i++) {
    // set LED color to blue
    ledStrip.setPixelColor(i, 0, 0, LED_BRIGHT);

    // send the updated pixel colors to the hardware
    ledStrip.show();

    delay(1000);
  }

  delay(1000);
}