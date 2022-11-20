/*********************************************************************
* FIREKEY - PROJECT
*
* Code snippet to test the oled display standby functionality
*
* Required Libs:
* - Adafruit SH1106 Lib(https://github.com/wonho-maker/Adafruit_SH1106)
*    -> is located in the 00_Lib folder
* - Adafruit GFX Lib(https://github.com/adafruit/Adafruit-GFX-Library)
*    -> can be installed via Arduino IDE
*********************************************************************/

// LIBRARIES
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>

// CONSTANTS
#define OLED_ADDR 0x3C
#define SLEEP_DELAY_SECONDS 5  // delay in seconds befor the display is going to sleep
#define ROW_COUNT 2            // number of rows
#define COL_COUNT 3            // number of columns

// PINS
#define OLED_RESET -1                   // use no extra reset pin
byte rows[ROW_COUNT] = { 5, 6 };        // define the row pins
byte cols[COL_COUNT] = { 10, 16, 14 };  // define the column pins

// OLED object
Adafruit_SH1106 display(OLED_RESET);

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

  display.begin(SH1106_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();

  Serial.println("display started");
}

void sleepDisplay() {
  if (sleeping == 0) {
    sleeping = 1;
    Serial.println("Display is going to sleep...");
    display.SH1106_command(SH1106_DISPLAYOFF);
  }
}

void wakeDisplay() {
  if (sleeping != 0) {
    sleeping = 0;
    Serial.println("Waking up display...");
    display.SH1106_command(SH1106_DISPLAYON);
    wirteToDisplay();
  }
}

void wirteToDisplay() {
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(25, 0);
  display.println("Hello display!");

  display.display();
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

  if (millis() - lastPress > SLEEP_DELAY_SECONDS * 1000) {
    sleepDisplay();
  }
}