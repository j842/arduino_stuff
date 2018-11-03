
class jrelay
{
  public:
    jrelay(int pin) : mPin(pin), mIsOn(false) {}
    void setup() { pinMode(mPin, OUTPUT);}
    void off() { digitalWrite(mPin, HIGH); mIsOn=false; }
    void on() { digitalWrite(mPin, LOW); mIsOn=true; }
    bool ison() { return mIsOn;}
  private:
    int mPin;
    bool mIsOn;
};