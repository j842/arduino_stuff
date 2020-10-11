#ifndef __JSWITCH_H
#define __JSWITCH_H

// A switch wired so it's open connection when off, and connected to ground when on.
// We use the PULLUP so when open it's connected to +3.3V via pullup resistor.

#define DEBOUNCE_MS (200)


class jswitch 
{
  public:
    jswitch(int pin) : mPin(pin), mIsOn(false), mLastChange(0) {}
    void setup() 
    {  
      pinMode(mPin, INPUT_PULLUP); 
    }

    bool changed_since_last_read()
    {
      return ready() && (_ison()!=mIsOn);
    }
    
    bool ison() { 
      if (changed_since_last_read())
      {
        // changed! Update mLastChange.
        mIsOn = _ison();
        mLastChange = millis();
      }
      return mIsOn;
    }

    bool ready() {
      return (millis()-mLastChange)>DEBOUNCE_MS;
    }

  private:
    bool _ison()
    {
        return (digitalRead(mPin)==LOW);
    }

    int mPin;
    bool mIsOn;
    unsigned long mLastChange;
};

#endif
