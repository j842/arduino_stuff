#ifndef __JSWITCH_H
#define __JSWITCH_H

// A switch wired so it's open connection when off, and connected to ground when on.
// We use the PULLUP so when open it's connected to +3.3V via pullup resistor.

#define DEBOUNCE_MS (200)


class jswitch 
{
  public:
    jswitch(int pin) : mPin(pin), mIsOn(false), mLastChange(0) 
    {
    }

    void setup() 
    {  
      pinMode(mPin, INPUT_PULLUP); 
    }

    void loop()
    {
      bool on = (digitalRead(mPin)==LOW);      
      bool ready = ((millis()-mLastChange)>DEBOUNCE_MS);
      if (mLastChange>0)
        if (on==mIsOn || !ready)
          return; // unchanged, or debouncing.

      mIsOn=on;
      mLastChange=millis();
    }
    
    bool ison() const
    { 
      return mIsOn;
    }

  private:
    int mPin;
    bool mIsOn;
    unsigned long mLastChange;
};

#endif
