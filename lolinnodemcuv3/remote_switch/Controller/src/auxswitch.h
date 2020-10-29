#ifndef __AUXSWITCH_H
#define __AUXSWITCH_H

#include <jbuf.h>

typedef enum
{
  kls_undefined,
  kls_waiting,
  kls_confirmed,
  kls_timeout,
  kls_fault,
} tstate;

class auxswitch : protected lightyswitch
{
    public:
    auxswitch(int switchpin, int onlightpin, int offlightpin, jbuzzer & buz, udpbro * udp, IPAddress ip) :
        lightyswitch(switchpin,onlightpin,offlightpin),
        mUDP(udp),
        mIP(ip),
        mState(kls_undefined),
        mShutdown(true),
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
        if (mState==kls_fault)
        {
            int x = ((millis()/111)%2);
            (x==0) ? setlights(false,false) : setlights(true,true);
        }
        if (mState==kls_waiting)
            {
                int x = (((mConfirmationTimeout - millis())/333)%2);
                (x==0) ? setlights(false,false) : setlights(true,true);

                if (millis()<mConfirmationTimeout)
                    return; // nothing else to do.

                shutdown();
                mBuz.playsong(7);
                mState = kls_timeout;
            }

        if (mShutdown)
            return;

        bool wason = ison();

        lightyswitch::loop();

        if (ison()!=wason) // state changed.
        {
            indicate();
            sendcmd();
        }
    }

    void indicate()
    {
        mConfirmationTimeout = millis() + 3000; // 3 sec confirmation.
        mState = kls_waiting;
    }

    void sendcmd()
    {
        jbuf buf;
        if (mShutdown)
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

    void ShutDown(bool shutdown)
    {
        mShutdown = shutdown;
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
            if (mShutdown) 
                shutdown();
            else
                setlights(buf.getBool(), !buf.getBool());
            mBuz.playsong( buf.getBool() ? 6 : 7);
        }
    }

    private:
        udpbro * mUDP;
        IPAddress mIP;
        tstate mState;

        //bool mPower;
        bool mShutdown;
        bool mOverride;

        unsigned long mConfirmationTimeout;

        jbuzzer & mBuz;
};




#endif