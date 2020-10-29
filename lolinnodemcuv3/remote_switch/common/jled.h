
class jled {
  public:
  jled(int pin) : mPin(pin), mOn(false) {}
  void setup() { pinMode(mPin,OUTPUT); mOn=true; seton(false);}
  void seton(bool on) { if (mOn!=on) digitalWrite(mPin, on?HIGH:LOW); mOn=on;}
  //void setfade(int f) {if (f<0) f=0; if (f>1023) f=1023; ledcWrite(mPin,1023-f);}
  void loop() {}
  private:
    int mPin;
    bool mOn;
};
