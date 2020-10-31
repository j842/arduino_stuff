#ifndef __JLED_H
#define __JLED_H

typedef enum 
{
  kled_on,
  kled_off,
  kled_fading,
  kled_flashing
} tled_state;

class jled {
  public:
    jled(int pin);
    void setup();
    void loop();
    void seton(bool on);
    void setbrightness(int f);
    void fade();
    void flash(int interval=250, bool phase=false);
    tled_state getstate() const;
  private:
    void loop_flashing();
    void loop_fading();
    void _lightUp(bool turnon);
    const int mPin;
    bool mIsOn;
    tled_state mState;

    int mFlashInterval;

    unsigned long mFlashFinish;
    bool mFlashLastPhase;
    unsigned long mFadeFinish;
    static const int kFadeTime = 3000; // 3 secs.
};



#if defined ARDUINO_ARCH_ESP8266

// ----------- ESP8266 ----------------
static const int kMaxBright = 1023; 

#elif defined ARDUINO_ARCH_ESP32

// ---------- ESP32 -------------------

// needs library:
// 	erropix/ESP32 AnalogWrite@^0.2
#include <analogWrite.h>

static const int kMaxBright = 255;

#endif

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


jled::jled(int pin) : 
  mPin(pin),
  mIsOn(false),
  mState(kled_off),
  mFlashInterval(255),
  mFadeFinish(0)
  {
  }

void jled::setup() 
  { 
    pinMode(mPin,OUTPUT);
    seton(false);
  }

void jled::seton(bool on) 
{ 
  mState = (on ? kled_on : kled_off);
  _lightUp(on);
}

void jled::loop() 
{
  switch (mState)
    {
      case kled_flashing:
        loop_flashing();
        break;
      case kled_fading:
        loop_fading();
        break;
      default:
        break;
    }
}

void jled::loop_flashing()
{
  unsigned long now = millis();
  if (now>(mFlashFinish+mFlashInterval))
  {
    mFlashFinish = now;
    mFlashLastPhase = !mFlashLastPhase;
    _lightUp(mFlashLastPhase);
  }
}

void jled::loop_fading()
{
  if (millis()>mFadeFinish)
  {
    seton(false);
    mState=kled_off;
    mFadeFinish=0;
  }
  else
  {
    float numerator = kMaxBright*(mFadeFinish-millis());
    float denominator = kFadeTime;
    setbrightness(numerator/denominator);
  }
}


void jled::fade()
{
  mState=kled_fading;
  mFadeFinish=millis()+kFadeTime;
}

void jled::setbrightness(int f) 
{
  if (f<0) 
    f=0; 
  if (f>kMaxBright) 
    f=kMaxBright; 
  analogWrite(mPin,f);
}

tled_state jled::getstate() const
{
  return mState;
}


void jled::flash(int interval, bool phase)
{
  mFlashInterval=interval;
  mFlashFinish=0;
  mFlashLastPhase=phase;

  mState=kled_flashing;
}

void jled::_lightUp(bool turnon)
{
  if (turnon)
    setbrightness(kMaxBright);
  else
    setbrightness(0);
}


#endif