#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <WiFiUdp.h>
#include "../../common/bufferbuddy.h"

/*

UDP Server. 10.10.10.200

test from linux with:   nc -u 10.10.10.200 9999

*/

bufferbuddy bb;


// based on guide here:
// https://medium.com/@loginov_rocks/quick-start-with-nodemcu-v3-esp8266-arduino-ecosystem-and-platformio-ide-b8415bf9a038

#define LED_RED D1
#define LED_BLUE D0


void ledOn(uint8_t LED)
{
  digitalWrite(LED, HIGH);
}
void ledOff(uint8_t LED)
{
  digitalWrite(LED, LOW);
}


void setup()
{ // connect via wifi to set up credentials (temporarily creates an access point to connect to)
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  ledOn(LED_RED);

  Serial.begin(115200);
  WiFiManager wifiManager;
  wifiManager.autoConnect("NodeMCU Setup");
  Serial.println("WiFi Connected!");

  if (!bb.setup())
    {
      ledOn(LED_RED);
      exit(-1);
    }

  ledOn(LED_BLUE);
  delay(1000);
  ledOff(LED_RED);
}

void cool()
{
  for (int i=0;i<10;i++)
  {
    ledOn(LED_BLUE);
    ledOff(LED_RED);
    delay(100);
    ledOff(LED_BLUE);
    ledOn(LED_RED);
    delay(100);
  }  
  ledOn(LED_BLUE);
  delay(500);
  ledOff(LED_RED);
}


void loop()
{
  if (bb.loop())
  {
    ledOff(LED_BLUE);
    ledOff(LED_RED);
    // receive incoming UDP message

    std::string ReceivedMessage( bb.getMessage() );
    Serial.printf("UDP message: %s\n", ReceivedMessage.c_str());

    if (tolower(ReceivedMessage[0])=='r')
      ledOn(LED_RED);
    else if (tolower(ReceivedMessage[0]=='b'))
      ledOn(LED_BLUE);
    else if (tolower(ReceivedMessage[0]=='c'))
      cool();
  }
}
