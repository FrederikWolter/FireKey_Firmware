byte rows[] = { 5, 6 };
byte columns[] = { 10, 16, 14 };

const int rowCount = sizeof(rows) / sizeof(rows[0]);
const int colCount = sizeof(columns) / sizeof(columns[0]);

void setup() {
  Serial.begin(9600);

  for (int x = 0; x < colCount; x++) {
    pinMode(columns[x], OUTPUT);
  }

  for (int x = 0; x < rowCount; x++) {
    pinMode(rows[x], INPUT_PULLUP);
  }
}

void loop() {
  readMatrix();
}

void keyPressed(int rowIdx, int colIdx) {
  Serial.print("Row: ");
  Serial.print(rowIdx);
  Serial.print(" Col: ");  
  Serial.print(colIdx);
  Serial.println(" pressed!");
}

void readMatrix() {
  // iterate the rows
  for (int rowIndex = 0; rowIndex < rowCount; rowIndex++) {
    // row: set to output to low
    byte curRow = rows[rowIndex];
    digitalWrite(curRow, LOW);

    // col: interate through the columns
    for (int colIndex = 0; colIndex < colCount; colIndex++) {
      byte colRow = columns[colIndex];
      int keyStatus = digitalRead(colRow);
      if (keyStatus == LOW) {
        keyPressed(rowIndex, colIndex);
      }
    }
  }
}