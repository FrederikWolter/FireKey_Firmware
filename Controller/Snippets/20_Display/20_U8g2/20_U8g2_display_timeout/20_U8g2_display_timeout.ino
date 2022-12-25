/*********************************************************************
* FIREKEY - PROJECT
*
* Code snippet to test the oled display standby functionality
*
* Required Libs:
* - U8g2 Lib(https://github.com/olikraus/u8g2)
*    -> can be installed via Arduino IDE
*********************************************************************/

// LIBRARIES
#include <U8g2lib.h>

// CONSTANTS
#define OLED_ADDR 0x3C
#define SLEEP_DELAY_SECONDS 5  // delay in seconds before the display is going to sleep
#define ROW_COUNT 2            // number of rows
#define COL_COUNT 3            // number of columns

// PINS
#define OLED_RESET -1                   // use no extra reset pin
byte rows[ROW_COUNT] = { 5, 6 };        // define the row pins
byte cols[COL_COUNT] = { 10, 16, 14 };  // define the column pins

// OLED object
U8G2_SH1106_128X64_NONAME_1_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);

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

  display.setFont(u8g2_font_6x10_tr);
  //display.setI2CAddress(OLED_ADDR);
  display.begin();

  Serial.println("display started");
}

void sleepDisplay() {
  if (sleeping == 0) {
    sleeping = 1;
    Serial.println("Display is going to sleep...");
    display.sleepOn();
  }
}

void wakeDisplay() {
  if (sleeping != 0) {
    sleeping = 0;
    Serial.println("Waking up display...");
    display.sleepOff();
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
      }
    }

    // pull output row high again
    digitalWrite(curRow, HIGH);
  }
}

void keyPressed(byte rowIdx, byte colIdx) {
  lastPress = millis();
  Serial.println("Reset timeout");

  wakeDisplay();
}

void loop() {
  if (millis() - lastPress > 10) {
    readMatrix();
  }

  if (sleeping == 0) {
    display.firstPage();
    do {
      display.setCursor(0, 20);
      display.print(F("Hello World!"));
    } while (display.nextPage());
  }

  if (millis() - lastPress > SLEEP_DELAY_SECONDS * 1000) {
    sleepDisplay();
  }
}