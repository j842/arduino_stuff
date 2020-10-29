#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <udpbro.h>
#include <jwifiota.h>
#include <jbuf.h>
#include <jrgbled.h>
#include <jrelay.h>

/*

Built with PlatformIO in Visual Studio Code.

*/

udpbro gUDP;
jwifiota wifiota("ESP8266 Aux Listener, Version 0.06");

const IPAddress kControllerIP(10,10,10,200);
const IPAddress kClockMasterIP(10,10,10,220);

jrgbled gLed(4,0,2); // D2, D3, D4
jrelay gRelay(14); // D5

class clientState
{
  public:

    clientState() :
      mControllerPowerOn(false),
      mControllerShutdown(false),
      mOverrideShutdown(false),
      mShuttingDown(false),
      mOffTime(0)
    {
    }

    bool isOn() const
    {
      if (!isShutDown() && mControllerPowerOn)
        return true;
      return false;
    }

    bool override() const{
      return mOverrideShutdown;
    }

    bool shuttingDown() 
    {
      if (!mShuttingDown)    
        return false;

      if (millis()>mOffTime)
      {
        mShuttingDown = false;
        mOffTime = 0;
        return false;
      }

      return true;
    }

    float percentFade()
    {
      if (!shuttingDown())
        return 0.0;
      float f = mOffTime-millis();
      return (f/static_cast<float>(kFadems));
    }

    void setControllerPowerOn(bool t) {mControllerPowerOn = t;}
    void setControllerShutdown(bool t) {mControllerShutdown = t; handleshutdown();}
    void setOverrideShutdown(bool t) {mOverrideShutdown = t; handleshutdown();}  

    bool isShutDown() const {
      return (mOverrideShutdown || mControllerShutdown);
    }
  private:
    void handleshutdown()
    { // figure out if we're shutting down.
      if (isShutDown())
      {
        mShuttingDown = true;
        mOffTime = millis()+kFadems;
      } else {
        mShuttingDown = false;
        mOffTime = 0;
      }
    }

  private:
    static const int kFadems = 4000;

    bool mControllerPowerOn;
    bool mControllerShutdown;
    bool mOverrideShutdown;

    bool mShuttingDown;
    unsigned long mOffTime;
};


static clientState sClientState;




void setup()
{ // connect via wifi to set up credentials (temporarily creates an access point to connect to)

  Serial.begin(115200);

  wifiota.setup();
  if (!gUDP.setup())
      exit(-1);

  gLed.setup();
  gRelay.setup();
}


void confirmstatus()
{
  // confirm our current status to Controller.
  jbuf rbuf;
  rbuf.setBool(kStat_Power, sClientState.isOn());
  gUDP.send(rbuf,kControllerIP);
}

void trafficlights()
{
    gLed.setRGB255(255,0,0);
    delay(200);
    gLed.setRGB255(0,255,0);
    delay(200);
    gLed.setRGB255(0,0,255);
    delay(200);
    gLed.setRGB255(255,0,0);
}

void loop()
{
  static bool firstrun=true;

  if (firstrun)
  {
    firstrun = false;

    trafficlights();

    jbuf rbuf;
    rbuf.setIDOnly(kReq_Power);
    gUDP.send(rbuf,kControllerIP);

    rbuf.setIDOnly(kReq_ClockMaster);
    gUDP.send(rbuf,kClockMasterIP);
  }

  if (sClientState.shuttingDown())
      gLed.setRGBh( (int)(400.0*sClientState.percentFade()), (int)(80.0*sClientState.percentFade()),0);

  if (gUDP.loop()) // has UDP received packet.
  {
    const jbuf & b( gUDP.getBuf());

    switch (b.getID())
    {
      case kCmd_Power:
      {
        Serial.println("Controller sets power state.");
        sClientState.setControllerPowerOn(b.getBool());
        sClientState.setControllerShutdown(false); // power state only set when shutdown cancelled.
        break;
      }
      case kCmd_Shutdown:
      {
        Serial.println("Controller requests we shut down!");
        sClientState.setControllerShutdown(true);
        break;
      }
      case kCmd_OverridePower:
      {
        Serial.println("ClockMaster overrides!");
        sClientState.setOverrideShutdown(b.getBool());
        break;
      }

      default:
        Serial.println("Unknown UDP Message.");
    }


    gRelay.set(!sClientState.isOn()); // we're using the Normally Open connection (D5 high at boot)
    if (!sClientState.isShutDown())
      sClientState.isOn() ? gLed.setRGB255(0,100,0) : gLed.setRGB255(100,20,0);

    confirmstatus();
  }

  wifiota.loop();
}
