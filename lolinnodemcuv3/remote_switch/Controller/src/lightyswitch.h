#ifndef __LIGHTYSWITCH_H
#define __LIGHTYSWITCH_H


typedef enum
{ // on,off
  kls_switch_enabled,
  kls_shutdown,
  kls_flashing
} tlightymode;

class lightyswitch 
{
  public:
    lightyswitch(int switchpin, int onlightpin, int offlightpin) :
      mSwitch(switchpin),
      mOnLight(onlightpin),
      mOffLight(offlightpin),
      mMode(kls_switch_enabled)
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

      if (mMode==kls_switch_enabled && mSwitch.ison()!=prevon)
        enable(); // updates lights.
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
      // if (mOnLight.getstate()==kled_on)
        mOnLight.fade();
      // if (mOffLight.getstate()==kled_on)
        mOffLight.fade();
    }

    void enable()
    {
      mMode = kls_switch_enabled;
      mOnLight.seton(mSwitch.ison());
      mOffLight.seton(!mSwitch.ison());
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

    tlightymode getMode() const {return mMode;}

  protected:
    jswitch mSwitch;
    jled mOnLight;
    jled mOffLight;
    tlightymode mMode;
};


#endif