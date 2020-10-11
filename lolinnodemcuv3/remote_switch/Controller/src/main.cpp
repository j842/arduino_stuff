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
jwifiota wifiota("ESP32 Controller, Version 0.11");
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

static bool alreadydone=false;

unsigned long mFadeOff;
static const int kFadeOffTime=3000; // 3 seconds.

void handleSwitches()
{
  for (auto & s: gSwitches) s.loop();
  for (auto & l: gOnLeds) l.loop();
  for (auto & l: gOffLeds) l.loop();

  auto & bossmain = gSwitches[0];
    
  if (bossmain.changed_since_last_read())
  {
    bool on = bossmain.ison();
    gOnLeds[0].seton(on);
    gOffLeds[0].seton(!on); 

    if (on)
    {   
      for (int i=1;i<gOnLeds.size();++i)
      {
          gOnLeds[i].seton(gSwitches[i].ison());
          gOffLeds[i].seton(!gSwitches[i].ison());
      }
    } else {
      mFadeOff = millis()+kFadeOffTime; // turn off after a bnit.
      for (int i=1;i<gOnLeds.size();++i)
        {
          gOnLeds[i].seton(false);
          gOffLeds[i].seton(false);
        }
      jbuz.playsong(4);
    }
  }

  if (bossmain.ison())
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
  else
  { // bossmain off.
    if (mFadeOff>0 && millis()>mFadeOff)
    {
      gOffLeds[0].seton(false); // shhh.
      mFadeOff=0;
    }
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
