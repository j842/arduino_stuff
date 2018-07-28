
// timer made up of buzzer, pot, MAX7219 display and red (stop) + blue (start) buttons.

class jtimer
{
  public:
    jtimer(jbuzzer * buz, jtext * txt, jpot * pot, jbutton * butred, jbutton * butblue) : 
    mBuzzer(buz), mText(txt), mPot(pot), mButton_Red(butred), mButton_Blue(butblue), mMaxSecs(120) { stoptimer(); }

    void setup()
    {
    }

    void stoptimer()
    {
      mRunning=false; mFinishMillis=0; mTimeSecs=0; mPlaying=false;
    }

    void loop_running()
    {
      if (mButton_Red->state()==kPressed)
      {
          stoptimer();
          mBuzzer->finish();
      }
      else
        if (!mPlaying)
        { 
          int secsr = (mFinishMillis-millis())/1000;
          if (secsr>=0)
            show(secsr);
          if (secsr==0)
          {
            mBuzzer->playsong(7,true); // repeat
            mText->printText("Done!");
            mPlaying=true;
          }        
        }
    }

    void loop() {
      if (mRunning)
        loop_running();
      else
        loop_notrunning();
    }

    void loop_notrunning()
    {
      uint32_t s = 5+ (uint32_t)(mPot->getval())*mMaxSecs/1024; // 120 secs max. Round to nearest 10s.
      if (s>mMaxSecs) s=mMaxSecs;

      if (s!=mTimeSecs)
       {
          mTimeSecs = s;
          show(mTimeSecs);
        }

      if (mButton_Blue->state()==kPressed)
      { // start timer.
        mRunning = true;
        mFinishMillis = millis() + 1000*mTimeSecs;
      }
    }

    void show(uint32_t val)
    {
      String s(val);
      s += " s";
      mText->printText(s.c_str());
    }

    private:
      bool mRunning;
      bool mPlaying;
      uint32_t mFinishMillis;
      uint32_t mTimeSecs;
      uint32_t mMaxSecs;

      jbuzzer * mBuzzer;
      jtext * mText;
      jpot * mPot;
      jbutton * mButton_Red;
      jbutton * mButton_Blue;
};