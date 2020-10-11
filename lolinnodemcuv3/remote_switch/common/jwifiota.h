#ifndef __WIFISETUPOTA
#define __WIFISETUPOTA

#include <jwifi.h>

// #include <ESPAsyncTCP.h>
// #include <ESPAsyncWebServer.h>

#include <AsyncElegantOTA.h>

static char serverMsg[255];


class jwifiota : public jwifi
{
    public:
        jwifiota(std::string s) : mServer(80) 
        {
            strncpy(serverMsg,s.c_str(),255);
        }

        void setup() 
        {
            WiFi.mode(WIFI_STA);

            jwifi::setup();

            mServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
                request->send(200, "text/plain", serverMsg);
            });

            AsyncElegantOTA.begin(&mServer);    // Start ElegantOTA
            mServer.begin();
            Serial.println("HTTP server started");
        }

        void loop()
        {
            jwifi::loop();

            AsyncElegantOTA.loop();
        }


    AsyncWebServer mServer;
};

#endif