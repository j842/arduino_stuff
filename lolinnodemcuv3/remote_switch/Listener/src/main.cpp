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
jwifiota wifiota("ESP8266 Aux Listener, Version 0.02");
const IPAddress kControllerIP(10,10,10,200);

jrgbled gLed(4,0,2); // D2, D3, D4


void setup()
{ // connect via wifi to set up credentials (temporarily creates an access point to connect to)

  Serial.begin(115200);

  wifiota.setup();
  if (!gUDP.setup())
      exit(-1);
}

void power(bool on)
{

}

void confirmstatus(bool status)
{
    // confirm with server our current status.
    jbuf rbuf;
    rbuf.setBool(kStat_Power, status);
    gUDP.send(rbuf,kControllerIP);
}

static const int kFadems = 4000;
static const int kFadeSteps = 80;
static const int kFadeStepSize = kFadems/kFadeSteps;

void loop()
{
  static bool firstrun=true;
  static bool shuttingdown=false;
  static unsigned long mOffTime=0;

  if (firstrun)
  {
    gLed.setRGB255(255,0,0);
    delay(333);
    gLed.setRGB255(0,255,0);
    delay(333);
    gLed.setRGB255(0,0,255);
    delay(333);
    gLed.setRGB255(255,0,0);


    firstrun = false;
    jbuf rbuf;
    rbuf.setIDOnly(kReq_Power);
    gUDP.send(rbuf,kControllerIP);
  }

  if (shuttingdown)
  {
    if (millis()>mOffTime)
    {
      gLed.setRGB255(0,0,0); // off
      shuttingdown=false; //already shut down :-)
    }
    else
    { // nice LED stuff.

      float stepnum = ((mOffTime-millis())/kFadeStepSize); // range (50..0 in 60 steps)
      gLed.setRGBh((int)(400.0*stepnum/kFadeSteps),(int)(80.0*stepnum/kFadeSteps),0);
    }
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

        turnon ? gLed.setRGB255(0,0,100) : gLed.setRGB255(100,20,0);
        power(turnon);
        confirmstatus(turnon);
        shuttingdown=false; //we're alive again!
        break;
      }
      case kCmd_Shutdown:
      {
        Serial.println("Server requests we shut down!");

        power(false);
        confirmstatus(false);
        shuttingdown = true;
        mOffTime = millis()+kFadems; // 4 sec off time.
        break;
      }
      default:
        Serial.println("Unknown UDP Message.");
    }
  }

  wifiota.loop();
}
