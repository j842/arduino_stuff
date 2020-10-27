#include <Arduino.h>

// #include <ESP8266WiFi.h>
// #include <WiFiUdp.h>

#include <FS.h>
#include <Update.h>
#include <WiFi.h>
#include <WiFiUdp.h>


#include <Wire.h> // Library for I2C communication
//#include <SPI.h>  // not used here, but needed to prevent a RTClib compile error w/ ESP8266
#include <RTClib.h>

#include <udpbro.h>
#include <jwifiota.h>
#include <jbuzzer.h>

#include <overrideswitch.h>


jwifiota wifiota("ESP32 Clock Master, Version 0.05");
udpbro gUDP;

const bool kScanI2C = false;
/* Output:
Scanning...
I2C device found at address 0x27  !
I2C device found at address 0x57  !
I2C device found at address 0x68  !
done
*/

const bool kSetClock = false;

RTC_DS1307 RTC;     // Setup an instance of DS1307 naming it RTC

overrideswitch gSwitch(17,{25,26,33,32});
jbuzzer gBuz(27);

// -------------------------------------------------------------------------------


void setdatetime()
{
  if (kSetClock)
  {
    RTC.adjust(DateTime(__DATE__, __TIME__));  // Time and date is expanded to date and time on your computer at compiletime
    Serial.printf("Time and date set to %s %s",__DATE__,__TIME__);
  }
  else  
    Serial.println("Clock left unchanged.");
}

// -------------------------------------------------------------------------------


void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  wifiota.setup();
  if (!gUDP.setup())
      exit(-1);

  RTC.begin();  // Init RTC

  gSwitch.setup();
  gBuz.setup();

  setdatetime();

  gBuz.playsong(6);
}

#include "scan.h"

// -------------------------------------------------------------------------------


// -------------------------------------------------------------------------------

// states to send to master:
// 7am  - On (all auxiliaries available)
// 8pm  - Jack in bed (Jack's forced off)
// 10pm - Tom in bed (Tom's bedroom available, everything else forced off)
// 11pm - Tom asleep (Everything forced off)


void loop() 
{
  static bool firstrun = true;

  if (firstrun)
  {
    firstrun=false;
    if (kScanI2C)
      scan();
  }

  wifiota.loop();
  bool gotUDPRequest = false;
  if (gUDP.loop()) // has UDP received packet.
  {
    const jbuf & b( gUDP.getBuf());
    if (b.getID()==kReq_ClockMaster)
      gotUDPRequest = true;
  }


  static unsigned long nextcheck = 0;
  static DateTime now;
  if (millis()>nextcheck)
  { // update time every second.
    now = RTC.now(); // also has day,month,year
    nextcheck = millis() + 1000;
  }

  static tSwitch4State prevstate = kState_Undefined;
  tSwitch4State newstate = gSwitch.loop(now);

  if (newstate != prevstate || gotUDPRequest)
  { // send the state to the client!
    jbuf b;
    b.setInt(kCmd_ClockMaster,static_cast<int>(newstate));
    gUDP.send(b,IPAddress(10,10,10,200));

    prevstate = newstate;
  }


  gBuz.loop();
}