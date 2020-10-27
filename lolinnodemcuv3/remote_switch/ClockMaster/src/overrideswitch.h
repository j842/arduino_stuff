#ifndef __OVERRIDE_SWITCH_H
#define __OVERRIDE_SWITCH_H

#include <iostream>

#include <jrotaryswitch.h>
#include <jswitch.h>

typedef enum
{
    kState_JackInBed=0,
    kState_TomInBed=1,
    kState_AllOff=2,
    kState_AllOn=3,
    kState_Undefined=-1,
} tSwitch4State;


class overrideswitch
{
    public:
        overrideswitch(int switchpin,
                        std::vector<int> pins)
                        :
                        mSwitch1(switchpin),
                        mSwitch4(pins)
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
                case kState_AllOff:                     
                    return autover ? "Auto: All Off"   : "Manual: All Off";
                case kState_AllOn:
                    return autover ? "Auto: All On"    : "Manual: All On";
                default:                                  
                    return autover ? "Auto: Error"     : "Manual: Error"; 
            }    
        }

        // returns true if message has changed.
        bool getMessage(displaystrings &s, const DateTime & now)
        {
            tSwitch4State state = getState(now);
            s.set(statename(state,!mSwitch1.ison()),
                    "    " + tostr(now.hour()) + ":" + tostr(now.minute()) + ":" + tostr(now.second()));
            return true;
        }

        tSwitch4State getState(const DateTime & now)
        {
            if (mSwitch1.ison()) 
                return static_cast<tSwitch4State>(mSwitch4.getState());

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
        jrotaryswitch mSwitch4;
};

#endif