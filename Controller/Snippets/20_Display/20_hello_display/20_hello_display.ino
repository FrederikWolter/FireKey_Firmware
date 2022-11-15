#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>

static const int OLED_RESET = 4;
Adafruit_SH1106 display(OLED_RESET);

void setup()   {
  Serial.begin(9600);
  display.begin(SH1106_SWITCHCAPVCC, 0x3D);
  display.clearDisplay();
}

void loop() {
  long runtime = millis() / 1000;
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(25, 0);
  display.println("Hello display!");
  
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.println(runtime);

  display.setTextSize(1);
  display.setTextColor(BLACK, WHITE);
  display.setCursor(20, 25);
  display.println("FireKey");
  
  display.display();
    
  display.clearDisplay();
}