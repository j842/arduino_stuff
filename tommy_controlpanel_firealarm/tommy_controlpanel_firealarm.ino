#define RED 7
#define BUZZER 8
#define BUTTON 9

#include "jbuzzer.h"
#include "jbutton.h"
#include "jled.h"

jbutton button_alarm(BUTTON);
jbuzzer buzzer1(BUZZER);
jled led1(RED);

jbutton button_blue(BUTTON);
jbutton button_red(BUTTON);


class jpot {
  public:
    jpot(int pin) : mPin(pin) {}
    void setup() { pinMode(mPin, INPUT); }
    float getval() { uint16_t t = analogRead(mPin); return (float)t / 1024.0;}

private:
    int mPin;
};

jpot pot1(A0);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  
  button_alarm.setup();
  buzzer1.setup();
  led1.setup(); // starts off.

  button_blue.setup();
  button_red.setup();
  pot1.setup();
}

class jtimer
{
  public:
    jtimer() : mMaxSecs(120) { stoptimer(); }

    void stoptimer()
    {
      mRunning=false; mFinishMillis=0; mTimeSecs=0; mPlaying=false;
    }

    void loop_running()
    {
      if (button_red.state()==kPressed)
          stoptimer();
      else
        if (!mPlaying)
        { 
          int secsr = (mFinishMillis-millis())/1000;
          if (secsr>=0)
            show(secsr);
          if (secsr==0)
          {
            buzzer1.playsong(6,true); // repeat
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
      int s = (int)(pot1.getval() * mMaxSecs); // 120 secs max.
      if (s!=mTimeSecs)
      {
        mTimeSecs = s;
        show(mTimeSecs);
      }

      if (button_blue.state()==kPressed)
      { // start timer.
        mRunning = true;
        mFinishMillis = millis() + 1000*(uint32_t)(mTimeSecs);
      }
    }

    void show(int val)
    {
      
    }

    bool mRunning;
    bool mPlaying;
    uint32_t mFinishMillis;
    int mTimeSecs;
    int mMaxSecs;
};

void loop() 
{
static jtimer timer;
  
  switch (button_alarm.state())
  {
    case kPressed:
      led1.seton(true);
      buzzer1.playsong(6,true); // repeat
      break; 
    case kReleased:
      buzzer1.finish();
      led1.seton(false);
      break;
    default: 
      ;
  }   

  // timer!
  timer.loop();
    
  buzzer1.loop();
}
  
    

