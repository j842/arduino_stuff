
class jled {
  public:
  jled(int pin) : mPin(pin), mOn(false) {}
  void setup() { pinMode(mPin,OUTPUT); mOn=true; seton(false);}
  void seton(bool on) { if (mOn!=on) digitalWrite(mPin, on?HIGH:LOW); mOn=on;}
  void loop() {}
  private:
    int mPin;
    bool mOn;
};
