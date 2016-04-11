#include <Wire.h>
//#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 8

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

DeviceAddress ds18b20Fridge = { 0x28, 0x77, 0x8A, 0x1E, 0x00, 0x00, 0x80, 0xD4 }; // Refridgerator
DeviceAddress ds18b20Freezer = { 0x28, 0xBE, 0x89, 0x1E, 0x00, 0x00, 0x80, 0x93 }; // Freezer

void setup()
{
  sensors.begin();
  sensors.setResolution(ds18b20Fridge, 9);
  sensors.setResolution(ds18b20Freezer, 9);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
}

void loop()
{
  sensors.requestTemperatures(); // Polls the sensors
  int tempFridge = sensors.getTempF(ds18b20Fridge);
  int tempFreezer = sensors.getTempF(ds18b20Freezer);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(3, 9);
  display.println("  Fridge:  Freezer:");
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0, 23);
  if (tempFreezer < 0 || tempFreezer > 9)
  {
    display.print(" ");
    display.print(tempFridge);
    display.print(" ");
    display.print(tempFreezer);
  }
  else
  {
    display.print(" ");
    display.print(tempFridge);
    display.print("  ");
    display.print(tempFreezer);
  }
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 50);
  display.println(" Temps in Fahrenheit");
  display.display();
}
