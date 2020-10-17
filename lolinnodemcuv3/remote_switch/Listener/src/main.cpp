#include <Arduino.h>

#include <ESP8266WiFi.h>
//#include <DNSServer.h>
//#include <ESP8266WebServer.h>
//#include <WiFiManager.h>
#include <WiFiUdp.h>

#include <udpbro.h>
#include <jwifiota.h>
#include <jbuf.h>
#include <jrgbled.h>

/*

Built with PlatformIO in Visual Studio Code.

*/

udpbro gUDP;
jwifiota wifiota("ESP8266 Aux Listener, Version 0.01");
const IPAddress kControllerIP(10,10,10,200);

jrgbled gLed(4,0,2); // D2, D3, D4


void setup()
{ // connect via wifi to set up credentials (temporarily creates an access point to connect to)

  Serial.begin(115200);

  wifiota.setup();
  if (!gUDP.setup())
      exit(-1);
}

void loop()
{
  static bool firstrun=true;

  if (firstrun)
  {
    gLed.setRGB(255,0,0);
    delay(1000);
    gLed.setRGB(0,255,0);
    delay(1000);
    gLed.setRGB(0,0,255);
    delay(1000);
    gLed.setRGB(100,100,100);


    firstrun = false;
    jbuf rbuf;
    rbuf.setBool(kReq_Power, true);
    gUDP.send(rbuf,kControllerIP);
  }

  if (gUDP.loop()) // has UDP received packet.
  {
    const jbuf & b( gUDP.getBuf());

    switch (b.getID())
    {
      case kCmd_Power:
      {
        bool turnon=b.getBool();
        Serial.printf("UDP Message: CMD_POWER, %s\n", turnon ? "ON" : "OFF");

        turnon ? gLed.setRGB(0,0,100) : gLed.setRGB(100,20,0);

        jbuf rbuf;
        rbuf.setBool(kStat_Power, turnon);
        gUDP.send(rbuf,kControllerIP);

        break;
      }
      default:
        Serial.println("Unknown UDP Message.");
    }
  }

  wifiota.loop();
}
