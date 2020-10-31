#ifndef __LIGHTYSWITCH_H
#define __LIGHTYSWITCH_H


typedef enum
{ // on,off
  kls_normal,
  kls_shutdown,
  kls_flashing    // interruptable
} tlightymode;

class lightyswitch 
{
  public:
    lightyswitch(int switchpin, int onlightpin, int offlightpin, bool fadeOff=false) :
      mSwitch(switchpin),
      mOnLight(onlightpin),
      mOffLight(offlightpin),
      mMode(kls_normal),
      mFadeOff(fadeOff)
      {
      }

    void setup() 
    {
      mSwitch.setup();
      mOnLight.setup();
      mOffLight.setup();

      enable();
    }

    void loop()
    {
      bool prevon = mSwitch.ison();

      mSwitch.loop();
      mOnLight.loop();
      mOffLight.loop();

      if (mMode!=kls_shutdown && mSwitch.ison()!=prevon)
        updatelights();
    }

    bool ison() const
    {
        return mSwitch.ison();
    }

    void shutdown_instant()
    {
      mMode = kls_shutdown;
      mOnLight.seton(false);
      mOffLight.seton(false);
    }

    void shutdown_fade()
    {
      mMode = kls_shutdown;
      _fadelight(mOnLight);
      _fadelight(mOffLight);
    }

    void _fadelight(jled & light)
    {
      if (light.getstate()==kled_on)
        light.fade();
      else
        light.seton(false); // not flashing
    }

    void enable()
    {
      mMode = kls_normal;
      updatelights();
    }

    void flash()
    {
      mMode = kls_flashing;
      mOnLight.flash(250,false);
      mOffLight.flash(250,true);
    }

  protected:
    // force the lights to be something other than the switch.
    // used to display the true state of the auxiliaries.
    void setlights(bool onl, bool offl)
    {
      mOnLight.seton(onl);
      mOffLight.seton(offl);
    }

    void updatelights()
    {
      bool ison = mSwitch.ison();

      mOnLight.seton(ison);
      if (mFadeOff && !ison)
        mOffLight.fade();
      else
        mOffLight.seton(!ison);
    }

    tlightymode getMode() const {return mMode;}

  protected:
    jswitch mSwitch;
    jled mOnLight;
    jled mOffLight;
    tlightymode mMode;
    bool mFadeOff;
};


#endif