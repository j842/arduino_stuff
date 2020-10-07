#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <WiFiUdp.h>
#include "../../common/bufferbuddy.h"

/*
Built with PlatformIO in Visual Studio Code.

UDP Server. 10.10.10.200

test from linux with:   nc -u 10.10.10.200 9999

*/

bufferbuddy bb;

// based on guide here:
// https://medium.com/@loginov_rocks/quick-start-with-nodemcu-v3-esp8266-arduino-ecosystem-and-platformio-ide-b8415bf9a038

void ledOn()
{
  digitalWrite(LED_BUILTIN, LOW);
}
void ledOff()
{
  digitalWrite(LED_BUILTIN, HIGH);
}
void flashit()
{
  for (int i=0;i<10;i++)
  {
    ledOn();
    delay(100+10*(10-i));
    ledOff();
    delay(100+10*(10-i));
  }  
}


void setup()
{ // connect via wifi to set up credentials (temporarily creates an access point to connect to)
  pinMode(LED_BUILTIN, OUTPUT);
  ledOn();

  Serial.begin(115200);
  WiFiManager wifiManager;
  wifiManager.autoConnect("NodeMCU Setup");
  Serial.println("WiFi Connected!");

  if (!bb.setup())
    exit(-1);

  flashit();
}

static bool alreadydone=false;

void loop()
{
  if (!alreadydone)
  {
    bb.sendString("cool","10.10.10.200",9999);
    alreadydone=true;
  }

}