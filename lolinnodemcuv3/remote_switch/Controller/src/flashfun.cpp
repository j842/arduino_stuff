#include <Arduino.h>

#include <vector>
#include <lightyswitch.h>
#include <flashfun.h>

// make turning the bossmain switch on or off fancy :-)
flashfun::flashfun(std::vector<lightyswitch> & lsv, jbuzzer & jbuz) : 
    mState(ff_NotRunning),mNdx(0),mNextTime(0),mPrevBoss(false),mSwitches(lsv),mBuz(jbuz) {}

void flashfun::start(bool bossmainOn)
{
  for (auto & s : mSwitches) s.changelightymode(kls_00);

  mNdx=0;
  if (bossmainOn)
  {
    mState=ff_RunningFlashy;
    mSwitches[0].changelightymode(kls_10);
    mNextTime=millis()+kNextTime;
  }
  else
  { // bossmain off, go to sleep.
    mState=ff_RunningFadeBossMain;
    for (auto & s : mSwitches) s.changelightymode(kls_00);
    mSwitches[0].changelightymode(kls_01);
    mNextTime=millis()+kFadeOffTime;
    mBuz.playsong(4);
  }
}

void flashfun::loop()
{

  if (mSwitches[0].ison() != mPrevBoss)
    {
      if (mSwitches[0].ison())
        start(true);
      else
        start(false);
      mPrevBoss = mSwitches[0].ison();
    }


  if (mState==ff_NotRunning || (millis()<mNextTime)) return;

  if (mState==ff_RunningFadeBossMain)
  { // finish up
    mState=ff_NotRunning;
    mSwitches[0].changelightymode(kls_00);
    return;
  }

  // flashyfun
  getSwitch().changelightymode(kls_00);
  mNdx++;
  if (mNdx<10)
  {
    getSwitch().changelightymode(mNdx<5 ? kls_10 : kls_01);
    mNextTime=millis()+kNextTime;
  }
  else
  { // all done, set to correct values.
    mState=ff_NotRunning;
    for (auto & s : mSwitches) s.changelightymode(kls_switch_enabled);
  }
}

tffstate flashfun::state() {return mState;}
lightyswitch & flashfun::getSwitch() 
  {
    if (mNdx<5) 
      return mSwitches[mNdx]; 
    else 
      return mSwitches[9-mNdx];
  }
