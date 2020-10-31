#ifndef __AUXSWITCH_H
#define __AUXSWITCH_H

#include <jbuf.h>

typedef enum
{
  kls_undefined,
  kls_waiting,
  kls_confirmed,
  kls_timeout,
} tstate;

class auxswitch : protected lightyswitch
{
    public:
    auxswitch(int switchpin, int onlightpin, int offlightpin, jbuzzer & buz, udpbro * udp, IPAddress ip) :
        lightyswitch(switchpin,onlightpin,offlightpin),
        mUDP(udp),
        mIP(ip),
        mState(kls_undefined),
        mOverride(false),
        mConfirmationTimeout(0),
        mBuz(buz)
        {
        }

    void setup()
    {
        lightyswitch::setup();
    }

    void loop()
    {
        if (mState==kls_waiting)
        {
            if (millis()<mConfirmationTimeout)
                return; // nothing else to do.

            lightyswitch::shutdown_fade(); // fade lights - aux is unreachable.
            mBuz.playsong(7);
            mState = kls_timeout;
        }

        bool wason = ison();
        lightyswitch::loop();
        if (ison()!=wason && getMode()==kls_switch_enabled) // state changed.
        {
            mBuz.playsong( ison() ? 6 : 7);
            indicate();
            sendcmd();
        }
    }

    void indicate()
    {
        mConfirmationTimeout = millis() + 3000; // 3 sec confirmation.
        mState = kls_waiting;
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
        indicate();
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
        if (buf.getID()==kReq_Power)
        {
            sendcmd();
        }

        if (buf.getID()==kStat_Power)
        {
            mState=kls_confirmed;

            if (lightyswitch::getMode()  == kls_switch_enabled || buf.getBool())
                setlights(buf.getBool(), !buf.getBool()); // override the lights shown by the switch!
            else
                setlights(false,false);
        }
    }

    private:
        udpbro * mUDP;
        IPAddress mIP;
        tstate mState;

        bool mOverride;

        unsigned long mConfirmationTimeout;

        jbuzzer & mBuz;
};




#endif