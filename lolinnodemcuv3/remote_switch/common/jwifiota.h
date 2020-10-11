#ifndef __WIFISETUPOTA
#define __WIFISETUPOTA

#include <jwifi.h>

// #include <ESPAsyncTCP.h>
// #include <ESPAsyncWebServer.h>

#include <AsyncElegantOTA.h>


class jwifiota : public jwifi
{
    public:
        jwifiota() : server(80) {}

        void setup() 
        {
            WiFi.mode(WIFI_STA);

            jwifi::setup();

            server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
                request->send(200, "text/plain", "Hi! I am ESP32.");
            });

            AsyncElegantOTA.begin(&server);    // Start ElegantOTA
            server.begin();
            Serial.println("HTTP server started");
        }

        void loop()
        {
            jwifi::loop();

            AsyncElegantOTA.loop();
        }


    AsyncWebServer server;
};

#endif