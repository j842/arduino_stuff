typedef enum {
  kUnchanged=0,
  kPressed,
  kReleased
} kState;

class jbutton 
{
  public:
    jbutton(int pin) : mPin(pin), mState(HIGH), mTriggered(false) {}
    void setup() {  pinMode(mPin, INPUT_PULLUP); }
    
    kState state() { 
      int newstate = digitalRead(mPin);

      if (newstate!=mState)
      {
        mState = newstate;  
        bool down = (mState==LOW);

        if (down && (millis()-mLastPush>200))
          {
            mTriggered = true;
            mLastPush = millis();
            return kPressed;
          }
        else if (!down && mTriggered)
        {
          mTriggered = false;
          return kReleased;        
        }
      }
      return kUnchanged;
    }

    int mState;
    int mPin;
    unsigned long mLastPush=0;
    bool mTriggered;
};
