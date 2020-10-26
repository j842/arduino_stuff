#ifndef __OVERRIDE_SWITCH_H
#define __OVERRIDE_SWITCH_H

#include <iostream>

#include <switch4.h>
#include <jswitch.h>

class overrideswitch
{
    public:
        overrideswitch(int switchpin,
                        int rotary1pin,
                        int rotary2pin,
                        int rotary3pin,
                        int rotary4pin)
                        :
                        mSwitch1(switchpin),
                        mSwitch4(rotary1pin,rotary2pin,rotary3pin,rotary4pin),
                        mDisplayTime(0)
            {
                
            }

        void setup()
        {
            mSwitch1.setup();
            mSwitch4.setup();
        }

        void loop()
        {
            mSwitch1.loop();
            mSwitch4.loop();
        }

        const std::string statename(tSwitch4State s, bool autover)
        {
            switch (s)
            {                      // 0123456789012345 // 0123456789012345
                case kState_JackInBed:
                    return autover ? "Auto: Jack Bed"  : "Manual: Jack Bed";
                case kState_TomInBed:                      
                    return autover ? "Auto: Tom Bed"   : "Manual: Tom Bed";
                case kState_TomAsleep:                     
                    return autover ? "Auto: All Off"   : "Manual: All Off";
                case kState_AllOn:
                    return autover ? "Auto: Daytime"   : "Manual: Daytime";
                default:                                  
                    return autover ? "Auto: Error"     : "Manual: Error"; 
            }    
        }

        // returns true if message has changed.
        bool getMessage(std::string &s1, std::string &s2, const DateTime & now)
        {
            tSwitch4State state = getState(now);
            s1 = statename(state,!mSwitch1.ison());
            s2 = "    " + tostr(now.hour()) + ":" + tostr(now.minute()) + ":" + tostr(now.second());
            return true;
        }

        tSwitch4State getState(const DateTime & now)
        {
            if (mSwitch1.ison()) 
                return mSwitch4.getState();

            // determine based on time.
            return kState_JackInBed;
        }

    private:
        std::string tostr(int x) 
            {
            char s[10];
            sprintf(s,"%02d",x);
            return s;
            }

    private:
        jswitch mSwitch1;
        switch4 mSwitch4;
        unsigned long mDisplayTime;

};

#endif