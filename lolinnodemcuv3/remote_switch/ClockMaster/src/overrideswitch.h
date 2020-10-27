#ifndef __OVERRIDE_SWITCH_H
#define __OVERRIDE_SWITCH_H

#include <iostream>

#include <jrotaryswitch.h>
#include <jswitch.h>
#include <j16x2lcd.h>

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
            mLCD.setup();
            mSwitch1.setup();
            mSwitch4.setup();
        }

        void loop(const DateTime & now)
        {
            mLCD.loop();
            mSwitch1.loop();
            mSwitch4.loop();

            displaystrings s;
            tSwitch4State state = getState(now);
            s.set(statename(state,!mSwitch1.ison()),totime(now));
            mLCD.message(s);
        }

    private:
        const std::string statename(tSwitch4State s, bool autover)
        {
            switch (s)
            {                      // 0123456789012345   // 0123456789012345
                case kState_JackInBed:
                    return autover ? "Auto:   Jack Bed"  : "Manual: Jack Bed";
                case kState_TomInBed:                      
                    return autover ? "Auto:    Tom Bed"  : "Manual:  Tom Bed";
                case kState_AllOff:                     
                    return autover ? "Auto:    All Off"  : "Manual:  All Off";
                case kState_AllOn:
                    return autover ? "Auto:     All On"  : "Manual:   All On";
                default:                                  
                    return autover ? "Auto:      Error"  : "Manual:    Error"; 
            }    
        }

        std::string totime(const DateTime & now)
        {
            return tostr(now.twelveHour(),false) + ":" 
                 + tostr(now.minute()) + " "
                 + std::string(now.isPM() ? "pm":"am");
        }

        tSwitch4State getState(const DateTime & now) const
        {
            if (mSwitch1.ison()) 
                return switch4state();
            // determine based on time.
            return kState_JackInBed;
        }

    private:
        tSwitch4State switch4state() const
        {
            return static_cast<tSwitch4State>(mSwitch4.getState());
        }

        static std::string tostr(int x, bool pad=true) 
            {
            char s[10];
            sprintf(s, pad ? "%02d" : "%d" ,x);
            return s;
            }

    private:
        jswitch mSwitch1;
        jrotaryswitch mSwitch4;
        j16x2lcd mLCD;
};

#endif