/*********************************************************************
* FIREKEY - PROJECT
* 
* Code snippet to test the key metric hardware component.
* 
* Requried libs:
* - None
*********************************************************************/

//Define the row pins
byte rows[] = { 5, 6 };

// Define the column pins
byte columns[] = { 10, 16, 14 };

// Amount of rows and columns
const int rowCount = sizeof(rows) / sizeof(rows[0]);
const int colCount = sizeof(columns) / sizeof(columns[0]);

void setup() {
  Serial.begin(9600);

  for (int x = 0; x < colCount; x++) {
    //declaring all the inputs and activating the internal pullup resistor
    pinMode(columns[x], INPUT_PULLUP);
  }

  for (int x = 0; x < rowCount; x++) {
    //Set output pins and pull them high
    pinMode(rows[x], OUTPUT);
    digitalWrite(rows[x],HIGH);
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

    digitalWrite(curRow, HIGH);
  }
}