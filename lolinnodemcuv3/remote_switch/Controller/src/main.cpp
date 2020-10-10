#include <Arduino.h>

#include <WiFi.h>

// #include <ESP8266WiFi.h>
//#include <DNSServer.h>
// #include <ESP8266WebServer.h>
//#include <WiFiManager.h>
#include <WiFiUdp.h>
#include <udpbro.h>

#define __NOTONE 1
#include <jbuzzer.h>

#include "/credentials/wificred.h"



/*
Built with PlatformIO in Visual Studio Code.

UDP Server. 10.10.10.200

test from linux with:   nc -u 10.10.10.200 9999

*/

udpbro udp;

jbuzzer jbuz(23);

// based on guide here:
// https://medium.com/@loginov_rocks/quick-start-with-nodemcu-v3-esp8266-arduino-ecosystem-and-platformio-ide-b8415bf9a038

// void ledOn()
// {
//   digitalWrite(LED_BUILTIN, LOW);
// }
// void ledOff()
// {
//   digitalWrite(LED_BUILTIN, HIGH);
// }
// void flashit()
// {
//   for (int i=0;i<10;i++)
//   {
//     ledOn();
//     delay(100+10*(10-i));
//     ledOff();
//     delay(100+10*(10-i));
//   }  
// }


void setup()
{ // connect via wifi to set up credentials (temporarily creates an access point to connect to)
//  pinMode(LED_BUILTIN, OUTPUT);
//  ledOn();

  Serial.begin(115200);
  WiFi.begin(myssid, mypassword);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.print("MAC address: ");
  Serial.println(WiFi.macAddress());
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("Network name: ");
  Serial.println(WiFi.SSID());
  Serial.print("Signal strength: ");
  Serial.println(WiFi.RSSI());
  Serial.println("Connected to the WiFi network");
  // WiFiManager wifiManager;
  // wifiManager.autoConnect("NodeMCU Setup");
  // Serial.println("WiFi Connected!");

  if (!udp.setup())
    exit(-1);

  jbuz.setup();

  //flashit();
}

static bool alreadydone=false;

void loop()
{
  if (!alreadydone)
  {
    jbuz.playsong(1);
    // buf b;
    // b.setString(kMessage,"cool");
    // udp.send(b,IPAddress(10,10,10,200),9999);
    // alreadydone=true;
  }


  jbuz.loop();
}