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
        mOverride(false),
        mConfirmationTimeout(0)
        {
        }

    void setup()
    {
        lightyswitch::setup();
    }

    void loop()
    {
        bool wason = ison();
        lightyswitch::loop();
        if (ison()!=wason && lightyswitch::getMode()!=kls_shutdown) // state changed.
        {
            mBuz.playsong( ison() ? 6 : 7);
            indicate();
            sendcmd();
        }

        if (lightyswitch::getMode()==kls_flashing)
            if (millis()>mConfirmationTimeout)
            {
                lightyswitch::enable();
                lightyswitch::setlights(false,false); // turn off lights - aux is unreachable.
                mBuz.playsong(7);
            }
    }

    void indicate()
    {
        mConfirmationTimeout = millis() + 3000; // 3 sec confirmation.
        lightyswitch::flash();
    }

    void sendcmd()
    {
        jbuf buf;
        if (mOverride || getMode()==kls_shutdown)
            buf.setIDOnly(kCmd_Shutdown);
        else
            buf.setBool(kCmd_Power,ison());

        mUDP->send(buf,mIP);
    }

    bool ison() const
    {
        return lightyswitch::ison();
    }

    void Override(bool override)
    {
        mOverride = override;
        sendcmd(); // temp until ClockMaster sends this itself.
    }

    // shutdown the auxswitch.
    void ShutDown(bool shutdown)
    {
        if (shutdown)
            lightyswitch::shutdown_instant();
        else
            lightyswitch::enable();

        indicate();
        sendcmd();
    }

    const IPAddress getIP() const {return mIP;}

    void handleUDP(const jbuf & buf)
    {
        switch (buf.getID())
        {
            case kReq_Power:
                sendcmd();
                break;

            case kStat_Power: // client sending us their status. Display on LEDs.
                {
                if (lightyswitch::getMode()==kls_shutdown)
                    break; // stay shut down.

                if (lightyswitch::getMode()==kls_flashing)
                    lightyswitch::enable(); // stop flashing before continuing.

                lightyswitch::setlights(buf.getBool(), !buf.getBool()); // override the lights shown by the switch!
                }

            default:
                break;
        }
    }

    private:
        udpbro * mUDP;
        IPAddress mIP;
        jbuzzer & mBuz;

        bool mOverride;

        unsigned long mConfirmationTimeout;
};




#endif