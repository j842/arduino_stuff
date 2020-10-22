#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <Wire.h> // Library for I2C communication
#include <SPI.h>  // not used here, but needed to prevent a RTClib compile error
#include "RTClib.h"
#include <LiquidCrystal_I2C.h> // Library for LCD

#include <udpbro.h>
#include <jwifiota.h>



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

  setdatetime();

  lcd.setCursor(0, 0);
  lcd.print("Real Time Clock");
  // lcd.clear();
}

#include "scan.h"

// -------------------------------------------------------------------------------

void displayTimeLoop()
{
  static unsigned long displaytime = millis();

  if (millis()>displaytime)
  {
    displaytime = millis()+1000;

    DateTime now = RTC.now();
  
    Serial.print(now.day(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.year(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    if (now.minute()<10) Serial.print("0");
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    if (now.second()<10) Serial.print("0");
    Serial.print(now.second(), DEC);
    Serial.println();
  
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Date: ");
    lcd.setCursor(0, 1);
    lcd.print("Time: ");
    
    lcd.setCursor(6, 0);
    lcd.print(now.day(), DEC);
    lcd.print("/");
    lcd.print(now.month(), DEC);
    lcd.print("/");
    lcd.print(now.year(), DEC);
  
    lcd.setCursor(6, 1);
    lcd.print(now.hour(), DEC);
    lcd.print(":");
    if (now.minute()<10) lcd.print("0");
    lcd.print(now.minute(), DEC);
    lcd.print(":");
    if (now.second()<10) lcd.print("0");
    lcd.print(now.second(), DEC); 
  }
}

// -------------------------------------------------------------------------------


void loop() 
{
  if (kScanI2C)
    scan();
  
  wifiota.loop();

  if (gUDP.loop()) // has UDP received packet.
  {
    const jbuf & b( gUDP.getBuf());
  }

  displayTimeLoop();
}