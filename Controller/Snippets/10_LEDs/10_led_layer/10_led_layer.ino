/*********************************************************************
* FIREKEY - PROJECT
* 
* Code snippet to test the key metric layer idea
* 
* Required libs:
* - Adafruit NeoPixel (https://github.com/adafruit/Adafruit_NeoPixel) 
*    -> can be installed via Arduino IDE
*********************************************************************/

#include <MemoryFree.h>
#include <Adafruit_NeoPixel.h>

// CONSTANTS
#define ROW_COUNT 2  // number of rows
#define COL_COUNT 3  // number of columns
#define NUM_LEDS 6   // number of LEDs in the strip

//Layers
#define LAYER_BACK_KEY 2     // define the back button key index
#define LAYER_HOME_KEY 1     // define the home button key index
#define LAYER_FORWARD_KEY 0  // define the forward button key index
#define MAX_LAYER 5
#define HOME_LAYER 0

// PINS
#define LED_PIN 21                      // A3 - pin connected to DIN of the LED strip
byte rows[ROW_COUNT] = { 5, 6 };        // define the row pins
byte cols[COL_COUNT] = { 10, 16, 14 };  // define the column pins

byte currentLayer = 0;

const byte layerRGB[MAX_LAYER][15][3] = {
  {
    { 0, 50, 0 },
    { 0, 50, 0 },
    { 0, 50, 0 },
    { 0, 50, 0 },
    { 0, 50, 0 },
    { 0, 50, 0 },
    { 0, 50, 0 },
    { 0, 50, 0 },
    { 0, 50, 0 },
    { 0, 50, 0 },
    { 0, 50, 0 },
    { 0, 50, 0 },
    { 0, 50, 0 },
    { 0, 50, 0 },
    { 0, 50, 0 },
  },
  {
    { 50, 0, 0 },
    { 50, 0, 0 },
    { 50, 0, 0 },
    { 50, 0, 0 },
    { 50, 0, 0 },
    { 50, 0, 0 },
    { 50, 0, 0 },
    { 50, 0, 0 },
    { 50, 0, 0 },
    { 50, 0, 0 },
    { 50, 0, 0 },
    { 50, 0, 0 },
    { 50, 0, 0 },
    { 50, 0, 0 },
    { 50, 0, 0 },
  },
  {
    { 0, 0, 50 },
    { 0, 0, 50 },
    { 0, 0, 50 },
    { 0, 0, 50 },
    { 0, 0, 50 },
    { 0, 0, 50 },
    { 0, 0, 50 },
    { 0, 0, 50 },
    { 0, 0, 50 },
    { 0, 0, 50 },
    { 0, 0, 50 },
    { 0, 0, 50 },
    { 0, 0, 50 },
    { 0, 0, 50 },
    { 0, 0, 50 },
  },
  {
    { 0, 50, 50 },
    { 0, 50, 50 },
    { 0, 50, 50 },
    { 0, 50, 50 },
    { 0, 50, 50 },
    { 0, 50, 50 },
    { 0, 50, 50 },
    { 0, 50, 50 },
    { 0, 50, 50 },
    { 0, 50, 50 },
    { 0, 50, 50 },
    { 0, 50, 50 },
    { 0, 50, 50 },
    { 0, 50, 50 },
    { 0, 50, 50 },
  },
  {
    { 50, 0, 50 },
    { 50, 0, 50 },
    { 50, 0, 50 },
    { 50, 0, 50 },
    { 50, 0, 50 },
    { 50, 0, 50 },
    { 50, 0, 50 },
    { 50, 0, 50 },
    { 50, 0, 50 },
    { 50, 0, 50 },
    { 50, 0, 50 },
    { 50, 0, 50 },
    { 50, 0, 50 },
    { 50, 0, 50 },
    { 50, 0, 50 },
  }
};

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

  setLayerRGB();
}

void loop() {
  //Serial.println(freeMemory());
  readMatrix();
  delay(100);
}

byte getLedIndex(byte rowIdx, byte colIdx) {
  // calculate led index out of row and col index
  byte index = rowIdx * COL_COUNT;
  index += rowIdx % 2 == 0 ? colIdx : COL_COUNT - 1 - colIdx;
  return index;
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

  byte keyIdx = getLedIndex(rowIdx, colIdx);

  switch (keyIdx) {
    case LAYER_BACK_KEY:
      currentLayer = (currentLayer - 1 + MAX_LAYER) % MAX_LAYER;
      break;
    case LAYER_HOME_KEY:
      currentLayer = HOME_LAYER;
      break;
    case LAYER_FORWARD_KEY:
      currentLayer = (currentLayer + 1) % MAX_LAYER;
      break;
    default:
      break;
  }
  setLayerRGB();
}

void setLayerRGB() {
  for (byte k = 0; k < NUM_LEDS; k++) {
    ledStrip.setPixelColor(k, layerRGB[currentLayer][k][0], layerRGB[currentLayer][k][1], layerRGB[currentLayer][k][2]);
  }
  ledStrip.show();
}