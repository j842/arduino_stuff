#ifndef __BUFFERBUDDY_H
#define __BUFFERBUDDY_H

#include <jbuf.h>
#include <WiFiUdp.h>

// thin wrapper on WiFiUDP.
class udpbro 
{
    public:
        udpbro() 
        {
        }

        bool setup() 
        {
            int rval = mUDP.begin(localUdpPort);
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
            int packetSize = mUDP.parsePacket();
            if (packetSize<=0)
                return false;

            Serial.printf("Received %d byte message from %s, port %d\n", packetSize, mUDP.remoteIP().toString().c_str(), mUDP.remotePort());

            uint8_t len = mUDP.read(mBuf._getBufW(),mBuf._getMaxLen());
            mBuf._setLen(len);

            return true;
        }

        const jbuf & getBuf() {return mBuf;}

        bool send(const jbuf & b, IPAddress ip, uint16_t port=localUdpPort)
        {
            mUDP.beginPacket(ip,port);
            size_t bytesWritten = mUDP.write(b._getBufR(),b._getLen());
            mUDP.endPacket();

            return (bytesWritten==b._getLen());
        }

        IPAddress getRemoteAddress() {return mUDP.remoteIP();}

    private:
        WiFiUDP mUDP;
        static const unsigned int localUdpPort = 9999;  // local port to listen on 

        jbuf mBuf;
};


#endif
