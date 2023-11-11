#include <Arduino.h>
#include <SPI.h>
#include <U8g2lib.h>

/* Constructor */
U8G2_SSD1306_128X64_NONAME_F_4W_HW_SPI u8g2(
  U8G2_R0,
  /* cs=*/ 7,
  /* dc=*/ A0,
  /* reset=*/ 8);

uint8_t *buf;

void setup(void) {
  buf = (uint8_t *)malloc(u8g2.getBufferSize());
  u8g2.setBufferPtr(buf);
  u8g2.initDisplay();
  u8g2.clearDisplay();
  u8g2.setPowerSave(0);
}

/* draw something on the display with the `firstPage()`/`nextPage()` loop*/
void loop(void) {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawStr(0,20,"Hello World!");
  } while ( u8g2.nextPage() );
  delay(100);
}