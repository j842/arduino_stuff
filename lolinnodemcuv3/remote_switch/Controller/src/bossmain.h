#ifndef __BOSSMAIN_H
#define __BOSSMAIN_H

#include <Arduino.h>
#include <lightyswitch.h>

class bossmain : protected lightyswitch
{
    public:
    bossmain(int switchpin, int onlightpin, int offlightpin, jbuzzer & buz) :
        lightyswitch(switchpin,onlightpin,offlightpin,buz),
        mPrevState(false),
        mIsFading(false),
        mNextTime(0)
        {
        }

    void setup()
    {
        lightyswitch::setup();
        mPrevState = ison();
    }

    void loop()
    {
        lightyswitch::loop();

        if (mPrevState != ison())
        {
            mPrevState = ison();
            if (ison())
                lightyswitch::changelightymode(kls_switch_enabled);
            else
            {
                mIsFading = true;
                mNextTime = millis() + kNextTime;
                lightyswitch::mBuz.playsong(4);
            }
        }

        if (mIsFading && millis()>=mNextTime)
        {
            mIsFading=false;
            lightyswitch::changelightymode(kls_00);
        }
    }

    bool ison() const
    {
        return lightyswitch::ison();
    }

    private:
        bool mPrevState;
        bool mIsFading;
        unsigned long mNextTime;
        static const int kNextTime=3000; // 3 secs
};


#endif
