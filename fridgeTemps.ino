#include <SimpleTimer.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WidgetRTC.h>
#include <Time.h>
#include <TimeLib.h>
#include <Wire.h>
#include <SPI.h>
WidgetTerminal terminal(V26);

char auth[] = "fromBlynkApp";

SimpleTimer timer;

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 15
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define ONE_WIRE_BUS 0 // WeMos pin D3 w/ pull-up
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

DeviceAddress ds18b20Fridge = { 0x28, 0x77, 0x8A, 0x1E, 0x00, 0x00, 0x80, 0xD4 }; // Refridgerator
DeviceAddress ds18b20Freezer = { 0x28, 0xBE, 0x89, 0x1E, 0x00, 0x00, 0x80, 0x93 }; // Freezer

#define fridgeOpto  16 // WeMos pin D0 for fridge
#define freezeOpto  14 // WeMos pin D5 for freezer

int tempFridge, tempFreezer, timerLatch, startTime, currentTime;

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, "ssid", "pw");

  while (Blynk.connect() == false) {
    // Wait until connected
  }

  Wire.begin(4, 5); // Set pins for SDA/SCL (4=D2 for SDA and 5=D1 for SCL)

  pinMode(fridgeOpto, INPUT);
  pinMode(freezeOpto, INPUT);

  sensors.begin();
  sensors.setResolution(ds18b20Fridge, 9);
  sensors.setResolution(ds18b20Freezer, 9);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.clearDisplay();

  timer.setInterval(2000L, sendTemps); // Temperature sensor polling interval
  fridgeDoorStatus1();

  statusBlinkOn();
}

BLYNK_WRITE(V27) // App button to report uptime
{
  int pinData = param.asInt();

  if (pinData == 0)
  {
    timer.setTimeout(8000L, uptimeSend);
  }
}

void uptimeSend()  // Blinks a virtual LED in the Blynk app to show the ESP is live and reporting.
{
  float secDur = millis() / 1000;
  float minDur = secDur / 60;
  float hourDur = minDur / 60;
  terminal.println(String("Refrigerator uptime: ") + hourDur + " hours ");
  terminal.flush();
}

void doorOpenTimer()
{
  if (timerLatch = 0)
  {
    startTime = millis();
  }

  timerLatch = 1;
}

void fridgeDoorStatus1()
{
  //Serial.println(digitalRead(fridgeOpto));
  //Serial.println(digitalRead(freezeOpto));

  if (digitalRead(freezeOpto) == HIGH && digitalRead(fridgeOpto) == HIGH)
  {
    Blynk.virtualWrite(25, "<Open       Open>");
    doorOpenTimer();
  }
  else if (digitalRead(freezeOpto) == LOW && digitalRead(fridgeOpto) == LOW)
  {
    Blynk.virtualWrite(25, "<Closed  Closed>");
    timerLatch = 0;
    startTime = millis();
  }
  else if (digitalRead(freezeOpto) == HIGH && digitalRead(fridgeOpto) == LOW)
  {
    Blynk.virtualWrite(25, "<Open    Closed>");
    doorOpenTimer();
  }
  else if (digitalRead(freezeOpto) == LOW && digitalRead(fridgeOpto) == HIGH)
  {
    Blynk.virtualWrite(25, "<Closed    Open>");
    doorOpenTimer();
  }

  timer.setTimeout(2000L, fridgeDoorStatus2);

}

void fridgeDoorStatus2()
{
  //Serial.println(digitalRead(fridgeOpto));
  //Serial.println(digitalRead(freezeOpto));

  if (digitalRead(freezeOpto) == HIGH && digitalRead(fridgeOpto) == HIGH)
  {
    Blynk.virtualWrite(25, String("<          ") + currentTime + "s         >");
    doorOpenTimer();
  }
  else if (digitalRead(freezeOpto) == LOW && digitalRead(fridgeOpto) == LOW)
  {
    Blynk.virtualWrite(25, "<Closed  Closed>");
    timerLatch = 0;
    startTime = millis();
  }
  else if (digitalRead(freezeOpto) == HIGH && digitalRead(fridgeOpto) == LOW)
  {
    Blynk.virtualWrite(25, String("<") + currentTime + "s       Closed>");
    doorOpenTimer();
  }
  else if (digitalRead(freezeOpto) == LOW && digitalRead(fridgeOpto) == HIGH)
  {
    Blynk.virtualWrite(25, "<Closed    Open>");
    Blynk.virtualWrite(25, String("<Closed       ") + currentTime + "s>");
    doorOpenTimer();
  }

  timer.setTimeout(2000L, fridgeDoorStatus1);

}

void statusBlinkOn()
{
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(54, 0);
  display.println(":");
  display.setCursor(108, 0);
  display.println(" ");
  timer.setTimeout(1000L, statusBlinkOff);
  display.display();
}

void statusBlinkOff()
{
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(55, 0);
  display.println(" ");
  display.setCursor(108, 0);
  display.println(":");
  timer.setTimeout(1000L, statusBlinkOn);
  display.display();
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
  display.println("  Freezer   Refrig");
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

  if (digitalRead(freezeOpto) == HIGH || digitalRead(fridgeOpto) == HIGH)
  {
    currentTime = (millis() - startTime) / 1000;
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 56);
    display.println(String("  DOOR OPEN ") + currentTime + " SEC");
  }
  /*
  else
  {
    display.setTextSize(1);
    display.setTextColor(BLACK);
    display.setCursor(0, 56);
    display.println(String("  DOOR OPEN ") + currentTime + " SEC");
  }
  */

  display.display();
}

void sendTemps()
{
  Blynk.virtualWrite(28, tempFreezer);
  Blynk.virtualWrite(29, tempFridge);
}
