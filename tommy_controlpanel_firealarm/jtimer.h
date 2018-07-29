
// timer made up of buzzer, pot, MAX7219 display and red (stop) + blue (start) buttons.

class jtimer
{
  public:
    jtimer(jbuzzer * buz, jtext * txt, jpot * pot, jbutton * butred, jbutton * butblue) : 
    mBuzzer(buz), mText(txt), mPot(pot), mButton_Red(butred), mButton_Blue(butblue) 
    { 
      mDimTime=millis();
      mMaxSecs = 600;
      stoptimer(); 
    }

    void setup()
    {
    }

    void stoptimer()
    {
      mRunning=false; mFinishMillis=0; mTimeSecs=0; mPlaying=false; mStopPlayingMillis=0;
    }

    void loop_running()
    {
      if (mButton_Red->state()==kPressed || (mPlaying && millis()>mStopPlayingMillis))
      {
          stoptimer();
          mBuzzer->finish();
          dim();
          return;
      }

      if (mPlaying)
        return;

      // counter running, not playing.
      int secsr = (mFinishMillis-millis())/1000;
      if (secsr>=0)
        show(secsr);
      if (secsr==0)
      {
        mStopPlayingMillis = millis()+10*1000; // 10 secs max.
        mBuzzer->playsong(7,true); // repeat
        mText->printText("Done!");
        mPlaying=true;
      }        
    }

    void loop() {
      if (mRunning)
        loop_running();
      else
        loop_notrunning();
    }

    void dim()
    {
      mText->setIntensity(0);
      mText->printText("Zzzzz");
      mDimTime=0;
    }

    void loop_notrunning()
    {
      uint32_t s = 1 + (uint32_t)(mPot->getval())*mMaxSecs/10/1024; // 120 secs max. Round to nearest 10s.
      s=s*10;
      if (s>mMaxSecs) s=mMaxSecs;

      if (s!=mTimeSecs)
       {
          mDimTime = millis()+10*1000;
          mTimeSecs = s;
          show(mTimeSecs);
        }
      else if (mButton_Blue->state()==kPressed)
      { // start timer.
        mRunning = true;
        mFinishMillis = millis() + 1000*mTimeSecs;
      }
      else if (mDimTime>0 && millis()>mDimTime)
      {
        dim();
      }
    }

    void show(uint32_t val)
    {
      int mins=0;
      int secs=val;

      while (secs>=60)
        {
          ++mins;
          secs-=60;
        }
      
      String s(" ");

      if (mins>0)
      {
        s += String(mins);
        s += ":";
        if (secs<10) s+="0";
        s += String(secs);
      }
      else
      {
        s += String(secs);
        s += "s";
      }
      mText->setIntensity(8);
      mText->printText(s.c_str());
    }

    private:
      bool mRunning;
      bool mPlaying;
      uint32_t mFinishMillis;
      uint32_t mStopPlayingMillis;
      uint32_t mTimeSecs;
      uint32_t mMaxSecs;
      uint32_t mDimTime;

      jbuzzer * mBuzzer;
      jtext * mText;
      jpot * mPot;
      jbutton * mButton_Red;
      jbutton * mButton_Blue;
};
