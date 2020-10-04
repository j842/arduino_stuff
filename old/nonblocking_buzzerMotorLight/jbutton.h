class jbutton 
{
  public:
    jbutton(int pin) : mPin(pin), mState(HIGH) {}
    void setup() {  pinMode(mPin, INPUT_PULLUP); }
    bool pressed() { 
      if (digitalRead(mPin)==LOW)
        mState=LOW;
      else // high
        if (mState==LOW && (millis()-mLastPush>200))
        {
          mState = HIGH;
          mLastPush = millis(); // debounce
          return true;
        }
      return false;
    }

    int mState;
    int mPin;
    unsigned long mLastPush=0;
};
