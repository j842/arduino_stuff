#ifndef __JRBGLED_H
#define __JRBGLED_H


// the keyes 140C05 is common anode. ESP8266 has 1023 resolution by default. 
class jrgbled
{
    public:
        jrgbled(int rpin, int gpin, int bpin) :mR(rpin),mG(gpin),mB(bpin)
        {
        }

        void setup()
        {
            pinMode(mR, OUTPUT);
            pinMode(mB, OUTPUT);
            pinMode(mG, OUTPUT);

            analogWrite(mR,1023);
            analogWrite(mB,1023);
            analogWrite(mG,1023);
        }

        void loop()
        {
        }

        void setRGB(int r, int g, int b)
        {
            r = (255-r)*1023/255;
            g = (255-g)*1023/255;
            b = (255-b)*1023/255;
            if (r<0) r=0; if (r>1023) r=1023;
            if (g<0) g=0; if (g>1023) g=1023;
            if (b<0) b=0; if (b>1023) b=1023;

            analogWrite(mR,r);
            analogWrite(mG,g); 
            analogWrite(mB,b); 
        }

    private:
        int mR,mG,mB;
};


#endif