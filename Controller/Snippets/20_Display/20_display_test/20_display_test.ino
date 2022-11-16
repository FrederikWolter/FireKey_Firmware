/*********************************************************************
* FIREKEY - PROJECT
*
* Code snippet to test the oled display hardware component.
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
#define OLED_RESET -1       // use no extra reset pin

// CONSTANTS
#define OLED_ADDR 0x3C

// OLED object
Adafruit_SH1106 display(OLED_RESET);


void setup()   {
  Serial.begin(9600);
  Serial.println("Starting ...");

  display.begin(SH1106_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  Serial.println("display started");
}


void loop() {
  display.clearDisplay();
  Serial.println("frame");

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(25, 0);
  display.println("Hello display!");
  
  display.setTextSize(2);
  display.println(millis() / 1000);

  display.setTextSize(1);
  display.setTextColor(BLACK, WHITE);
  display.setCursor(20, 25);
  display.println(" FireKey ");
  
  display.display();
  delay(1000);
}