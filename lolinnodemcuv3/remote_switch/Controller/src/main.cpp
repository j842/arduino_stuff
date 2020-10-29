#include <Arduino.h>

#include <FS.h>
#include <Update.h>

#include <WiFi.h>
#include <WiFiUdp.h>

#include <udpbro.h>
#include <jbuzzer.h>
#include <jwifiota.h>

#include <jswitch.h>
#include <jled.h>

#include <lightyswitch.h>
#include <bossmain.h>
#include <auxswitch.h>


/*
Built with PlatformIO in Visual Studio Code.

Switches on and off Tom's auxiliary systems.

UDP Server. 10.10.10.200

Uses AsyncElegantOTA.
Visit http:10.10.10.200 for current version.
http:10.10.10.200/update to update firmware.

*/

const IPAddress kControllerIP(10,10,10,200);
const IPAddress kClockMasterIP(10,10,10,220);

udpbro udp;
jwifiota wifiota("ESP32 Controller, Version 0.26");
jbuzzer jbuz(12); // buzzer + on pin 12.

bossmain gBossMain(27,16,21,jbuz);
std::vector<auxswitch> gSwitches;
std::vector<bool> gOverride = {false,false,false,false};

void setup()
{
  Serial.begin(115200);

  wifiota.setup();
  if (!udp.setup()) // needs to be _after_ wifi is set up.
  {
    Serial.println("Failed to set up UDP.");
    sleep(1000);
    exit(-1);  
  }

  jbuz.setup();

  gSwitches.push_back(auxswitch(26,17,13,jbuz,&udp,IPAddress(10,10,10,201)));
  gSwitches.push_back(auxswitch(25, 5,14,jbuz,&udp,IPAddress(10,10,10,202)));
  gSwitches.push_back(auxswitch(33,18,22,jbuz,&udp,IPAddress(10,10,10,203)));
  gSwitches.push_back(auxswitch(32,19,23,jbuz,&udp,IPAddress(10,10,10,204)));

  for (auto & s: gSwitches) s.setup();
  gBossMain.setup();
}

void applybossmain()
{
  for (int i=0;i<gSwitches.size();++i)
  {
    gSwitches[i].Override(gOverride[i]);
    gSwitches[i].ShutDown(!gBossMain.ison());
  }
}

void loop()
{
  static bool firstloop = true;

  jbuz.loop();
  wifiota.loop();

  if (firstloop)
    { // request the clockmaster gives us our mode :-)
      jbuf b;
      b.setIDOnly(kReq_ClockMaster);
      udp.send(b,kClockMasterIP);
    }

  bool prevBossMain = gBossMain.ison();
  gBossMain.loop();
  if (gBossMain.ison() != prevBossMain || firstloop)
    applybossmain();

  for (auto & s: gSwitches) s.loop();

  
  if (udp.loop()) // has UDP received packet.
  {
    const jbuf & b( udp.getBuf());
    IPAddress remoteaddr = udp.getRemoteAddress();
    for (auto & s : gSwitches)
      if (s.getIP()==remoteaddr)
        s.handleUDP(b);

    if (b.getID()==kCmd_ClockMaster)
    { // clockmaster override!
      tSwitch4State state = static_cast<tSwitch4State>(b.getInt());
      jbuz.playsong(8);

      switch(state)
      {
        case kState_AllOn:
          gOverride = {false,false,false,false}; break;
        case kState_AllOff:
          gOverride = {true,true,true,true}; break;
        case kState_JackInBed:
          gOverride = {false,false,true,false}; break;
        case kState_TomInBed:
          gOverride = {true,false,true,true}; break;

        default:
          gOverride = {false,false,false,false}; break;
      }

      applybossmain();
    }
  }

  firstloop=false;
}
