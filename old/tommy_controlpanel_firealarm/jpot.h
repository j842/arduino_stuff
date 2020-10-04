#define AVERAGETIMEMILLIS 100

class jpot {
  public:
    jpot(int pin) : mPin(pin), mVal(0), mLastPotChange(0), mCount(0), mSum(0) {}
    void setup() { pinMode(mPin, INPUT); }
    int getval() {
      if (millis()-mLastPotChange < AVERAGETIMEMILLIS)
      {
        ++mCount;
        mSum += analogRead(mPin);
        return mVal;
      }

      // can update.
      mVal = (int)(mSum/mCount);
      mCount=0;
      mSum=0;
      mLastPotChange=millis();
      return mVal;
    }

private:
    int mPin;
    uint32_t mLastPotChange;
    int mCount;
    uint64_t mSum;
    int mVal;
};
