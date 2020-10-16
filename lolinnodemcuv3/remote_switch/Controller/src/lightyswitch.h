#ifndef __LIGHTYSWITCH_H
#define __LIGHTYSWITCH_H


#include <jswitch.h>
#include <jled.h>
#include <jbuzzer.h>

typedef enum
{ // on,off
  kls_switch_enabled,
  kls_00, 
  kls_10,
  kls_01,
  kls_11
} tlightymode;


class lightyswitch 
{
  public:
    lightyswitch(int switchpin, int onlightpin, int offlightpin, jbuzzer & buz) :
      mSwitch(switchpin),
      mOnLight(onlightpin),
      mOffLight(offlightpin),
      mMode(kls_switch_enabled),
      mBuz(buz),
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
          if (ison())
            mBuz.playsong(6);
          else
            mBuz.playsong(7);
        }
    }

    // override light behaviour.
    void changelightymode(tlightymode newmode)
    {
      mMode = newmode;
      switch (newmode)
      {
        case kls_00:
          setlights(false,false); break;
        case kls_01:
          setlights(false,true); break;
        case kls_10:
          setlights(true,false); break;
        case kls_11:
          setlights(true,true); break;

        default:
          updatelightsauto(); break;
      };
    }

    bool ison() const
    {
        return mSwitch.ison();
    }

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

    jswitch mSwitch;
    jled mOnLight;
    jled mOffLight;
    tlightymode mMode;
    jbuzzer & mBuz;

    bool mPrevOn;
};


#endif