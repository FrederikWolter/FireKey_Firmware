/*********************************************************************
* FIREKEY - PROJECT
* 
* Code snippet to test they keyboard lib with layer handling.
* 
* Required libs:
* - Keyboard(https://github.com/arduino-libraries/Keyboard)
*   -> can be installed via arduino ide (defauled installed)
* - USBHost
*   -> can be installed via arduino ide
*********************************************************************/

// LIBRARIES
#include <Keyboard.h>
#include <Keyboard_de_DE.h>
#include <KeyboardLayout.h>

// CONSTANTS
#define ROW_COUNT 2  // number of rows
#define COL_COUNT 3  // number of columns
//Layers
#define LAYER_BACK_KEY 2     // define the back button led index
#define LAYER_HOME_KEY 1     // define the home button led index
#define LAYER_FORWARD_KEY 0  // define the forward button led index
#define MAX_LAYER 5
#define HOME_LAYER 0

// Button constants defining the led index
#define KEY_1 3
#define KEY_2 4
#define KEY_3 5
#define KEY_4 8
#define KEY_5 7
#define KEY_6 6
#define KEY_7 9
#define KEY_8 10
#define KEY_9 11
#define KEY_10 14
#define KEY_11 13
#define KEY_12 12

// PINS
byte rows[ROW_COUNT] = { 5, 6 };        // define the row pins
byte cols[COL_COUNT] = { 10, 16, 14 };  // define the column pins

byte currentLayer = 0;

void setup() {
  Serial.begin(9600);

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

byte getLedIndex(byte rowIdx, byte colIdx) {
  // calculate led index out of row and col index
  byte index = rowIdx * COL_COUNT;

  if (rowIdx % 2 == 0) {  // even row?
    index += colIdx;
  } else {  // odd row?
    index += COL_COUNT - 1 - colIdx;
  }
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
      keyLayerBackPressed(LAYER_BACK_KEY);
      break;
    case LAYER_HOME_KEY:
      keyLayerHomePressed(LAYER_HOME_KEY);
      break;
    case LAYER_FORWARD_KEY:
      keyLayerForwardPressed(LAYER_FORWARD_KEY);
      break;
    case KEY_1:
      Serial.println(1);
      keyOnePressed(KEY_1);
      break;
    case KEY_2:
      Serial.println(2);
      keyTwoPressed(KEY_2);
      break;
    case KEY_3:
      Serial.println(3);
      keyThreePressed(KEY_3);
      break;
    case KEY_4:
      Serial.println(4);
      keyFourPressed(KEY_4);
      break;
    case KEY_5:
      Serial.println(5);
      keyFivePressed(KEY_5);
      break;
    case KEY_6:
      Serial.println(6);
      keySixPressed(KEY_6);
      break;
    case KEY_7:
      Serial.println(7);
      keySevenPressed(KEY_7);
      break;
    case KEY_8:
      Serial.println(8);
      keyEightPressed(KEY_8);
      break;
    case KEY_9:
      Serial.println(9);
      keyNinePressed(KEY_9);
      break;
    case KEY_10:
      Serial.println(10);
      keyTenPressed(KEY_10);
      break;
    case KEY_11:
      Serial.println(11);
      keyElevenPressed(KEY_11);
      break;
    case KEY_12:
      Serial.println(12);
      keyTwelvePressed(KEY_12);
      break;
    default:
      Serial.println("No Action");
  }
}

void keyLayerForwardPressed(byte keyLedIndex) {
  Serial.println("Layer forward");
  currentLayer = (currentLayer + 1) % MAX_LAYER;
  Serial.println(currentLayer);
}

void keyLayerHomePressed(byte keyLedIndex) {
  Serial.println("Home");
  currentLayer = HOME_LAYER;
  Serial.println(currentLayer);
}

void keyLayerBackPressed(byte keyLedIndex) {
  Serial.println("Layer back");
  currentLayer = (currentLayer - 1 + MAX_LAYER) % MAX_LAYER;
  Serial.println(currentLayer);
}

void keyOnePressed(byte keyLedIndex) {
  switch (currentLayer) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
  }
}

void keyTwoPressed(byte keyLedIndex) {
  switch (currentLayer) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
  }
}

void keyThreePressed(byte keyLedIndex) {
  switch (currentLayer) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
  }
}

void keyFourPressed(byte keyLedIndex) {
  switch (currentLayer) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
  }
}

void keyFivePressed(byte keyLedIndex) {
  switch (currentLayer) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
  }
}

void keySixPressed(byte keyLedIndex) {
  switch (currentLayer) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
  }
}

void keySevenPressed(byte keyLedIndex) {
  switch (currentLayer) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
  }
}

void keyEightPressed(byte keyLedIndex) {
  switch (currentLayer) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
  }
}

void keyNinePressed(byte keyLedIndex) {
  switch (currentLayer) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
  }
}

void keyTenPressed(byte keyLedIndex) {
  switch (currentLayer) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
  }
}

void keyElevenPressed(byte keyLedIndex) {
  switch (currentLayer) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
  }
}

void keyTwelvePressed(byte keyLedIndex) {
  switch (currentLayer) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
  }
}
