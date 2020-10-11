#include <Arduino.h>

#include <FS.h>
#include <Update.h>

#include <WiFi.h>
#include <WiFiUdp.h>

#include <udpbro.h>
#include <jbuzzer.h>
#include <jwifiota.h>


/*
Built with PlatformIO in Visual Studio Code.

UDP Server. 10.10.10.200

test from linux with:   nc -u 10.10.10.200 9999

*/

udpbro udp;
jwifiota wifiota;
jbuzzer jbuz(12); // buzzer + on pin 12.

void setup()
{ 
  Serial.begin(115200);

  wifiota.setup();

  if (!udp.setup())
    exit(-1);

  jbuz.setup();
}

static bool alreadydone=false;

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

  jbuz.loop();
  wifiota.loop();
}
