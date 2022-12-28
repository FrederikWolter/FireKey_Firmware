/*********************************************************************
* FIREKEY - PROJECT
* 
* Code snippet to test the LED strip hardware component.
* The program iterates over each LED and its color.
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
#define NUM_LEDS 6     // amount of LEDs in the LED strip
#define LED_COLOR 255  // LED color
#define LED_BRIGHT 128  // LED strip brightness (brightness = (percentage / 100) * 255)

// LED strip object
Adafruit_NeoPixel ledStrip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  // initialize LED strip object
  ledStrip.begin();
  ledStrip.setBrightness(LED_BRIGHT);
  ledStrip.show();
}


void loop() {
  // turn all LEDs off
  ledStrip.clear();

  for (int i = 0; i < NUM_LEDS; i++) {
    // set LED color to green
    ledStrip.setPixelColor(i, 0, LED_COLOR, 0);

    // send the updated pixel colors to the hardware
    ledStrip.show();
    Serial.println(ledStrip.getBrightness());
    delay(1000);
  }

  for (int i = 0; i < NUM_LEDS; i++) {
    // set LED color to red
    ledStrip.setPixelColor(i, LED_COLOR, 0, 0);

    // send the updated pixel colors to the hardware
    ledStrip.show();
    Serial.println(ledStrip.getBrightness());
    delay(1000);
  }

  for (int i = 0; i < NUM_LEDS; i++) {
    // set LED color to blue
    ledStrip.setPixelColor(i, 0, 0, LED_COLOR);

    // send the updated pixel colors to the hardware
    ledStrip.show();
    Serial.println(ledStrip.getBrightness());
    delay(1000);
  }

  delay(1000);
}