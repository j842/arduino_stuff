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
        lightyswitch(switchpin,onlightpin,offlightpin,buz),
        mUDP(udp),
        mIP(ip),
        mState(kls_undefined),
        mPower(false),
        mShutdown(true),
        mConfirmationTimeout(0)
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
            (x==0) ? TurnLightsOff() : TurnLightsOn();
        }
        if (mState==kls_waiting)
            {
                int x = (((mConfirmationTimeout - millis())/333)%2);
                (x==0) ? TurnLightsOff() : TurnLightsOn();

                if (millis()<mConfirmationTimeout)
                    return; // nothing else to do.

                TurnLightsOff();
                mBuz.playsong(7);
                mState = kls_timeout;
            }

        if (mShutdown)
            return;

        bool wason = ison();
        lightyswitch::loop();

        if (ison()!=wason || ison()!=mPower) // state changed.
        {
            mPower = ison();
            sendcmd_and_indicate();
        }
    }

    void sendcmd_and_indicate()
    {
        mConfirmationTimeout = millis() + 3000; // 3 sec confirmation.
        mState = kls_waiting;
        sendcmd();
    }

    void sendcmd()
    {
        jbuf buf;
        buf.setBool(kCmd_Power,mPower);
        mUDP->send(buf,mIP);
    }

    bool ison() const
    {
        return lightyswitch::ison();
    }

    void Enable() 
    {
        mShutdown = false;
        mPower = ison();
        sendcmd_and_indicate();
    }

    void ShutDown()
    {
      mShutdown = true;
      mPower = false;
      sendcmd_and_indicate();
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
            if (buf.getBool()==mPower)
            {
                mState=kls_confirmed;
                mShutdown ? TurnLightsOff() : TurnLightsOn();
                mBuz.playsong( mPower ? 6 : 7);
            }
            else
                mState=kls_fault;
        }
    }

    private:
        udpbro * mUDP;
        IPAddress mIP;
        tstate mState;

        bool mPower;
        bool mShutdown;

        unsigned long mConfirmationTimeout;
};




#endif