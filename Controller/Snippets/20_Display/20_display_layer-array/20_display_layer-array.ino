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

char layerNames[5][7] = {
  "Layer1",
  "Layer2",
  "Layer3",
  "Layer4",
  "Layer5",
};

// Uses as second index the key index which is the led index
char layerButtonFunc[5][12][5] = {
  { "L0L1", "L0M1", "L0R1",  // name: Layer 0 Left Button Row 1; Layer 0 Middle Button Row 1; Layer 0 Middle Button Row 1
    "L0R2", "L0M2", "L0L2",
    "L0L3", "L0M3", "L0R3",
    "L0R4", "L0M4", "L0L4" },
  { "L1L1", "L1M1", "L1R1",
    "L1R2", "L1M2", "L1L2",
    "L1L3", "L1M3", "L1R3",
    "L1R4", "L1M4", "L1L4" },
  { "L2L1", "L2M1", "L2R1",
    "L2R2", "L2M2", "L2L2",
    "L2L3", "L2M3", "L2R3",
    "L2R4", "L2M4", "L2L4" },
  { "L3L1", "L3M1", "L3R1",
    "L3R2", "L3M2", "L3L2",
    "L3L3", "L3M3", "L3R3",
    "L3R4", "L3M4", "L3L4" },
  { "L4L1", "L4M1", "L4R1",
    "L4R2", "L4M2", "L4L2",
    "L4L3", "L4M3", "L4R3",
    "L4R4", "L4M4", "L4L4" },
};

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

  display.begin(SH1106_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  Serial.println("display started");

  // declare all cols as input and activate internal pullup resistor
  for (int x = 0; x < COL_COUNT; x++) {
    pinMode(cols[x], INPUT_PULLUP);
  }

  // declare all rows as output and set them high
  for (int x = 0; x < ROW_COUNT; x++) {
    pinMode(rows[x], OUTPUT);
    digitalWrite(rows[x], HIGH);
  }

  Serial.println("metirc initialized");
  setDisplayText();
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
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  drawStringAtPosition(layerNames[currentLayer], CENTER, ROW0);

  display.drawLine(LEFT, HLINE1, RIGHT, HLINE1, WHITE);
  display.drawLine(VLINE1, HLINE1, VLINE1, BOTTOM, WHITE);
  display.drawLine(VLINE2, HLINE1, VLINE2, BOTTOM, WHITE);

  drawStringAtPosition(layerButtonFunc[currentLayer][getLedIndex(1, 2) - 3], LEFT, ROW1);
  drawStringAtPosition(layerButtonFunc[currentLayer][getLedIndex(1, 1) - 3], CENTER, ROW1);
  drawStringAtPosition(layerButtonFunc[currentLayer][getLedIndex(1, 0) - 3], RIGHT, ROW1);

  drawStringAtPosition(layerButtonFunc[currentLayer][getLedIndex(2, 2) - 3], LEFT, ROW2);
  drawStringAtPosition(layerButtonFunc[currentLayer][getLedIndex(2, 1) - 3], CENTER, ROW2);
  drawStringAtPosition(layerButtonFunc[currentLayer][getLedIndex(2, 0) - 3], RIGHT, ROW2);

  drawStringAtPosition(layerButtonFunc[currentLayer][getLedIndex(3, 2) - 3], LEFT, ROW3);
  drawStringAtPosition(layerButtonFunc[currentLayer][getLedIndex(3, 1) - 3], CENTER, ROW3);
  drawStringAtPosition(layerButtonFunc[currentLayer][getLedIndex(3, 0) - 3], RIGHT, ROW3);

  drawStringAtPosition(layerButtonFunc[currentLayer][getLedIndex(4, 2) - 3], LEFT, ROW4);
  drawStringAtPosition(layerButtonFunc[currentLayer][getLedIndex(4, 1) - 3], CENTER, ROW4);
  drawStringAtPosition(layerButtonFunc[currentLayer][getLedIndex(4, 0) - 3], RIGHT, ROW4);

  display.display();
}