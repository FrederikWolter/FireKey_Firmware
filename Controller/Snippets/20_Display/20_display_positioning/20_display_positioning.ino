/*********************************************************************
* FIREKEY - PROJECT
*
* Code snippet which displays the button metric functions
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
  Serial.println("Starting ...");

  display.begin(SH1106_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  Serial.println("display started");

  displayShowMetricFuncs();
}


void loop() {
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

void displayShowMetricFuncs() {
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