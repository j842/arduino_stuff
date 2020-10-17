#ifndef __LIGHTYSWITCH_H
#define __LIGHTYSWITCH_H


typedef enum
{ // on,off
  kls_switch_enabled,
  kls_switch_disabled_00, 
  kls_switch_disabled_10,
  kls_switch_disabled_01,
  kls_switch_disabled_11
} tlightymode;

class lightyswitch 
{
  public:
    lightyswitch(int switchpin, int onlightpin, int offlightpin, jbuzzer & buz) :
      mBuz(buz),
      mSwitch(switchpin),
      mOnLight(onlightpin),
      mOffLight(offlightpin),
      mMode(kls_switch_enabled),
      mPrevOn(false)
      {
      }

    void setup() 
    {
      mSwitch.setup();
      mOnLight.setup();
      mOffLight.setup();

      changelightymode(kls_switch_enabled);
      updatelightsauto();
    }

    void loop()
    {
      mSwitch.loop();
      mOnLight.loop();
      mOffLight.loop();

      if (mMode==kls_switch_enabled && changed())
        {
          updatelightsauto();
        }
    }


    bool ison() const
    {
        return mSwitch.ison();
    }

    void TurnLightsOff()
    {
      changelightymode(kls_switch_disabled_00);
    }

    void TurnLightsOn()
    {
      changelightymode(kls_switch_enabled);
    }

  protected:
      jbuzzer & mBuz;

  private:
    bool changed()
    {
      return (mPrevOn!=mSwitch.ison());
    }
    void updatelightsauto()
    {
      mPrevOn=mSwitch.ison();
      setlights(mSwitch.ison(), !mSwitch.ison());
    }

    void setlights(bool onl, bool offl)
    {
      mOnLight.seton(onl);
      mOffLight.seton(offl);
    }

    
    // override light behaviour.
    void changelightymode(tlightymode newmode)
    {
      mMode = newmode;
      switch (newmode)
      {
        case kls_switch_disabled_00:
          setlights(false,false); break;
        case kls_switch_disabled_01:
          setlights(false,true); break;
        case kls_switch_disabled_10:
          setlights(true,false); break;
        case kls_switch_disabled_11:
          setlights(true,true); break;

        default:
          updatelightsauto(); break;
      };
    }

    jswitch mSwitch;
    jled mOnLight;
    jled mOffLight;
    tlightymode mMode;

    bool mPrevOn;
};


#endif