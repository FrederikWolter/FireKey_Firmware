/*********************************************************************
* FIREKEY - PROJECT
*
* Code snippet to test the led standby (like the display)
*
* Required Libs:
* - Adafruit NeoPixel (https://github.com/adafruit/Adafruit_NeoPixel) 
*    -> can be installed via Arduino IDE
*********************************************************************/

// LIBRARIES
#include <MemoryFree.h>
#include <Adafruit_NeoPixel.h>

// CONSTANTS
#define SLEEP_DELAY_SECONDS 5  // delay in seconds before the display is going to sleep
#define ROW_COUNT 2            // number of rows
#define COL_COUNT 3            // number of columns
#define NUM_LEDS 6             // number of LEDs in the strip

// PINS
#define OLED_RESET -1                   // use no extra reset pin
#define LED_PIN 21                      // A3 - pin connected to DIN of the LED strip
byte rows[ROW_COUNT] = { 5, 6 };        // define the row pins
byte cols[COL_COUNT] = { 10, 16, 14 };  // define the column pins

// LED strip object
Adafruit_NeoPixel ledStrip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// VARIABLES
long lastPress;
bool sleeping = 0;  // check if the display is sleeping


void setup() {
  Serial.begin(9600);
  Serial.println("Starting ...");

  // declare all cols as input and activate internal pullup resistor
  for (int x = 0; x < COL_COUNT; x++) {
    pinMode(cols[x], INPUT_PULLUP);
  }

  // declare all rows as output and set them high
  for (int x = 0; x < ROW_COUNT; x++) {
    pinMode(rows[x], OUTPUT);
    digitalWrite(rows[x], HIGH);
  }

  // initialize LED strip object
  ledStrip.begin();

  Serial.println("led strip started");

  lastPress = millis();
  showLED();
}

void sleepLEDStrip() {
  if (sleeping == 0) {
    sleeping = 1;
    Serial.println("Leds are going to sleep...");
    ledStrip.clear();
    ledStrip.show();
  }
}

void wakeLEDStrip() {
  if (sleeping) {
    sleeping = false;
    Serial.println("Waking up leds...");
    showLED();
  }
}

void showLED() {
  ledStrip.setPixelColor(0, 50, 50, 50);
  ledStrip.show();
}

void readMatrix() {
  // scan matrix
  for (int rowIndex = 0; rowIndex < ROW_COUNT; rowIndex++) {
    // pull output row to low
    byte curRow = rows[rowIndex];
    digitalWrite(curRow, LOW);

    // is any column pulled to low due to a button being pressed?
    for (int colIndex = 0; colIndex < COL_COUNT; colIndex++) {
      byte colRow = cols[colIndex];
      if (digitalRead(colRow) == LOW) {
        keyPressed(rowIndex, colIndex);
      }
    }

    // pull output row high again
    digitalWrite(curRow, HIGH);
  }
}

void keyPressed(byte rowIdx, byte colIdx) {
  lastPress = millis();
  Serial.println("Reset timeout");

  wakeLEDStrip();
}

void loop() {
  if (millis() - lastPress > 10) {
    readMatrix();
  }

  if (millis() - lastPress > SLEEP_DELAY_SECONDS * 1000) {
    sleepLEDStrip();
  }
}