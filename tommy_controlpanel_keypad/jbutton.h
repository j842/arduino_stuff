typedef enum {
  kUnchanged=0,
  kPressed,
  kReleased
} kState;

#define DEBOUNCE_MS (50)

class jbutton 
{
  public:
    jbutton(int pin) : mPin(pin), mState(HIGH), mLastChange(0) {}
    void setup() {  pinMode(mPin, INPUT_PULLUP); }
    
    kState state() { 
      int newstate = digitalRead(mPin);

      if (newstate==mState || (millis()-mLastChange <= DEBOUNCE_MS))
        return kUnchanged;

      mState = newstate;  
      mLastChange = millis();

      return ((mState == LOW) ? kPressed : kReleased);
    }

    int mState;
    int mPin;
    unsigned long mLastChange;
};
