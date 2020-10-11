#ifndef __WIFISETUP
#define __WIFISETUP

#include "/credentials/wificred.h"
// format of the wificred.h file is:
// const char* myssid = "ssid";
// const char* mypassword =  "password";



class jwifi
{
    public:
        jwifi() {}

        void setup()
        {
            WiFi.begin(myssid, mypassword);
            while (WiFi.status() != WL_CONNECTED) {
                delay(500);
                Serial.println("Connecting to WiFi..");
            }
            Serial.print("MAC address: ");
            Serial.println(WiFi.macAddress());
            Serial.print("Local IP: ");
            Serial.println(WiFi.localIP());
            Serial.print("Network name: ");
            Serial.println(WiFi.SSID());
            Serial.print("Signal strength: ");
            Serial.println(WiFi.RSSI());
            Serial.println("Connected to the WiFi network");
        }

        void loop()
        {
        }

};

#endif