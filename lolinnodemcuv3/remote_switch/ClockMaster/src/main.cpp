#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <Wire.h> // Library for I2C communication
#include <SPI.h>  // not used here, but needed to prevent a RTClib compile error
#include "RTClib.h"
#include <LiquidCrystal_I2C.h> // Library for LCD

#include <udpbro.h>
#include <jwifiota.h>
#include <switch4.h>

#include <iostream>

jwifiota wifiota("ESP8266 Clock Master, Version 0.01");
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


LiquidCrystal_I2C lcd = LiquidCrystal_I2C(PCF8574_ADDR_A21_A11_A01); // 0x27
RTC_DS1307 RTC;     // Setup an instance of DS1307 naming it RTC

switch4 gSwitch(14,12,13,15); // D5,D6,D7,D8

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

  Wire.begin();
  lcd.begin(16,2);
  lcd.backlight();
  RTC.begin();  // Init RTC

  gSwitch.setup();

  setdatetime();

  lcd.setCursor(0, 0);
  lcd.print("Real Time Clock");
  // lcd.clear();
}

#include "scan.h"

// -------------------------------------------------------------------------------


void lcdmessage(std::string l1, std::string l2)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(l1.c_str());
  lcd.setCursor(0,1);
  lcd.print(l2.c_str());
}

std::string tostr(int x) 
{
  char s[10];
  sprintf(s,"%02d",x);
  return s;
}

static unsigned long displaytime = millis();
static std::string gModeStr="<starting up>";
void displayTimeLoop()
{
  if (millis()>displaytime)
  {
    displaytime = millis()+1000; // update every second.

    DateTime now = RTC.now(); // also has day,month,year
    std::string l2 = "    " + tostr(now.hour()) + ":" + tostr(now.minute()) + ":" + tostr(now.second());
    lcdmessage(gModeStr,l2);
  }
}

// -------------------------------------------------------------------------------


void loop() 
{
  static tSwitch4State state = kState_Undefined;
  static bool firstrun = true;

  if (firstrun)
  {
    firstrun=false;

    if (kScanI2C)
      scan();
  }


  wifiota.loop();

  gSwitch.loop();  

  if (gUDP.loop()) // has UDP received packet.
  {
    const jbuf & b( gUDP.getBuf());
  }

  if (gSwitch.getSate()!=state)
  {
    state=gSwitch.getSate();
    switch (state)
    {
      case kState_Auto:
        lcdmessage("Set to fully","automatic.");
        gModeStr = "Automatic";
        break;
      case kState_TomInBed:
        gModeStr = "Manual: in bed";
        lcdmessage("Manual mode:","TOM IN BED");
        break;
      case kState_TomAsleep:
        gModeStr = "Manual: asleep";
        lcdmessage("Manual mode:","TOM ASLEEP zzz");
        break;
      case kState_TomAwake:
        gModeStr = "Manual: daytime";
        lcdmessage("Manual mode:","DAYTIME! ALL ON.");
        break;
      default:
        lcdmessage("ERROR","Bad state.");
    }

    displaytime = millis()+2000;
  } 
  else
    displayTimeLoop();
}