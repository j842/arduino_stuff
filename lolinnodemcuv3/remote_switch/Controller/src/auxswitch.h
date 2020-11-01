#ifndef __AUXSWITCH_H
#define __AUXSWITCH_H

#include <jbuf.h>

class auxswitch : protected lightyswitch
{
    public:
    auxswitch(int switchpin, int onlightpin, int offlightpin, jbuzzer & buz, udpbro * udp, IPAddress ip) :
        lightyswitch(switchpin,onlightpin,offlightpin),
        mUDP(udp),
        mIP(ip),
        mBuz(buz),
        mConfirmationTimeout(0)
        {
        }

    void setup()
    {
        lightyswitch::setup();
    }

    void loop()
    {
        bool wason = lightyswitch::ison();
        lightyswitch::loop();
        if (lightyswitch::ison()!=wason && lightyswitch::getMode()!=kls_shutdown) // state changed.
        {
            Serial.printf("Physical switch changed to: %s\n",lightyswitch::ison() ? "on" : "off");

            mBuz.playsong( ison() ? 6 : 7);
            indicate();
            _sendcmd();
        }

        if (lightyswitch::isFlashing())
            if (millis()>mConfirmationTimeout)
            {
                lightyswitch::stopFlashing();
                lightyswitch::setlights(false,false); // turn off lights - aux is unreachable.
                mBuz.playsong(7);
            }
    }

    void indicate()
    {
        mConfirmationTimeout = millis() + 3000; // 3 sec confirmation.
        lightyswitch::flash();
    }

    // shutdown the auxswitch.
    void ShutDown(bool shutdown)
    {
        bool isShutdown = (lightyswitch::getMode()==kls_shutdown);
        if (isShutdown==shutdown)
            return;

        Serial.printf("Shutdown state changed to: %s\n",shutdown ? "SHUTDOWN" : "Enable");

        if (shutdown)
            lightyswitch::shutdown_instant();
        else
            lightyswitch::enable();

        indicate();
        _sendcmd();
    }

    const IPAddress getIP() const {return mIP;}

    void handleUDP(const jbuf & buf)
    {
        switch (buf.getID())
        {
            case kReq_Power:
            {
                Serial.printf("Got kReq_Power from %s\n",mIP.toString().c_str());
                _sendcmd();
                break;
            }

            case kStat_Power: // client sending us their status. Display on LEDs.
                {
                Serial.printf("Got kState_Power from %s - power %s\n",mIP.toString().c_str(), buf.getBool() ? "ON" : "OFF");

                if (lightyswitch::isFlashing())
                    lightyswitch::stopFlashing();

                if (lightyswitch::getMode()==kls_normal)
                    lightyswitch::setlights(buf.getBool(), !buf.getBool()); // override the lights shown by the switch!
                }

            default:
                break;
        }
    }

    private:
        void _sendcmd()
        {
            jbuf buf;
            if (getMode()==kls_shutdown)
            {
                Serial.printf("Telling %s to shutdown.\n",mIP.toString().c_str());
                buf.setIDOnly(kCmd_Shutdown);
            }
            else 
            {
                Serial.printf("Telling %s to turn %s\n",mIP.toString().c_str(), ison() ? "ON" : "OFF");
                buf.setBool(kCmd_Power,ison());
            }

            mUDP->send(buf,mIP);
        }

    private:
        udpbro * mUDP;
        IPAddress mIP;
        jbuzzer & mBuz;

        unsigned long mConfirmationTimeout;
};




#endif