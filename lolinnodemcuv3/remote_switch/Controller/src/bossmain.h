#ifndef __BOSSMAIN_H
#define __BOSSMAIN_H

#include "lightyswitch.h"

class bossmain : protected lightyswitch
{
    public:
    bossmain(int switchpin, int onlightpin, int offlightpin, jbuzzer & buz) :
        lightyswitch(switchpin,onlightpin,offlightpin,buz)
        {
        }

    void setup()
    {
        lightyswitch::setup();
    }

    void loop()
    {
        lightyswitch::loop();
    }

    bool ison() const
    {
        return lightyswitch::ison();
    }
};


#endif
