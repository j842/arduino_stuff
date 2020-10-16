#include <Arduino.h>

#include <FS.h>
#include <Update.h>

#include <WiFi.h>
#include <WiFiUdp.h>

#include <udpbro.h>
#include <jbuzzer.h>
#include <jwifiota.h>

#include <lightyswitch.h>
#include <flashfun.h>


/*
Built with PlatformIO in Visual Studio Code.

Switches on and off Tom's auxiliary systems.

UDP Server. 10.10.10.200

Uses AsyncElegantOTA.
Visit http:10.10.10.200 for current version.
http:10.10.10.200/update to update firmware.

*/

udpbro udp;
jwifiota wifiota("ESP32 Controller, Version 0.17");
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

void loop()
{
  static flashfun ff(gSwitches,jbuz);

  jbuz.loop();
  wifiota.loop();

  for (auto & s: gSwitches) s.loop();
  ff.loop();
}
