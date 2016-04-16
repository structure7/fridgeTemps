#include <Wire.h>
#include <SPI.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 0
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

const unsigned char kenmoreLogo [] PROGMEM =
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x3f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfc,
  0x7f, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xf0, 0x00, 0x00, 0x3f, 0xfe,
  0x7f, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x1f, 0xfe,
  0x7f, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x1f, 0xfe,
  0x7f, 0xff, 0xff, 0xff, 0x80, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x1f, 0xfe,
  0x7f, 0xff, 0xff, 0xfe, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x1f, 0xfe,
  0x7f, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x3f, 0xfe,
  0x7f, 0xff, 0xff, 0xc0, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x7f, 0xfe,
  0x7f, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xff, 0xfe,
  0x7f, 0xff, 0xfc, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc1, 0xff, 0xfe,
  0x7f, 0xff, 0xf0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x07, 0xff, 0xfe,
  0x7f, 0xff, 0xc0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xfe,
  0x7f, 0xff, 0x07, 0xc1, 0xc1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xfe,
  0x7f, 0xfc, 0x3f, 0x83, 0x83, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe,
  0x7f, 0xf8, 0xff, 0x83, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe,
  0x7f, 0xe3, 0xff, 0x82, 0x0f, 0xc0, 0xf8, 0x20, 0xf0, 0x03, 0x07, 0xe0, 0x3e, 0x01, 0xe0, 0x3e,
  0x7f, 0x8f, 0xff, 0x80, 0x1f, 0x00, 0x38, 0x00, 0x70, 0x00, 0x03, 0x80, 0x1c, 0x01, 0xc0, 0x1e,
  0x7f, 0x3f, 0xff, 0x00, 0x3e, 0x1c, 0x18, 0x10, 0x60, 0x40, 0x83, 0x06, 0x0c, 0x01, 0x07, 0x0e,
  0x7e, 0xff, 0xff, 0x00, 0x7c, 0x1c, 0x10, 0x70, 0x60, 0xc1, 0x82, 0x0e, 0x0c, 0x1f, 0x0f, 0x06,
  0x7f, 0xff, 0xff, 0x00, 0x3c, 0x00, 0x10, 0x70, 0x60, 0xc3, 0x06, 0x1e, 0x0c, 0x3e, 0x00, 0x06,
  0x7f, 0xff, 0xff, 0x08, 0x18, 0x00, 0x30, 0x70, 0xe1, 0x83, 0x04, 0x1e, 0x08, 0x3e, 0x00, 0x0e,
  0x7f, 0xff, 0xfe, 0x0c, 0x18, 0x3f, 0xf0, 0xe0, 0xc1, 0x83, 0x04, 0x1e, 0x18, 0x3e, 0x1f, 0xfe,
  0x7f, 0xff, 0xfe, 0x0e, 0x08, 0x38, 0x60, 0xe0, 0xc1, 0x83, 0x0e, 0x1c, 0x18, 0x7e, 0x0c, 0x1e,
  0x7f, 0xff, 0xfe, 0x0e, 0x04, 0x00, 0x60, 0xe0, 0xc1, 0x06, 0x0e, 0x00, 0x30, 0x7f, 0x00, 0x3e,
  0x7f, 0xff, 0xfe, 0x1f, 0x06, 0x01, 0xe0, 0xe1, 0xc3, 0x06, 0x0f, 0x80, 0xf0, 0x7f, 0x80, 0xfe,
  0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe,
  0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe,
  0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char arduinoLogo [] PROGMEM =
{ 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x07, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x80, 0x01, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x03, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x03, 0xff, 0xe0, 0x07, 0xff, 0xe0, 0x07, 0xff, 0xe0, 0x07, 0xff, 0xc0, 0x00, 0x00,
  0x00, 0x00, 0x07, 0xff, 0x80, 0x00, 0xff, 0xf0, 0x0f, 0xff, 0x00, 0x01, 0xff, 0xe0, 0x00, 0x00,
  0x00, 0x00, 0x0f, 0xfe, 0x00, 0x00, 0x7f, 0xf8, 0x1f, 0xfc, 0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00,
  0x00, 0x00, 0x0f, 0xfc, 0x00, 0x00, 0x1f, 0xfc, 0x3f, 0xf8, 0x00, 0x00, 0x3f, 0xf0, 0x00, 0x00,
  0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x0f, 0xfc, 0x7f, 0xf0, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00,
  0x00, 0x00, 0x1f, 0xf0, 0x00, 0x00, 0x07, 0xfe, 0x7f, 0xe0, 0x00, 0x00, 0x0f, 0xf8, 0x00, 0x00,
  0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00, 0x03, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x07, 0xf8, 0x00, 0x00,
  0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x07, 0x80, 0x07, 0xfc, 0x00, 0x00,
  0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x07, 0x80, 0x03, 0xfc, 0x00, 0x00,
  0x00, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x07, 0x80, 0x03, 0xfc, 0x00, 0x00,
  0x00, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x07, 0x80, 0x01, 0xfe, 0x00, 0x00,
  0x00, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x7f, 0xf8, 0x01, 0xfe, 0x00, 0x00,
  0x00, 0x00, 0x7f, 0x80, 0x7f, 0xff, 0x00, 0x3f, 0xf8, 0x00, 0x7f, 0xf8, 0x01, 0xfe, 0x00, 0x00,
  0x00, 0x00, 0x7f, 0x80, 0x7f, 0xff, 0x00, 0x1f, 0xf8, 0x00, 0x7f, 0xf8, 0x01, 0xfe, 0x00, 0x00,
  0x00, 0x00, 0x7f, 0x80, 0x7f, 0xff, 0x00, 0x1f, 0xf8, 0x00, 0x7f, 0xf8, 0x01, 0xfe, 0x00, 0x00,
  0x00, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x00, 0x3f, 0xf8, 0x00, 0x7f, 0xf8, 0x01, 0xfe, 0x00, 0x00,
  0x00, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x07, 0x80, 0x01, 0xfe, 0x00, 0x00,
  0x00, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x07, 0x80, 0x01, 0xfe, 0x00, 0x00,
  0x00, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x07, 0x80, 0x01, 0xfc, 0x00, 0x00,
  0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x07, 0x80, 0x03, 0xfc, 0x00, 0x00,
  0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x03, 0xfc, 0x00, 0x00,
  0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00, 0x03, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x07, 0xfc, 0x00, 0x00,
  0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00, 0x07, 0xfe, 0x7f, 0xc0, 0x00, 0x00, 0x0f, 0xf8, 0x00, 0x00,
  0x00, 0x00, 0x1f, 0xf0, 0x00, 0x00, 0x0f, 0xfc, 0x7f, 0xe0, 0x00, 0x00, 0x0f, 0xf8, 0x00, 0x00,
  0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x1f, 0xfc, 0x3f, 0xf0, 0x00, 0x00, 0x1f, 0xf0, 0x00, 0x00,
  0x00, 0x00, 0x0f, 0xfc, 0x00, 0x00, 0x3f, 0xf8, 0x1f, 0xfc, 0x00, 0x00, 0x7f, 0xf0, 0x00, 0x00,
  0x00, 0x00, 0x07, 0xff, 0x00, 0x00, 0x7f, 0xf0, 0x0f, 0xfe, 0x00, 0x00, 0xff, 0xe0, 0x00, 0x00,
  0x00, 0x00, 0x07, 0xff, 0xc0, 0x01, 0xff, 0xe0, 0x0f, 0xff, 0x80, 0x03, 0xff, 0xc0, 0x00, 0x00,
  0x00, 0x00, 0x03, 0xff, 0xfc, 0x1f, 0xff, 0xc0, 0x07, 0xff, 0xf8, 0x3f, 0xff, 0xc0, 0x00, 0x00,
  0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0x80, 0x03, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x03, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00,
  0x00, 0x00, 0x03, 0xe0, 0x7f, 0xc3, 0xfe, 0x1c, 0x1c, 0x7f, 0xe3, 0xc3, 0x87, 0xf8, 0x00, 0x00,
  0x00, 0x00, 0x03, 0xe0, 0x7f, 0xe3, 0xff, 0x1c, 0x1c, 0x7f, 0xe3, 0xc3, 0x8f, 0xfc, 0x00, 0x00,
  0x00, 0x00, 0x03, 0xf0, 0x71, 0xe3, 0x8f, 0x9c, 0x1c, 0x0f, 0x03, 0xe3, 0x9e, 0x3c, 0x00, 0x00,
  0x00, 0x00, 0x07, 0xf0, 0x70, 0xe3, 0x87, 0x9c, 0x1c, 0x0f, 0x03, 0xe3, 0x9c, 0x1e, 0x00, 0x00,
  0x00, 0x00, 0x07, 0x70, 0x70, 0xe3, 0x83, 0x9c, 0x1c, 0x0f, 0x03, 0xf3, 0xbc, 0x1e, 0x00, 0x00,
  0x00, 0x00, 0x07, 0x78, 0x71, 0xe3, 0x83, 0x9c, 0x1c, 0x0f, 0x03, 0xf3, 0xb8, 0x0e, 0x00, 0x00,
  0x00, 0x00, 0x0e, 0x38, 0x7f, 0xc3, 0x83, 0x9c, 0x1c, 0x0f, 0x03, 0xbb, 0xb8, 0x0e, 0x00, 0x00,
  0x00, 0x00, 0x0e, 0x38, 0x7f, 0x83, 0x83, 0x9c, 0x1c, 0x0f, 0x03, 0xbb, 0xb8, 0x0e, 0x00, 0x00,
  0x00, 0x00, 0x0e, 0x3c, 0x77, 0x83, 0x83, 0x9c, 0x1c, 0x0f, 0x03, 0x9f, 0xb8, 0x0e, 0x00, 0x00,
  0x00, 0x00, 0x0f, 0xfc, 0x73, 0xc3, 0x83, 0x9c, 0x1c, 0x0f, 0x03, 0x9f, 0xb8, 0x0e, 0x00, 0x00,
  0x00, 0x00, 0x1f, 0xfc, 0x71, 0xc3, 0x87, 0x9c, 0x1c, 0x0f, 0x03, 0x9f, 0xbc, 0x1e, 0x00, 0x00,
  0x00, 0x00, 0x1f, 0xfc, 0x71, 0xe3, 0x8f, 0x1e, 0x1c, 0x0f, 0x03, 0x8f, 0x9e, 0x3c, 0x00, 0x00,
  0x00, 0x00, 0x1c, 0x1e, 0x70, 0xe3, 0xff, 0x0f, 0xfc, 0x7f, 0xe3, 0x8f, 0x8f, 0xfc, 0x00, 0x00,
  0x00, 0x00, 0x38, 0x0e, 0x70, 0xf3, 0xfc, 0x0f, 0xf8, 0x7f, 0xe3, 0x87, 0x8f, 0xf8, 0x00, 0x00,
  0x00, 0x00, 0x38, 0x0e, 0x70, 0x73, 0xf0, 0x03, 0xe0, 0x7f, 0xe3, 0x87, 0x83, 0xe0, 0x00, 0x00
};

