#ifndef __BUFFERBUDDY_H
#define __BUFFERBUDDY_H

#include <WiFiUdp.h>

class udpbro 
{
    public:
        udpbro() 
        {
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

            return true;
        }

        std::string getString() 
        {
            char ReceivedMessage[255];  // buffer for incoming packets
            int len = getBytes((uint8_t *)(&ReceivedMessage[0]),255);
            if (len > 0)
                {
                ReceivedMessage[len] = 0;
                if (ReceivedMessage[len-1]=='\n')
                    ReceivedMessage[len-1]=0;
                }
            else
                    ReceivedMessage[0]=0;
                
            Serial.printf("UDP message: \"%s\"\n", ReceivedMessage);
            std::string s(ReceivedMessage);
            return s;
        }


        bool sendString(std::string msg, const char *host, uint16_t port)
        {
            bool okay = sendBytes((uint8_t *)(msg.c_str()),msg.length(),host,port);
            if (okay) 
                Serial.printf("Sent: \"%s\"\n",msg.c_str());
            else
                Serial.printf("Failed to send: \"%s\"\n",msg.c_str());
            
            return okay;
        }

        size_t getBytes(uint8_t * bytes, size_t maxlen)
        {
            return Udp.read(bytes, maxlen);
        }

        bool sendBytes(uint8_t * bytes, size_t len, const char *host, uint16_t port)
        {
            Udp.beginPacket(host,port);
            size_t bytesWritten = Udp.write(bytes,len);
            Udp.endPacket();

            return (bytesWritten==len);
        }

    private:
        WiFiUDP Udp;
        const unsigned int localUdpPort = 9999;  // local port to listen on 
};


#endif
