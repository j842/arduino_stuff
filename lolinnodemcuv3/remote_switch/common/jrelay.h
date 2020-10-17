#ifndef __JRELAY_H
#define __JRELAY_H

class jrelay {
    public:
        jrelay(int pin) : mPin(pin), mOn(false) {}

        void setup() {
            pinMode(mPin,OUTPUT);
        }

        void set(bool ison) 
        {
            mOn=ison;
            digitalWrite(mPin,ison? HIGH : LOW);
        }

        bool get()
        {
            return mOn;
        }

        void loop() {

        }

    private:
        int mPin;
        bool mOn;
};


#endif