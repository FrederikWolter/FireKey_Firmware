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

// PINS
#define OLED_RESET -1  // use no extra reset pin

// CONSTANTS
#define OLED_ADDR 0x3C
#define SLEEP_DELAY 100  // max 255 otherwise change sleepCounter to int
#define ROW_COUNT 2      // number of rows
#define COL_COUNT 3      // number of columns

// OLED object
Adafruit_SH1106 display(OLED_RESET);

byte rows[ROW_COUNT] = { 5, 6 };        // define the row pins
byte cols[COL_COUNT] = { 10, 16, 14 };  // define the column pins

byte sleepCounter = 0;  // counter which is increased inside the loop. if this value reaches SLEEP_DELAY, the display is going to sleep

byte sleeping = 0;  // bool to check if the display is sleeping

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
    Serial.println("Display is goint to sleep...");
    display.SH1106_command(SH1106_DISPLAYOFF);
  }
}

void wakeDisplay() {
  if (sleeping != 0) {
    sleeping = 0;
    Serial.println("Waking up display...");
    display.SH1106_command(SH1106_DISPLAYON);
  }
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

void keyPressed(byte rowIdx, byte colIdx) {
  sleepCounter = 0;
  Serial.println("Reset timeout");
}

void loop() {
  readMatrix();
  if (sleepCounter == SLEEP_DELAY) {
    sleepDisplay();
  } else {
    sleepCounter++;
    wakeDisplay();
    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(25, 0);
    display.println("Hello display!");

    display.display();
  }
  delay(10);  //debounce delay
}