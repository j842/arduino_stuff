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

/*
Built with PlatformIO in Visual Studio Code.

Switches on and off Tom's auxiliary systems.

UDP Server. 10.10.10.200

Uses AsyncElegantOTA.
Visit http:10.10.10.200 for current version.
http:10.10.10.200/update to update firmware.

*/

udpbro udp;
jwifiota wifiota("ESP32 Controller, Version 0.12");
jbuzzer jbuz(12); // buzzer + on pin 12.

std::vector<jswitch> gSwitches;

std::vector<jled> gOnLeds;
std::vector<jled> gOffLeds;

void setup()
{
  Serial.begin(115200);

  gSwitches.push_back(jswitch(27)); 
  gSwitches.push_back(jswitch(26)); 
  gSwitches.push_back(jswitch(25)); 
  gSwitches.push_back(jswitch(33)); 
  gSwitches.push_back(jswitch(32)); 

  gOnLeds.push_back(jled(16));
  gOnLeds.push_back(jled(17));
  gOnLeds.push_back(jled(5));
  gOnLeds.push_back(jled(18));
  gOnLeds.push_back(jled(19));

  gOffLeds.push_back(jled(21));
  gOffLeds.push_back(jled(13));
  gOffLeds.push_back(jled(14));
  gOffLeds.push_back(jled(22));
  gOffLeds.push_back(jled(23));


  for (auto & s: gSwitches) s.setup();
  for (auto & l: gOnLeds) l.setup();
  for (auto & l: gOffLeds) l.setup();

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
      for (auto & l: gOnLeds) l.seton(false);
      for (auto & l: gOffLeds) l.seton(false);
      mNdx=0;
      if (bossmainOn)
      {
        mState=ff_RunningFlashy;
        gOnLeds[0].seton(true);
        mNextTime=millis()+kNextTime;
      }
      else
      {
        mState=ff_RunningFadeBossMain;
        gOffLeds[0].seton(true);
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
        gOffLeds[0].seton(false); // ssh.
        return;
      }

      // flashyfun
      getLED().seton(false);
      mNdx++;
      if (mNdx<10)
      {
        getLED().seton(true);
        mNextTime=millis()+kNextTime;
      }
      else
      { // all done, set to correct values.
        mState=ff_NotRunning;
        for (int i=0;i<gSwitches.size();++i)
        {
          gOnLeds[i].seton(gSwitches[i].ison());
          gOffLeds[i].seton(!gSwitches[i].ison());
        }
      }
    }

    tffstate state() {return mState;}
    jled & getLED() {if (mNdx<5) return gOnLeds[mNdx]; else return gOffLeds[9-mNdx];}
  private:
    tffstate mState;
    int mNdx;
    unsigned long mNextTime;
    static const int kFadeOffTime=3000; // 3 seconds.
    static const int kNextTime=100; // 100 milliseconds.
};

flashfun ff;

static bool alreadydone=false;

void handleSwitches()
{
  for (auto & s: gSwitches) s.loop();
  for (auto & l: gOnLeds) l.loop();
  for (auto & l: gOffLeds) l.loop();

  ff.loop();

  auto & bossmain = gSwitches[0];
    
  if (bossmain.changed_since_last_read())
  {
    bool on = bossmain.ison();
    gOnLeds[0].seton(on);
    gOffLeds[0].seton(!on); 

    if (on)
      ff.start(true);
    else
      ff.start(false);
  }

  if (bossmain.ison() && ff.state()==ff_NotRunning)
  {
    int toplay=0;
    for (int i=1;i<gSwitches.size();++i)
      if (gSwitches[i].changed_since_last_read())
      {
        bool on = gSwitches[i].ison();
        if (!on && toplay==0)
          toplay=7;
        else
          toplay=6;
        
        gOnLeds[i].seton(on);
        gOffLeds[i].seton(!on);      
      }
    if (toplay>0)
      jbuz.playsong(toplay);
  } 
}


void loop()
{
  if (!alreadydone)
  {
    jbuz.playsong(1);
    // buf b;
    // b.setString(kMessage,"cool");
    // udp.send(b,IPAddress(10,10,10,200),9999);
    alreadydone=true;
  }

  handleSwitches();

  jbuz.loop();
  wifiota.loop();
}
