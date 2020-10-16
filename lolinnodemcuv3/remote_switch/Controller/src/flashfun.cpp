#include <Arduino.h>

#include <vector>
#include <bossmain.h>
#include <flashfun.h>

// make turning the bossmain switch on or off fancy :-)
flashfun::flashfun(const bossmain & bm, std::vector<lightyswitch> & lsv, jbuzzer & jbuz) : 
    mState(ff_NotRunning),mNdx(0),mNextTime(0),mPrevBoss(false),
    mSwitches(lsv),mBossMain(bm),mBuz(jbuz) {}


void flashfun::loop()
{
  if (mBossMain.ison() != mPrevBoss)
    {
      start(mBossMain.ison());
      mPrevBoss = mBossMain.ison();
    }


  if (mState==ff_NotRunning || (millis()<mNextTime)) return;

  // flashyfun
  getSwitch().changelightymode(kls_00);
  mNdx++;
  if (mNdx<2*mSwitches.size())
  {
    getSwitch().changelightymode(mNdx<mSwitches.size() ? kls_10 : kls_01);
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
    if (mNdx<mSwitches.size()) 
      return mSwitches[mNdx]; 
    else 
      return mSwitches[(2*mSwitches.size()-1)-mNdx];
  }


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
    mState=ff_NotRunning;
    for (auto & s : mSwitches) s.changelightymode(kls_00);
    mBuz.playsong(4);
  }
}