/**
* Test snippet to test the led strip hardware.
* The program iterates over each LED and switches one led more (green) on
*/

#include <Adafruit_NeoPixel.h>

// The pin of the controller to which the led strip is connected
static const int LED_PIN = 21;
// The amount of leds in the led strip
static const int NUM_LEDS = 6;

// Setup led strip lib
Adafruit_NeoPixel led_strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // INITIALIZE led strip object
  led_strip.begin();
}

void loop() {
  // Turn all leds off
  led_strip.clear();

  // For each led...
  for(int i=0; i<NUM_LEDS; i++) {
    // Set led-color to green
    led_strip.setPixelColor(i, 0, 60, 0);

    // Send the updated pixel colors to the hardware
    led_strip.show();

    // Wait 500 ms
    delay(500);
  }
}
