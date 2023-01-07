/*********************************************************************
* FIREKEY - PROJECT
* 
* Code snippet to test they keyboard lib.
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
#define ROW_COUNT 2       // number of rows
#define COL_COUNT 3       // number of columns
#define DEBOUNCE_TIME 10  // Limit how often the metric is scanned.

// PINS
byte rows[ROW_COUNT] = { 5, 6 };        // define the row pins
byte cols[COL_COUNT] = { 10, 16, 14 };  // define the column pins

// Needed to not spam a key
byte keyDownCounter[ROW_COUNT][COL_COUNT];  // max 255 otherwise change to int

unsigned long startTime;  // debounce

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

  // Init keyboard lib with de layout
  Keyboard.begin(KeyboardLayout_de_DE);
}

void loop() {
  if ((millis() - startTime) > DEBOUNCE_TIME) {
    readMatrix();
    startTime = millis();
  }
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

void keyPressed(int rowIdx, int colIdx) {
  if (keyDownCounter[rowIdx][colIdx] != 0) return;

  keyDownCounter[rowIdx][colIdx]++;

  byte keyIdx = getLedIndex(rowIdx, colIdx);

  switch (keyIdx) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('c');
      Keyboard.releaseAll();
      break;
    case 4:
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('v');
      Keyboard.releaseAll();
      break;
    case 5:
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('#');
      Keyboard.releaseAll();
      break;
  }
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
      } else if (keyDownCounter[rowIndex][colIndex] != 0) {
        keyDownCounter[rowIndex][colIndex] = 0;
      }
    }

    // pull output row high again
    digitalWrite(curRow, HIGH);
  }
}