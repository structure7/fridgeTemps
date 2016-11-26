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

#include <ESP8266mDNS.h>        // Required for OTA
#include <WiFiUdp.h>            // Required for OTA
#include <ArduinoOTA.h>         // Required for OTA

char auth[] = "fromBlynkApp";
char ssid[] = "ssid";
char pass[] = "pw";

char* hostSF = "raspi";
char* streamId   = "publicKey";
char* privateKey = "privateKey";

SimpleTimer timer;
WidgetRTC rtc;
BLYNK_ATTACH_WIDGET(rtc, V8);

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

DeviceAddress ds18b20Freezer = { 0x28, 0x77, 0x8A, 0x1E, 0x00, 0x00, 0x80, 0xD4 }; // Freezer
DeviceAddress ds18b20Fridge = { 0x28, 0xBE, 0x89, 0x1E, 0x00, 0x00, 0x80, 0x93 };  // Refridgerator

int tempFridge, tempFreezer, timerLatch, startTime, currentTime;

int vertAdj = 8;    // Changing this value higher moves everything on the display down.

int dailyFridgeHigh = 0;
int dailyFridgeLow = 100;
int dailyFreezerHigh = -50;
int dailyFreezerLow = 100;

bool updateLabelFlag = 0;
String currentTimeDate;        // Time formatted as "0:00AM on 0/0"
String resetTimeDate;          // When hardware was reset.

int tempSensorErrorCount = 0;

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);

  while (Blynk.connect() == false) {
    // Wait until connected
  }

  Wire.begin(4, 5); // Set pins for SDA/SCL (4 = WeMos D2 for SDA and 5 = WeMos D1 for SCL)

  sensors.begin();
  sensors.setResolution(ds18b20Fridge, 9);
  sensors.setResolution(ds18b20Freezer, 9);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.clearDisplay();

  // START OTA ROUTINE
  ArduinoOTA.setHostname("esp8266-Fridge");

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC address: ");
  Serial.println(WiFi.macAddress());
  // END OTA ROUTINE

  testdrawrect();
  display.display();
  delay(5000);
  display.clearDisplay();

  rtc.begin();

  statusBlinkOn();
  timer.setInterval(15432L, sfWebhook);            // ~15 sec run status updates to data.sparkfun.com.
  timer.setInterval(2121L, tempSensorPolling);     // ~2 sec delay between sensor polling.
  timer.setInterval(5432L, sendTemps);             // ~5 sec delay between temp data pushed to app display.
  timer.setInterval(61221L, updateAppLabel);       // Update display property (app label).
}

void updateAppLabel()
{
  Blynk.setProperty(V40, "label", String("Hi/Lo temps since ") + resetTimeDate + ". " + tempSensorErrorCount + " errors.");
}

void tempSensorPolling()
{
  sensors.requestTemperatures();                   // Polls the sensors
  tempFridge = sensors.getTempF(ds18b20Fridge);
  tempFreezer = sensors.getTempF(ds18b20Freezer);
}

void sendTemps()
{
  Blynk.virtualWrite(36, tempFreezer);
  Blynk.virtualWrite(35, tempFridge);

  if (tempFridge > dailyFridgeHigh)
  {
    dailyFridgeHigh = tempFridge;
  }

  if (tempFridge < dailyFridgeLow && tempFridge > -50)
  {
    dailyFridgeLow = tempFridge;
  }

  if (tempFreezer > dailyFreezerHigh)
  {
    dailyFreezerHigh = tempFreezer;
  }

  if (tempFreezer < dailyFreezerLow && tempFreezer > -50)
  {
    dailyFreezerLow = tempFreezer;
  }

  if (year() != 1970) // Doesn't start until RTC is set correctly.
  {
    // Below gives me leading zeros on minutes and AM/PM.
    if (minute() > 9 && hour() > 11) {
      currentTimeDate = String(hourFormat12()) + ":" + minute() + "PM on " + month() + "/" + day();
    }
    else if (minute() < 10 && hour() > 11) {
      currentTimeDate = String(hourFormat12()) + ":0" + minute() + "PM on " + month() + "/" + day();
    }
    else if (minute() > 9 && hour() < 12) {
      currentTimeDate = String(hourFormat12()) + ":" + minute() + "AM on " + month() + "/" + day();
    }
    else if (minute() < 10 && hour() < 12) {
      currentTimeDate = String(hourFormat12()) + ":0" + minute() + "AM on " + month() + "/" + day();
    }
  }

  if (updateLabelFlag == 0 && year() != 1970)
  {
    resetTimeDate = currentTimeDate;
    updateLabelFlag = 1;
  }

  Blynk.virtualWrite(40, String("Freezer: ") + dailyFreezerHigh + "/" + dailyFreezerLow + "    Fridge: " + dailyFridgeHigh + "/" + dailyFridgeLow);
}

