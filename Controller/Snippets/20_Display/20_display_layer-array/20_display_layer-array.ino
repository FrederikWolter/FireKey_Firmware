/*********************************************************************
* FIREKEY - PROJECT
* 
* Code snippet to test the display for multi layer usage
* 
* Required libs:
* - Adafruit SH1106 Lib(https://github.com/wonho-maker/Adafruit_SH1106)
*    -> is located in the 00_Lib folder
* - Adafruit GFX Lib(https://github.com/adafruit/Adafruit-GFX-Library)
*    -> can be installed via Arduino IDE
*********************************************************************/

// LIBRARIES
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>

// CONSTANTS
#define ROW_COUNT 2     // number of rows
#define COL_COUNT 3     // number of columns
#define OLED_RESET -1   // use no extra reset pin
#define OLED_ADDR 0x3C  //i2c address

// PINS
byte rows[ROW_COUNT] = { 5, 6 };        // define the row pins
byte cols[COL_COUNT] = { 10, 16, 14 };  // define the column pins

// LAYER
const byte layerRowIdx = 0;         // define the layer row index
const byte layerBackColIdx = 2;     // define the back button column index
const byte layerHomeColIdx = 1;     // define the home button column index
const byte layerForwardColIdx = 0;  // define the forward button column index
const byte maxLayer = 5;
const byte homeLayer = 0;

byte currentLayer = 0;

typedef enum xPos {
  LEFT = 0,
  VLINE1 = 41,
  CENTER = 64,
  VLINE2 = 85,
  RIGHT = 127
} xPosition;

typedef enum yPos {
  TOP = 0,
  ROW0 = 2,
  HLINE1 = 11,
  ROW1 = 16,
  ROW2 = 28,
  ROW3 = 40,
  ROW4 = 52,
  BOTTOM = 63
} yPosition;

// OLED object
Adafruit_SH1106 display(OLED_RESET);

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
  delay(100);  // TODO real deboune
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
  if (rowIdx == layerRowIdx) {
    // it's the row where the layer buttons are located
    switch (colIdx) {
      case layerBackColIdx:
        Serial.println("Layer back");
        if (currentLayer == 0) {
          currentLayer = maxLayer - 1;
        } else {
          currentLayer--;
        }
        Serial.println(currentLayer);
        break;
      case layerForwardColIdx:
        Serial.println("Layer forward");
        currentLayer = (currentLayer + 1) % maxLayer;
        Serial.println(currentLayer);
        break;
      case layerHomeColIdx:
      default:
        Serial.println("Home");
        currentLayer = homeLayer;
        Serial.println(currentLayer);
    }
    setDisplayText();
  }
}

void drawStringAtPosition(const char *buf, xPosition xPosition, yPosition yPosition) {
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(buf, xPosition, yPosition, &x1, &y1, &w, &h);  //calc width of new string

  if (xPosition == LEFT) {
    display.setCursor(LEFT, yPosition);
  } else if (xPosition == CENTER) {
    int xVal = CENTER - w / 2;
    display.setCursor(xVal, yPosition);
  } else if (xPosition == RIGHT) {
    int xVal = RIGHT - w;
    display.setCursor(xVal, yPosition);
  }
  display.print(buf);
}

void setDisplayText() {
  display.setTextSize(1);
  display.setTextColor(WHITE);

  drawStringAtPosition("Prev", LEFT, ROW0);
  drawStringAtPosition("Home", CENTER, ROW0);
  drawStringAtPosition("Next", RIGHT, ROW0);

  display.drawLine(LEFT, HLINE1, RIGHT, HLINE1, WHITE);
  display.drawLine(VLINE1, TOP, VLINE1, BOTTOM, WHITE);
  display.drawLine(VLINE2, TOP, VLINE2, BOTTOM, WHITE);

  drawStringAtPosition("Action", LEFT, ROW1);
  drawStringAtPosition("Action", CENTER, ROW1);
  drawStringAtPosition("Action", RIGHT, ROW1);

  drawStringAtPosition("Action", LEFT, ROW2);
  drawStringAtPosition("Action", CENTER, ROW2);
  drawStringAtPosition("Action", RIGHT, ROW2);

  drawStringAtPosition("Action", LEFT, ROW3);
  drawStringAtPosition("Action", CENTER, ROW3);
  drawStringAtPosition("Action", RIGHT, ROW3);

  drawStringAtPosition("Action", LEFT, ROW4);
  drawStringAtPosition("Action", CENTER, ROW4);
  drawStringAtPosition("Action", RIGHT, ROW4);

  display.display();
}