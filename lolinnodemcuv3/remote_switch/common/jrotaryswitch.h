#ifndef __SWITCH4_H
#define __SWITCH4_H

class jrotaryswitch
{
    public:
        jrotaryswitch(std::vector<int> pins) :
            mPins(pins),
            mState(-1) // undefined
        {
        }

        void setup()
        {
            for (auto pin : mPins)
                pinMode(pin, INPUT_PULLUP); 
        }

        void loop()
        {
            for (unsigned int i=0;i<mPins.size();++i)
            {
                bool on = (digitalRead(mPins[i])==LOW);    
                if (on)
                    mState = i;
            }
        }

        int getState()
        {
            return mState;
        }

    private:
        std::vector<int> mPins;
        int mState;
};


#endif