void statusBlinkOn()
{
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(54, 0 + vertAdj);
  display.println(":");
  display.setCursor(108, 0 + vertAdj);
  display.println(" ");
  timer.setTimeout(1000L, statusBlinkOff);
  display.display();
}

void statusBlinkOff()
{
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(55, 0 + vertAdj);
  display.println(" ");
  display.setCursor(108, 0 + vertAdj);
  display.println(":");
  timer.setTimeout(1000L, statusBlinkOn);
  display.display();
}

void loop()
{
  Blynk.run();
  timer.run();
  ArduinoOTA.handle();

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0 + vertAdj);
  display.println("  Freezer   Refrig");
  display.setTextSize(3);
  if (tempFreezer < -9 && tempFreezer > -100) // If temp is 3-char
  {
    display.setCursor(8, 13 + vertAdj);
    display.print(tempFreezer);
    display.setCursor(75, 13 + vertAdj);
    display.print(tempFridge);
    display.setTextSize(1);
    display.setCursor(0, 40 + vertAdj);
    display.println(" Temps in Fahrenheit");
  }
  else if (tempFreezer > -10 && tempFreezer < 0) // If temp is 2-char (negative temp)
  {
    display.setCursor(18, 13 + vertAdj);
    display.print(tempFreezer);
    display.setCursor(75, 13 + vertAdj);
    display.print(tempFridge);
    display.setTextSize(1);
    display.setCursor(0, 40 + vertAdj);
    display.println(" Temps in Fahrenheit");
  }
  else if (tempFreezer > -1 && tempFreezer < 10) // If temp is 1-char
  {
    display.setCursor(27, 13 + vertAdj);
    display.print(tempFreezer);
    display.setCursor(75, 13 + vertAdj);
    display.print(tempFridge);
    display.setTextSize(1);
    display.setCursor(0, 40 + vertAdj);
    display.println(" Temps in Fahrenheit");
  }
  else if (tempFreezer < -100 || tempFreezer > 150 || tempFridge < -100 || tempFridge > 150) // ERROR
  {
    display.setCursor(19, 13 + vertAdj);
    display.print("ERROR");
    display.setTextSize(1);
    display.setCursor(0, 40 + vertAdj);
    display.println("Please let Mike know!");
  }
  else // If temp is 2-char #2
  {
    display.setCursor(18, 13 + vertAdj);
    display.print(tempFreezer);
    display.setCursor(75, 13 + vertAdj);
    display.print(tempFridge);
    display.setTextSize(1);
    display.setCursor(0, 40 + vertAdj);
    display.println(" Temps in Fahrenheit");
  }

  display.display();
}

void testdrawrect(void) {   // Draws a test pattern on the OLED display.
  for (int16_t i = 0; i < display.height() / 2; i += 2) {
    display.drawRect(i, i, display.width() - 2 * i, display.height() - 2 * i, WHITE);
    display.display();
    delay(1);
  }
}

void sfWebhook() {
  if (tempFreezer > -50 && tempFridge > 0)  // Screens out temp sensor errors.
  {
    //Blynk.virtualWrite(37, String("tempfreezer=") + tempFreezer + "&tempfridge=" + tempFridge);
    sfSend();
  }
  else
  {
    ++tempSensorErrorCount;
  }
}

void sfSend()
{
  Serial.print("connecting to ");
  Serial.println(hostSF);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 8080;
  if (!client.connect(hostSF, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  Serial.print("Requesting...");

  // This will send the request to the server
  client.print(String("GET ") + "/input/" + streamId + "?private_key=" + privateKey + "&tempfreezer=" + tempFreezer + "&tempfridge=" + tempFridge + " HTTP/1.1\r\n" +
               "Host: " + hostSF + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 15000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("closing connection");
}

BLYNK_WRITE(V27) // App button to report uptime
{
  int pinData = param.asInt();

  if (pinData == 0)
  {
    timer.setTimeout(18000L, uptimeSend);
  }
}

void uptimeSend()
{
  long minDur = millis() / 60000L;
  long hourDur = millis() / 3600000L;
  if (minDur < 121)
  {
    terminal.print(String("Refigerator: ") + minDur + " mins @ ");
    terminal.println(WiFi.localIP());
  }
  else if (minDur > 120)
  {
    terminal.print(String("Refigerator: ") + hourDur + " hrs @ ");
    terminal.println(WiFi.localIP());
  }
  terminal.flush();
}
