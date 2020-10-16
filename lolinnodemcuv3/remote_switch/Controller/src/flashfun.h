#ifndef __FLASHFUN_H
#define __FLASHFUN_H

typedef enum {
  ff_NotRunning,
  ff_RunningFlashy
} tffstate;

// make turning the bossmain switch on or off fancy :-)
class flashfun
{
  public:
    flashfun(const bossmain & bm, std::vector<lightyswitch> & lsv, jbuzzer & jbuz);
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
    const bossmain & mBossMain;
    jbuzzer & mBuz;

    static const int kNextTime=100; // 100 milliseconds.
};

#endif