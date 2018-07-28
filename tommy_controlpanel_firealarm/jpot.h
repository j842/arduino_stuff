class jpot {
  public:
    jpot(int pin) : mPin(pin) {}
    void setup() { pinMode(mPin, INPUT); }
    float getval() { uint16_t t = analogRead(mPin); return (float)t / 1024.0;}
    int getval_int() {int t = analogRead(mPin); return t;}

private:
    int mPin;
};
