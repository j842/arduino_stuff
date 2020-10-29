#ifndef __LIGHTYSWITCH_H
#define __LIGHTYSWITCH_H


typedef enum
{ // on,off
  kls_switch_enabled,
  kls_shutdown
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
          setlights(mSwitch.ison(), !mSwitch.ison());
    }

    bool ison() const
    {
        return mSwitch.ison();
    }

    void shutdown()
    {
      mMode = kls_shutdown;
      setlights(false,false);
    }

    void enable()
    {
      mMode = kls_switch_enabled;
      setlights(mSwitch.ison(), !mSwitch.ison());
    }

  protected:
    void setlights(bool onl, bool offl)
    {
      mOnLight.seton(onl);
      mOffLight.seton(offl);
    }

  private:
    jswitch mSwitch;
    jled mOnLight;
    jled mOffLight;
    tlightymode mMode;
};


#endif