#ifndef __SWITCH4_H
#define __SWITCH4_H

typedef enum
{
    kState_Auto=0,
    kState_TomInBed=1,
    kState_TomAsleep=2,
    kState_TomAwake=3,
    kState_Undefined=4,
} tSwitch4State;


class switch4
{
    public:
        switch4(int s1pin, int s2pin, int s3pin, int s4pin) :
            mState(kState_Undefined), mHasChanged(true)
        {
            mPins.push_back(s1pin);
            mPins.push_back(s2pin);
            mPins.push_back(s3pin);
            mPins.push_back(s4pin);
        }

        void setup()
        {
            for (auto pin : mPins)
                pinMode(pin, INPUT_PULLUP); 
        }

        tSwitch4State loop()
        {
            for (unsigned int i=0;i<mPins.size();++i)
            {
                bool on = (digitalRead(mPins[i])==LOW);    
                if (on)
                {
                    mState = static_cast<tSwitch4State>(i);
                    break; // break out of for loop.
                }
            }
            return mState;
        }

        bool hasChanged()
        {
            return mHasChanged;
        }

        tSwitch4State getSate()
        {
            return mState;
        }



    private:
        std::vector<int> mPins;
        tSwitch4State mState;
        bool mHasChanged;
};


#endif