#ifndef __BUFFERBUDDY_H
#define __BUFFERBUDDY_H

#include <WiFiUdp.h>

class bufferbuddy 
{
    public:
        bufferbuddy() 
        {
            ReceivedMessage[0]=0;
        }

        bool setup() 
        {
            int rval = Udp.begin(localUdpPort);
            if (rval!=1)
            {
                Serial.printf("Failed to open UDP port %d\n",localUdpPort);
                return false;
            }
            Serial.printf("UDP up and running on IP %s\n", WiFi.localIP().toString().c_str());
            return true;
        }

        bool loop() 
        {
              int packetSize = Udp.parsePacket();
              if (packetSize<=0)
                  return false;
                Serial.printf("Received %d byte message from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());

                // receive incoming UDP message
                int len = Udp.read(ReceivedMessage, 255);
                if (len > 0)
                {
                ReceivedMessage[len] = 0;
                if (ReceivedMessage[len-1]=='\n')
                    ReceivedMessage[len-1]=0;
                }
                Serial.printf("UDP message: %s\n", ReceivedMessage);
                return true;
        }

        const char * getMessage() 
        {
            return ReceivedMessage;
        }

        bool sendMessage(std::string msg, const char *host, uint16_t port)
        {
            Udp.beginPacket(host,port);
            Udp.write(msg.c_str());
            Udp.endPacket();
            return true;
        }

    private:
        WiFiUDP Udp;

        const unsigned int localUdpPort = 9999;  // local port to listen on 
        char ReceivedMessage[255];  // buffer for incoming packets
};

#endif
