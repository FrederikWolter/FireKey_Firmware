/*********************************************************************
* FIREKEY - PROJECT
* 
* Code snippet to test the key metric hardware component.
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
  Serial.print("pressed:");
  readMatrix();
  Serial.println("");
  delay(100);
}


void keyPressed(int rowIdx, int colIdx) {
  Serial.print("R");
  Serial.print(rowIdx);
  Serial.print("C");
  Serial.print(colIdx);
  Serial.print(" ");
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