
#include <jswitch.h>
#include <jled.h>
#include <jbuzzer.h>

typedef enum
{ // on,off
  kls_auto,
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
      mMode(kls_00),
      mBuz(buz)
      {
      }

    void setup() 
    {
      mSwitch.setup();
      mOnLight.setup();
      mOffLight.setup();

      changelightymode(kls_00);
    }

    void loop()
    {
      mSwitch.loop();
      mOnLight.loop();
      mOffLight.loop();

      if (mMode==kls_auto)
        if (changed_since_last_read())
        {
          updatelightsauto();
          if (ison())
            mBuz.playsong(6);
          else
            mBuz.playsong(7);
        }
    }

    void updatelightsauto()
    {
      setlights(mSwitch.ison(), !mSwitch.ison());
    }

    void setlights(bool onl, bool offl)
    {
      mOnLight.seton(onl);
      mOffLight.seton(offl);
    }

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

    bool ison()
    {
        return mSwitch.ison();
    }

  private:
    bool changed_since_last_read()
    {
        return mSwitch.changed_since_last_read();
    }   

    jswitch mSwitch;
    jled mOnLight;
    jled mOffLight;
    tlightymode mMode;
    jbuzzer & mBuz;
};
