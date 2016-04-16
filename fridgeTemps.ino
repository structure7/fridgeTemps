#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>


#include <WidgetRTC.h>
#include <Time.h>
#include <TimeLib.h>
#include <SimpleTimer.h>

char auth[] = "fromBlynkApp";
SimpleTimer timer;

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

#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 12 // NodeMCU D6, ESP-12E GPIO12 (WORKS!)

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

DeviceAddress ds18b20Fridge = { 0x28, 0x77, 0x8A, 0x1E, 0x00, 0x00, 0x80, 0xD4 }; // Refridgerator
DeviceAddress ds18b20Freezer = { 0x28, 0xBE, 0x89, 0x1E, 0x00, 0x00, 0x80, 0x93 }; // Freezer

int tempFridge, tempFreezer;

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, "ssid", "pw");
  Wire.begin(4, 5); // Set pins for SDA/SCL (4=D2 for SDA and 5=D1 for SCL)

  sensors.begin();
  sensors.setResolution(ds18b20Fridge, 9);
  sensors.setResolution(ds18b20Freezer, 9);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.clearDisplay();

  timer.setInterval(2000L, sendTemps); // Temperature sensor polling interval
}

void loop()
{
  Blynk.run();
  timer.run();

  sensors.requestTemperatures(); // Polls the sensors
  tempFridge = sensors.getTempF(ds18b20Fridge);
  tempFreezer = sensors.getTempF(ds18b20Freezer);

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
  display.display();
}

void sendTemps()
{
  Blynk.virtualWrite(0, tempFreezer);
  Blynk.virtualWrite(1, tempFridge);
}
