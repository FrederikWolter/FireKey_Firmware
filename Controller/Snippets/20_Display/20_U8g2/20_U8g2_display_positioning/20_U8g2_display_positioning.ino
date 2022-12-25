/*********************************************************************
* FIREKEY - PROJECT
*
* Code snippet which displays the button metric functions
*
* Required Libs:
* - U8g2 Lib(https://github.com/olikraus/u8g2)
*    -> can be installed via Arduino IDE
*********************************************************************/

// LIBRARIES
#include <U8g2lib.h>
#include <MemoryFree.h>

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
  HLINE1 = 14,
  ROW1 = 16,
  ROW2 = 28,
  ROW3 = 40,
  ROW4 = 52,
  BOTTOM = 63
} yPosition;

// OLED object
U8G2_SH1106_128X64_NONAME_1_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);

void setup() {
  Serial.begin(9600);
  Serial.println("Starting ...");

  display.setFont(u8g2_font_6x10_tr);
  //display.setI2CAddress(OLED_ADDR);
  display.begin();
  Serial.println("display started");
}


void loop() {
  display.firstPage();
  do {
    displayShowMetricFuncs();
  } while (display.nextPage());
  Serial.println(freeMemory());
  delay(1000);
}

void drawStringAtPosition(const char *buf, xPosition xPosition, yPosition yPosition) {
  int h = display.getFontAscent() - display.getFontDescent();
  int w = display.getStrWidth(buf);

  if (xPosition == LEFT) {
    display.setCursor(LEFT, yPosition + h);
  } else if (xPosition == CENTER) {
    int xVal = CENTER - w / 2;
    display.setCursor(xVal, yPosition + h);
  } else if (xPosition == RIGHT) {
    int xVal = RIGHT - w;
    display.setCursor(xVal, yPosition + h);
  }
  display.print(buf);
}

void displayShowMetricFuncs() {
  drawStringAtPosition("Prev", LEFT, ROW0);
  drawStringAtPosition("Home", CENTER, ROW0);
  drawStringAtPosition("Next", RIGHT, ROW0);

  display.drawLine(LEFT, HLINE1, RIGHT, HLINE1);
  display.drawLine(VLINE1, TOP, VLINE1, BOTTOM);
  display.drawLine(VLINE2, TOP, VLINE2, BOTTOM);

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
}