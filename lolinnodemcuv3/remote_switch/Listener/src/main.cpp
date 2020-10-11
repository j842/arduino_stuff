#include <Arduino.h>

#include <ESP8266WiFi.h>
//#include <DNSServer.h>
//#include <ESP8266WebServer.h>
//#include <WiFiManager.h>
#include <WiFiUdp.h>

#include <udpbro.h>
#include <jwifiota.h>

/*

Built with PlatformIO in Visual Studio Code.

*/

udpbro udp;
jwifiota wifiota("ESP8266 Auxiliary, Version 0.01");



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
  // pinMode(LED_RED, OUTPUT);
  // pinMode(LED_BLUE, OUTPUT);

  // ledOn(LED_RED);

  Serial.begin(115200);
  // WiFiManager wifiManager;
  // wifiManager.autoConnect("NodeMCU Setup");
  // Serial.println("WiFi Connected!");

  wifiota.setup();


  if (!udp.setup())
      exit(-1);

}

void loop()
{
  if (udp.loop()) // has UDP received packet.
  {
    const buf & b( udp.getBuf());

    std::string ReceivedMessage( b.getString() );
    Serial.printf("UDP message: %s\n", ReceivedMessage.c_str());

    // if (tolower(ReceivedMessage[0])=='r')
    //   ledOn(LED_RED);
    // else if (tolower(ReceivedMessage[0]=='b'))
    //   ledOn(LED_BLUE);
    // else if (tolower(ReceivedMessage[0]=='c'))
    //   cool();
  }

  wifiota.loop();
}
