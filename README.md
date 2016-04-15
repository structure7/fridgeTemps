# fridgeTemps

What's more exciting than displaying your refrigerator and freezer temperature on a small OLED? Nothing.

Sending temperatures with two DS18B20s. On an Arduno Uno for now... looking at ESP8266-01. OLED is "I2C interface 0.96" OLED 128x64 display module" from eBay.

![ScreenShot](http://i.imgur.com/x2MWiis.jpg)

## Libraries and Resources

Title | Include | Link | w/ IDE?
------|---------|------|----------
Adafruit_SSD1306 | Adafruit_SSD1306.h | https://github.com/adafruit/Adafruit_SSD1306 | No
Adafruit-GFX-Library | Adafruit_GFX.h | https://github.com/adafruit/Adafruit-GFX-Library | No
esp8266/Arduino | Wire.h | https://github.com/esp8266/Arduino | NO!‡
OneWire | OneWire.h | https://github.com/PaulStoffregen/OneWire | No
Arduino-Temperature-Control-Library | DallasTemperature.h | https://github.com/milesburton/Arduino-Temperature-Control-Library | No
‡See lessons learned below.

Many thanks to all of the people above. [How to edit this.](https://guides.github.com/features/mastering-markdown/)

## Lessons Learned
###HNodeMCU's ESP-12E versus the I2C OLED
I purchased a pretty plain OLED display (http://www.ebay.com/itm/130566448551?_trksid=p2057872.m2749.l2649&ssPageName=STRK%3AMEBIDX%3AIT) advertised as "I2C 0.96" OLED display module (compatible Arduino)" and further described as "I2C interface 0.96" OLED 128x64 display module." In the eBay listing there was a "source code" link and it mentioned it was tested and 100% compatible with Adafruit's code.

It in fact was. I attached the display to the SDA/SCL pins on my Arduino Uno and she worked great! That is, after I:
* Changed the I2C address from 0x3D to 0x3C. Therefore `display.begin(SSD1306_SWITCHCAPVCC, 0x3C);`
