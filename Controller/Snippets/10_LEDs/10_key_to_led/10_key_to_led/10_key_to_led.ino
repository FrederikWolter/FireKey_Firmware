/*********************************************************************
* FIREKEY - PROJECT
* 
* Code snippet which enables the led of the pressed key
* 
* Requried libs:
* - Adafruit NeoPixel (https://github.com/adafruit/Adafruit_NeoPixel) 
*    -> can be installed via Arduino IDE
*********************************************************************/

// LIBRARIES
#include <Adafruit_NeoPixel.h>

// CONSTANTS
#define ROW_COUNT 2  // number of rows
#define COL_COUNT 3  // number of columns
#define NUM_LEDS 6   // amount of LEDs in the LED strip

// PINS
byte rows[ROW_COUNT] = { 5, 6 };        // define the row pins
byte cols[COL_COUNT] = { 10, 16, 14 };  // define the column pins
#define LED_PIN 21                      // A3 - pin connected to DIN of the LED strip

// LED strip object
Adafruit_NeoPixel ledStrip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);

  // initialize LED strip object
  ledStrip.begin();

  // declare all cols as input and activate internal pullup resistor
  for (int x = 0; x < COL_COUNT; x++) {
    pinMode(cols[x], INPUT_PULLUP);
  }

  // declare all rows as output and set them high
  for (int x = 0; x < ROW_COUNT; x++) {
    pinMode(rows[x], OUTPUT);
    digitalWrite(rows[x], HIGH);
  }
}

void loop() {
  readMatrix();
  delay(100);
}

void readMatrix() {
  // scan matrix
  for (int rowIndex = 0; rowIndex < ROW_COUNT; rowIndex++) {
    // row: set output to low
    byte curRow = rows[rowIndex];
    digitalWrite(curRow, LOW);

    // col: interate through the columns
    for (int colIndex = 0; colIndex < COL_COUNT; colIndex++) {
      byte colRow = cols[colIndex];
      if (digitalRead(colRow) == LOW) {
        keyPressed(rowIndex, colIndex);
      }
    }

    digitalWrite(curRow, HIGH);
  }
}

void keyPressed(byte rowIdx, byte colIdx) {
  //calculate led index out of row and col index
  byte index;

  if ((rowIdx + 1) % 2 == 0) {
    Serial.println("this");
    index = 3 * rowIdx - (colIdx - 2);
  } else {
    index = 3 * rowIdx + colIdx;
  }

  // set LED color to green
  ledStrip.setPixelColor(index, 0, 20, 0);

  // send the updated pixel colors to the hardware
  ledStrip.show();
}