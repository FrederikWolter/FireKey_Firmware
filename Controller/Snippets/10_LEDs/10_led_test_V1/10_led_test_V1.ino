/**
* FIREKEY - PROJECT
* 
* Code snippet to test the LED strip hardware component.
* The program iterates over each LED and switches one LED more (green) on.
* 
* Requried libs:
* - Adafruit NeoPixel (https://github.com/adafruit/Adafruit_NeoPixel) can be installed via Arduino IDE
*/

#include <Adafruit_NeoPixel.h>

// pin connected to DIN of the LED strip
const int LED_PIN = 21;  // A3

// amount of LEDs in the LED strip
const int NUM_LEDS = 6;

// LED brightness
const int LED_BRIGHTNEESS = 20;

// get LED strip object
Adafruit_NeoPixel ledStrip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  // initialize LED strip object
  ledStrip.begin();
}


void loop() {
  // turn all LEDs off
  ledStrip.clear();

  // for each LED ...
  for (int i = 0; i < NUM_LEDS; i++) {
    // set LED color to green
    ledStrip.setPixelColor(i, 0, LED_BRIGHTNEESS, 0);

    // send the updated pixel colors to the hardware
    ledStrip.show();

    delay(1000);
  }

  // for each LED ...
  for (int i = 0; i < NUM_LEDS; i++) {
    // set LED color to red
    ledStrip.setPixelColor(i, LED_BRIGHTNEESS, 0, 0);

    // send the updated pixel colors to the hardware
    ledStrip.show();

    delay(1000);
  }

  // for each LED ...
  for (int i = 0; i < NUM_LEDS; i++) {
    // set LED color to blue
    ledStrip.setPixelColor(i, 0, 0, LED_BRIGHTNEESS);

    // send the updated pixel colors to the hardware
    ledStrip.show();

    delay(1000);
  }

  delay(1000);
}