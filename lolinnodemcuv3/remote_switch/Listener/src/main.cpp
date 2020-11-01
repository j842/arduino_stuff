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
jwifiota wifiota("ESP8266 Aux Listener, Version 0.09");

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
      mOverrideShutdown(false)
    {
    }

    bool isOn() const
    {
      if (!isShutDown() && mControllerPowerOn)
        return true;
      return false;
    }

    void setControllerPowerOn(bool t) {mControllerPowerOn = t;}
    void setControllerShutdown(bool t) {mControllerShutdown = t;}
    void setOverrideShutdown(bool t) {mOverrideShutdown = t;}  

    bool isShutDown() const {
      return (mOverrideShutdown || mControllerShutdown);
    }

    rgb1023 getColour()
    {
      if (mOverrideShutdown)
        return rgb1023(20,0,0); // very dark red
      if (mControllerShutdown)
        return rgb1023(100,50,0); // dark orange.
      return mControllerPowerOn ? rgb1023(0,500,0) : rgb1023(500,100,0);
    }

  private:
    bool mControllerPowerOn;
    bool mControllerShutdown;
    bool mOverrideShutdown;
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
    gLed.setRGB(rgb1023(1023,0,0));
    delay(300);
    gLed.setRGB(rgb1023(0,1023,0));
    delay(300);
    gLed.setRGB(rgb1023(0,0,1023));
    delay(300);
    gLed.setRGB(rgb1023(512,0,0));
}

void firstrun()
{
  static bool sFirstRun=true;
  if (!sFirstRun) return;
  sFirstRun=false;

  trafficlights();

  jbuf rbuf;
  rbuf.setIDOnly(kReq_Power);
  gUDP.send(rbuf,kControllerIP);

  rbuf.setIDOnly(kReq_ClockMaster);
  gUDP.send(rbuf,kClockMasterIP);
}

void handleUDP()
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
      return;
  }

  // update relay state.
  gRelay.set(!sClientState.isOn()); // we're using the Normally Open connection (D5 high at boot)

  // update LED state.
  if (sClientState.isShutDown())
    gLed.fade(sClientState.getColour());
  else  
    gLed.setRGB(sClientState.getColour());

  // confirm our current status with the controller.
  confirmstatus();
}

void loop()
{
  firstrun();

  wifiota.loop();
  gLed.loop();

  if (gUDP.loop()) // has UDP received packet.
    handleUDP();

}
