/*********************************************************************
* FIREKEY - PROJECT
* 
* Code snippet to test debouncing and key holds
* 
* Requried libs:
* - None
*********************************************************************/

// CONSTANTS
#define ROW_COUNT 2  // number of rows
#define COL_COUNT 3  // number of columns

// PINS
byte rows[ROW_COUNT] = { 5, 6 };        // define the row pins
byte cols[COL_COUNT] = { 10, 16, 14 };  // define the column pins

byte keyDownCounter[ROW_COUNT][COL_COUNT];
byte keySpamMode[ROW_COUNT][COL_COUNT];

byte spamSpeed = 15;
byte holdDelay = 350;
byte debounceTime = 10;
unsigned long startTime;

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
  if ((millis() - startTime) > debounceTime) {
    readMatrix();
    startTime = millis();
  }
}


void keyPressed(int rowIdx, int colIdx) {
  if (keyDownCounter[rowIdx][colIdx] == 0) {
    Serial.print("R");
    Serial.print(rowIdx);
    Serial.print("C");
    Serial.print(colIdx);
    Serial.println(" ");
  } else if (keySpamMode[rowIdx][colIdx] && keyDownCounter[rowIdx][colIdx] > spamSpeed) {
    Serial.print("R");
    Serial.print(rowIdx);
    Serial.print("C");
    Serial.print(colIdx);
    Serial.println(" ");
    keyDownCounter[rowIdx][colIdx] = 0;
  } else if (keyDownCounter[rowIdx][colIdx] > holdDelay) {
    Serial.println("long");
    keySpamMode[rowIdx][colIdx] = 1;
  }
  keyDownCounter[rowIdx][colIdx]++;
}

void resetKey(byte rowIndex, byte colIndex) {
  keySpamMode[rowIndex][colIndex] = 0;
  keyDownCounter[rowIndex][colIndex] = 0;
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
      } else if (keyDownCounter[rowIndex][colIndex] != 0) {
        resetKey(rowIndex, colIndex);
      }
    }

    digitalWrite(curRow, HIGH);
  }
}