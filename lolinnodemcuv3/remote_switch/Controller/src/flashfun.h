#ifndef __FLASHFUN_H
#define __FLASHFUN_H

typedef enum {
  ff_NotRunning,
  ff_RunningFlashy,
  ff_RunningFadeBossMain
} tffstate;

// make turning the bossmain switch on or off fancy :-)
class flashfun
{
  public:
    flashfun(std::vector<lightyswitch> & lsv, jbuzzer & jbuz);
    void loop();
    tffstate state();

  private:
    void start(bool bossmainOn);
    lightyswitch & getSwitch();

    tffstate mState;
    int mNdx;
    unsigned long mNextTime;
    bool mPrevBoss;

    std::vector<lightyswitch> & mSwitches;
    jbuzzer & mBuz;

    static const int kFadeOffTime=3000; // 3 seconds.
    static const int kNextTime=100; // 100 milliseconds.
};

#endif