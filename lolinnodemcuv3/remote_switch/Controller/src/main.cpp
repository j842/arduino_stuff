#include <Arduino.h>

#include <FS.h>
#include <Update.h>

#include <WiFi.h>
#include <WiFiUdp.h>

#include <udpbro.h>
#include <jbuzzer.h>
#include <jwifiota.h>

#include <lightyswitch.h>


/*
Built with PlatformIO in Visual Studio Code.

Switches on and off Tom's auxiliary systems.

UDP Server. 10.10.10.200

Uses AsyncElegantOTA.
Visit http:10.10.10.200 for current version.
http:10.10.10.200/update to update firmware.

*/

udpbro udp;
jwifiota wifiota("ESP32 Controller, Version 0.14");
jbuzzer jbuz(12); // buzzer + on pin 12.


std::vector<lightyswitch> gSwitches;


void setup()
{
  Serial.begin(115200);

  gSwitches.push_back(lightyswitch(27,16,21,jbuz));
  gSwitches.push_back(lightyswitch(26,17,13,jbuz));
  gSwitches.push_back(lightyswitch(25, 5,14,jbuz));
  gSwitches.push_back(lightyswitch(33,18,22,jbuz));
  gSwitches.push_back(lightyswitch(32,19,23,jbuz));

  for (auto & s: gSwitches) s.setup();

  wifiota.setup();

  if (!udp.setup())
    exit(-1);

  jbuz.setup();
}

typedef enum {
  ff_NotRunning,
  ff_RunningFlashy,
  ff_RunningFadeBossMain
} tffstate;

// make turning the bossmain switch on or off fancy :-)
class flashfun
{
  public:
    flashfun() : mState(ff_NotRunning),mNdx(0),mNextTime(0) {}
    
    void start(bool bossmainOn)
    {
      for (auto & s : gSwitches) s.changelightymode(kls_00);

      mNdx=0;
      if (bossmainOn)
      {
        mState=ff_RunningFlashy;
        gSwitches[0].changelightymode(kls_10);
        mNextTime=millis()+kNextTime;
      }
      else
      { // bossmain off, go to sleep.
        mState=ff_RunningFadeBossMain;
        for (auto & s : gSwitches) s.changelightymode(kls_00);
        gSwitches[0].changelightymode(kls_01);
        mNextTime=millis()+kFadeOffTime;
        jbuz.playsong(4);
      }
    }

    void loop()
    {
      if (mState==ff_NotRunning || (millis()<mNextTime)) return;

      if (mState==ff_RunningFadeBossMain)
      { // finish up
        mState=ff_NotRunning;
        gSwitches[0].changelightymode(kls_00);
        return;
      }

      // flashyfun
      getSwitch().changelightymode(kls_00);
      mNdx++;
      if (mNdx<10)
      {
        getSwitch().changelightymode(mNdx<5 ? kls_01 : kls_10);
        mNextTime=millis()+kNextTime;
      }
      else
      { // all done, set to correct values.
        mState=ff_NotRunning;
        for (auto & s : gSwitches) s.changelightymode(kls_auto);
      }
    }

    tffstate state() {return mState;}
    lightyswitch & getSwitch() {if (mNdx<5) return gSwitches[mNdx]; else return gSwitches[9-mNdx];}
  private:
    tffstate mState;
    int mNdx;
    unsigned long mNextTime;
    static const int kFadeOffTime=3000; // 3 seconds.
    static const int kNextTime=100; // 100 milliseconds.
};

flashfun ff;

bool prevbossmain = false;

void loop()
{
  jbuz.loop();
  wifiota.loop();

  for (auto & s: gSwitches) s.loop();

  ff.loop();

  // handle bossmain switch.
  auto & bossmain = gSwitches[0];
  if (bossmain.ison() != prevbossmain)
  {
    if (bossmain.ison())
      ff.start(true);
    else
      ff.start(false);
    prevbossmain = bossmain.ison();
  }

}
