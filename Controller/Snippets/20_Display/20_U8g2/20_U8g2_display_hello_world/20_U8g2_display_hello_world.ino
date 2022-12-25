#include <Arduino.h>
#include <U8g2lib.h>
#include <MemoryFree.h>

U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

void setup(void) {
  u8g2.begin();
}

void loop(void) {
  u8g2.setFont(u8g2_font_5x8_tr);
  u8g2.firstPage();
  do {
    u8g2.setCursor(0, 20);
    u8g2.print(F("Hello World!"));
  } while ( u8g2.nextPage() );
  Serial.println(freeMemory());
  delay(1000);
}

