#ifndef __AUXSWITCH_H
#define __AUXSWITCH_H


class auxswitch : protected lightyswitch
{
    public:
    auxswitch(int switchpin, int onlightpin, int offlightpin, jbuzzer & buz, udpbro * udp, IPAddress ip) :
        lightyswitch(switchpin,onlightpin,offlightpin,buz),
        mUDP(udp),
        mIP(ip)
        {
        }

    void setup()
    {
        lightyswitch::setup();
    }

    void loop()
    {
        lightyswitch::loop();
    }

    bool ison() const
    {
        return lightyswitch::ison();
    }

    void Enable() 
    {
      lightyswitch::Enable();
    }

    void ShutDown()
    {
      lightyswitch::ShutDown();
    }


    private:
        udpbro * mUDP;
        IPAddress mIP;
};




#endif