#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

// based on guide here:
// https://medium.com/@loginov_rocks/quick-start-with-nodemcu-v3-esp8266-arduino-ecosystem-and-platformio-ide-b8415bf9a038

//#define LED D0
#define LED LED_BUILTIN

void setup()
{ // connect via wifi to set up credentials (temporarily creates an access point to connect to)
  Serial.begin(115200);
  WiFiManager wifiManager;
  wifiManager.autoConnect("NodeMCU-Arduino-PlatformIO");
  Serial.println("Connected!");

  pinMode(LED, OUTPUT);
}

void loop()
{
  digitalWrite(LED, LOW);
  delay(500);
  digitalWrite(LED, HIGH);
  delay(1000);
}