#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 12 // NodeMCU D6, ESP-12E GPIO12 (WORKS!)

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

DeviceAddress ds18b20Fridge = { 0x28, 0x77, 0x8A, 0x1E, 0x00, 0x00, 0x80, 0xD4 }; // Refridgerator
DeviceAddress ds18b20Freezer = { 0x28, 0xBE, 0x89, 0x1E, 0x00, 0x00, 0x80, 0x93 }; // Freezer

int doorPin = 0; // GPIO0 = Board pin D3
int timerLatch;
unsigned long openTime, openTimeStart;

void setup()
{
  Wire.begin(4, 5); // Set pins for SDA/SCL (4=D2 for SDA and 5=D1 for SCL)
  sensors.begin();
  sensors.setResolution(ds18b20Fridge, 9);
  sensors.setResolution(ds18b20Freezer, 9);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.clearDisplay();

  display.drawBitmap(0, 0, kenmoreLogo, 128, 64, WHITE);
  display.display();
  delay(750);
  display.clearDisplay();
  display.drawBitmap(0, 0, arduinoLogo, 128, 64, WHITE);
  display.display();
  delay(750);

  pinMode (doorPin, INPUT_PULLUP);
}

void loop()
{
  sensors.requestTemperatures(); // Polls the sensors
  int tempFridge = sensors.getTempF(ds18b20Fridge);
  int tempFreezer = sensors.getTempF(ds18b20Freezer);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("  Freezer:  Refrig:");
  display.setTextSize(3);
  if (tempFreezer < -9 && tempFreezer > -100) // If temp is 3-char
  {
    display.setCursor(8, 13);
    display.print(tempFreezer);
    display.setCursor(75, 13);
    display.print(tempFridge);
    display.setTextSize(1);
    display.setCursor(0, 40);
    display.println(" Temps in Fahrenheit");
  }
  else if (tempFreezer > -10 && tempFreezer < 0) // If temp is 2-char (negative temp)
  {
    display.setCursor(18, 13);
    display.print(tempFreezer);
    display.setCursor(75, 13);
    display.print(tempFridge);
    display.setTextSize(1);
    display.setCursor(0, 40);
    display.println(" Temps in Fahrenheit");
  }
  else if (tempFreezer > -1 && tempFreezer < 10) // If temp is 1-char
  {
    display.setCursor(27, 13);
    display.print(tempFreezer);
    display.setCursor(75, 13);
    display.print(tempFridge);
    display.setTextSize(1);
    display.setCursor(0, 40);
    display.println(" Temps in Fahrenheit");
  }
  else if (tempFreezer < -100 || tempFreezer > 150 || tempFridge < -100 || tempFridge > 150) // ERROR
  {
    display.setCursor(19, 13);
    display.print("ERROR");
    display.setTextSize(1);
    display.setCursor(0, 40);
    display.println("Please let Mike know!");
  }
  else // If temp is 2-char #2
  {
    display.setCursor(18, 13);
    display.print(tempFreezer);
    display.setCursor(75, 13);
    display.print(tempFridge);
    display.setTextSize(1);
    display.setCursor(0, 40);
    display.println(" Temps in Fahrenheit");
  }

  if (digitalRead(doorPin) == LOW) // Door open
  {
    if (timerLatch == 0)
    {
      openTimeStart = millis();
      timerLatch = 1;
    }
    openTime = (millis() - openTimeStart) / 1000;
    display.setCursor(0, 52);
    display.println(String("Door open ") + openTime + " seconds");
    //display.println("123456789012345678901");
  }
  else // Door closed
  {
    display.setCursor(0, 52);
    display.println("     Door closed");
    timerLatch = 0;
  }
  display.display();
}
