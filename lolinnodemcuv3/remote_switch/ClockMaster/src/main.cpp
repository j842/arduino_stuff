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


// see https://github.com/espressif/arduino-esp32/issues/4
// need to add the following to the LiquidCrystal_I2C.h library
// #define analogWrite ledcWrite
#include <LiquidCrystal_I2C.h> // Library for LCD

#include <udpbro.h>
#include <jwifiota.h>
#include <jbuzzer.h>

#include <overrideswitch.h>


jwifiota wifiota("ESP32 Clock Master, Version 0.02");
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


// connected via the Wire SCL and SDA pins (GPIO 22, 21)
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(PCF8574_ADDR_A21_A11_A01); // 0x27

RTC_DS1307 RTC;     // Setup an instance of DS1307 naming it RTC

overrideswitch gSwitch(17,25,26,33,32);
// switch4 gSwitch4(25,26,33,32); // 35,34,39,36 input only pins don't support pull up/down
// jswitch gSwitch(17);

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

  Wire.begin();
  lcd.begin(16,2);
  lcd.backlight();
  RTC.begin();  // Init RTC

  gSwitch.setup();
  gBuz.setup();

  setdatetime();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Hello!");

  gBuz.playsong(6);
}

#include "scan.h"

// -------------------------------------------------------------------------------

void padTo(std::string &str, const size_t num=16, const char paddingChar = ' ')
{
    if(num > str.size())
        str.insert(0, num - str.size(), paddingChar);
}

void lcdmessage(std::string l1, std::string l2)
{
  static std::string ol1, ol2;

  if (ol1.compare(l1)==0 && ol2.compare(l2)==0)
    return;
  ol1=l1;
  ol2=l2;

  padTo(l1);
  padTo(l2);
          
  //lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(l1.c_str());
  lcd.setCursor(0,1);
  lcd.print(l2.c_str());
}

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
  gSwitch.loop();
  gBuz.loop();
  bool gotUDPPacket = gUDP.loop();


  if (gotUDPPacket) // has UDP received packet.
  {
    const jbuf & b( gUDP.getBuf());
  }

  // handle change of auto/manual switch.

  std::string s1,s2; 
  DateTime now = RTC.now(); // also has day,month,year
  if (gSwitch.getMessage(s1,s2,now))
    lcdmessage(s1,s2);
}