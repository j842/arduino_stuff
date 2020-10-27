#ifndef __OVERRIDE_SWITCH_H
#define __OVERRIDE_SWITCH_H

#include <iostream>

#include <jrotaryswitch.h>
#include <jswitch.h>
#include <j16x2lcd.h>
#include <jbuf.h>

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

        tSwitch4State loop(const DateTime & now)
        {
            mLCD.loop();
            mSwitch1.loop();
            mSwitch4.loop();

            displaystrings s;
            tSwitch4State state = getState(now);
            s.set(statename(state,!mSwitch1.ison()),totime(now));
            mLCD.message(s);

            return state;
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
            switch (now.hour())
            {
                case 23:
                case 0: 
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:  return kState_AllOff;

                case 20: return kState_JackInBed;

                case 21: 
                case 22: return kState_TomInBed;

                default: return kState_AllOn;
            }


            if (now.hour()<7 || now.hour()>=23) // before 7am or after 11pm
                return kState_AllOff;
            
            if (now.hour()<20) // before 8pm
                return kState_AllOn;
            
            if (now.hour()<21) // 8-9pm
                return kState_JackInBed;

            return kState_TomInBed;
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