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
#define LAYER_BACK_COL_KEY_IDX 2     // define the back button key index
#define LAYER_HOME_COL_KEY_IDX 1     // define the home button key index
#define LAYER_FORWARD_COL_KEY_IDX 0  // define the forward button key index
#define MAX_LAYER 5
#define HOME_LAYER 0

// Buttons
#define KEY_1_IDX 3
#define KEY_2_IDX 4
#define KEY_3_IDX 5
#define KEY_4_IDX 8
#define KEY_5_IDX 7
#define KEY_6_IDX 6
#define KEY_7_IDX 9
#define KEY_8_IDX 10
#define KEY_9_IDX 11
#define KEY_10_IDX 14
#define KEY_11_IDX 13
#define KEY_12_IDX 12

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
    case LAYER_BACK_COL_KEY_IDX:
      Serial.println("Layer back");
      if (currentLayer == 0) {
        currentLayer = MAX_LAYER - 1;
      } else {
        currentLayer--;
      }
      Serial.println(currentLayer);
      break;
    case LAYER_HOME_COL_KEY_IDX:
      Serial.println("Home");
      currentLayer = HOME_LAYER;
      Serial.println(currentLayer);
      break;
    case LAYER_FORWARD_COL_KEY_IDX:
      Serial.println("Layer forward");
      currentLayer = (currentLayer + 1) % MAX_LAYER;
      Serial.println(currentLayer);
      break;
    case KEY_1_IDX:
      Serial.println(1);
      keyOnePressed();
      break;
    case KEY_2_IDX:
      Serial.println(2);
      keyTwoPressed();
      break;
    case KEY_3_IDX:
      Serial.println(3);
      keyThreePressed();
      break;
    case KEY_4_IDX:
      Serial.println(4);
      keyFourPressed();
      break;
    case KEY_5_IDX:
      Serial.println(5);
      keyFivePressed();
      break;
    case KEY_6_IDX:
      Serial.println(6);
      keySixPressed();
      break;
    case KEY_7_IDX:
      Serial.println(7);
      keySevenPressed();
      break;
    case KEY_8_IDX:
      Serial.println(8);
      keyEightPressed();
      break;
    case KEY_9_IDX:
      Serial.println(9);
      keyNinePressed();
      break;
    case KEY_10_IDX:
      Serial.println(10);
      keyTenPressed();
      break;
    case KEY_11_IDX:
      Serial.println(11);
      keyElevenPressed();
      break;
    case KEY_12_IDX:
      Serial.println(12);
      keyTwelvePressed();
      break;
    default:
      Serial.println("No Action");
  }
}

void keyOnePressed() {
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

void keyTwoPressed() {
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

void keyThreePressed() {
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

void keyFourPressed() {
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

void keyFivePressed() {
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

void keySixPressed() {
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

void keySevenPressed() {
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

void keyEightPressed() {
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

void keyNinePressed() {
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

void keyTenPressed() {
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

void keyElevenPressed() {
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

void keyTwelvePressed() {
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
