# fridgeTemps

What's more exciting than displaying your refrigerator and freezer temperature on a small OLED? Nothing.

Sending temperatures with two DS18B20s. On an NodeMCU (ESP-12E)... may downgrade to an ESP8266-01. OLED is "I2C interface 0.96" OLED 128x64 display module" from eBay.

![ScreenShot](http://i.imgur.com/x2MWiis.jpg)

## Libraries and Resources

Title | Include | Link | w/ IDE?
------|---------|------|----------
Adafruit_SSD1306 | Adafruit_SSD1306.h | https://github.com/adafruit/Adafruit_SSD1306 | No
Adafruit-GFX-Library | Adafruit_GFX.h | https://github.com/adafruit/Adafruit-GFX-Library | No
Wire | Wire.h | https://github.com/esp8266/Arduino | NO!‡
SPI | SPI.h | https://github.com/esp8266/Arduino | NO!‡
OneWire | OneWire.h | https://github.com/PaulStoffregen/OneWire | No
Arduino-Temperature-Control-Library | DallasTemperature.h | https://github.com/milesburton/Arduino-Temperature-Control-Library | No
‡See lessons learned below.

Many thanks to all of the people above. [How to edit this.](https://guides.github.com/features/mastering-markdown/)

## Lessons Learned
###NodeMCU's ESP-12E versus the I2C OLED (w/ Arduino... no Lua)
I purchased a pretty plain OLED display (http://www.ebay.com/itm/130566448551?_trksid=p2057872.m2749.l2649&ssPageName=STRK%3AMEBIDX%3AIT) advertised as "I2C 0.96" OLED display module (compatible Arduino)" and further described as "I2C interface 0.96" OLED 128x64 display module." In the eBay listing there was a "source code" link and it mentioned it was tested and 100% compatible with Adafruit's code.

It in fact was. I attached the display to the SDA/SCL pins on my Arduino Uno and she worked great! That is, after I:
* Changed the I2C address from 0x3D to 0x3C. Therefore `display.begin(SSD1306_SWITCHCAPVCC, 0x3C);`
* As notified during my first unsuccessful compile, changed the comment in line 69-70 of Adafruit_SSD1306.h (`#define SSD1306_128_64`) to include my OLED dimensions.

Almost a happy ending. Using the exact same code from the Uno for my ESP-12E, I couldn't get anything to display on the OLED. I then learned that unlike the Uno, the ESP8266 family (of which the NodeMCU/ESP-12E belongs) doesn't have defined SDA/SCL. Therefore, lesson one:

* Add `Wire.begin(4, 5);` as my first line in void setup()

This was a little trick because the ESP-12E pins are massively messed up. To summarize, the ESP8266 chip on the board has GPIOs, but these GPIO numbers don't correspond to the ESP-12E pins. GPIO1 ≠ D1, GPIO5 ≠ D5, and this is true for every GPIO. It's a mix-up. So in my `Wire.begin(4, 5);` 4 is my SDA (that's GPIO4, but pin D1), and 5 is my SCL (that's GPIO5, but pin D2). Lordy.

Excited to figure this out I compiled and got an error that Wire.begin wasn't a thing in Wire.h. Gah. Lesson #2:

* If you're switching between two different "groups" of boards in Arduino IDE's board manager (such as Arduino standard, and ESP8266-family), each one of those might have it's own includes that are named exactly the same, but are actually different.

So get this: `Wire.begin(4, 5);` wouldn't compile... said it didn't exist in Wire.h. In the IDE I went to Tools -> Board, and changed from Uno to ESP-12E. Then I deleted `#include <Wire.h>`, went to Sketch -> Include Library, and selected Wire again. `#include <Wire.h>` pasted itself into the code. Doesn't seem any different, eh? It was! It is! This time it pulled the Wire.h from the ESP8266/Arduino library. Hit compile and it worked! Whheee!

But the OLED still didn't. I just needed to learn to read.

I was hell-bent (especially after seeing other peoples' glowing, pride-filled examples) to make the "stock" Adafruit library work with this OLED on my ESP-12E. After not too much more cursing I learned lesson #4:

* Change `#define OLED_RESET 4` to `#define OLED_RESET 0`, or some number so it's not the same as whatever SDA/SLC you've picked!

This was just bad luck that the SDA/SLC pins I chose were defined as a reset in Adafruit's library, but after changing it from 4 to 0, the OLED at last lit up on my ESP-12E.
