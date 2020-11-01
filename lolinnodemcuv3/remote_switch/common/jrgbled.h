#ifndef __JRBGLED_H
#define __JRBGLED_H

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


class rgb1023
{
    public:
        rgb1023() : mR(0),mG(0),mB(0) {}
        rgb1023(int r, int g, int b) : mR(r),mG(g),mB(b) {clip();}
        rgb1023(const rgb1023 & other) : mR(other.mR),mG(other.mG),mB(other.mB) {clip();}

        int getval(const int x, const int m) const {return m==1023 ? x : x*m/1023;}

        int getR(int maxval=1023) const {return getval(mR,maxval);}
        int getG(int maxval=1023) const {return getval(mG,maxval);}
        int getB(int maxval=1023) const {return getval(mB,maxval);}

        void clip(int & x) {if (x<0) x=0; if (x>1023) x=1023;}
        void clip()
        {
            clip(mR);
            clip(mG);
            clip(mB);
        }

    public:
        rgb1023 operator+(const rgb1023& other) 
        {
            return rgb1023(mR+other.mR, mG+other.mG, mB+other.mB);
        }

    protected:
        int mR,mG,mB;
};

rgb1023 operator* (const rgb1023 & y,float x)  {return rgb1023(x*y.getR(),x*y.getG(),x*y.getB());}
rgb1023 operator* (float x, const rgb1023 & y) {return y*x;}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


class fadeinfo 
{
    public:
        fadeinfo() : mFading(false), mNextFadeStepTime(0),mFadeTotalTime(0),mFadeStartTime(0) {}

        bool isFading() const {return mFading;}
        void fade(rgb1023 tocol, unsigned long howlong)
        {
            mFading=true;
            mFadeTotalTime = howlong;
            mFadeStartTime = millis();
            mFadeEndColour = tocol;
            mNextFadeStepTime = 0;
        }     

        bool needtoUpdateColour()
        {
            if (!mFading) return false;
            unsigned long now = millis();
            if (now<mNextFadeStepTime) return false;

            float r = (now-mFadeStartTime)/(float)(mFadeTotalTime);
            if (r>1.0) 
            {
                mFading=false;
                mColour = mFadeEndColour;
            }
            else
            {
                mNextFadeStepTime = now + (mFadeTotalTime/30); // 30 steps.
                mColour = ((1.0-r)*mFadeStartColour) + (r*mFadeEndColour);
            }
            return true;
        }   

        void setCurrentColour(const rgb1023 & r) {mFadeStartColour=r;}

        const rgb1023 & getFadeColour() {return mColour;}

    private:
        bool mFading;
        rgb1023 mFadeStartColour,mFadeEndColour;
        unsigned long mNextFadeStepTime;
        unsigned long mFadeTotalTime;
        unsigned long mFadeStartTime;

        rgb1023 mColour;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

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

            setRGB(rgb1023());
        }

        void loop()
        {
            if (mFadeInfo.needtoUpdateColour())
                _setRGB(mFadeInfo.getFadeColour());
        }

        void fade(rgb1023 tocol=rgb1023(0,0,0), unsigned long howlong=3000)
        {
            mFadeInfo.fade(tocol,howlong);
        }

        void setRGB(const rgb1023 & r)
        {
            mFadeInfo.setCurrentColour(r);
            _setRGB(r);
        }
        bool isFading() const 
        {
            return mFadeInfo.isFading();
        }

    private:
        void _setRGB(const rgb1023 & r)
        {
            static const int maxv = 1023; // for ESP8266.
            analogWrite(mR,maxv-r.getR(maxv));
            analogWrite(mG,maxv-r.getG(maxv)); 
            analogWrite(mB,maxv-r.getB(maxv));             
        }

    private:
        int mR,mG,mB;
        fadeinfo mFadeInfo;
};


#endif