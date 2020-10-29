#ifndef __BOSSMAIN_H
#define __BOSSMAIN_H

class bossmain : protected lightyswitch
{
    public:
    bossmain(int switchpin, int onlightpin, int offlightpin, jbuzzer & buz) :
        lightyswitch(switchpin,onlightpin,offlightpin),
        mIsFading(false),
        mNextTime(0),
        mBuz(buz)
        {
        }

    void setup()
    {
        lightyswitch::setup();
    }

    void loop()
    {
        bool prev = lightyswitch::ison();

        lightyswitch::loop();

        if (prev != ison())
        {
            if (ison())
                lightyswitch::enable();
            else
            {
                mIsFading = true;
                mNextTime = millis() + kNextTime;
                mBuz.playsong(4);
            }
        }

        if (!mIsFading)
            return;
        
        if (millis()>=mNextTime)
        {
            mIsFading=false;
            mNextTime=0;
            lightyswitch::shutdown();
        }
    }

    bool ison() const
    {
        return lightyswitch::ison();
    }

    private:
        bool mIsFading;
        unsigned long mNextTime;
        static const int kNextTime=3000; // 3 secs
        jbuzzer & mBuz;
};


#endif
