// Use the MD_MAX72XX library to scroll text on the display
//
// Demonstrates the use of the callback function to control what
// is scrolled on the display text.
//
// User can enter text on the serial monitor and this will display as a
// scrolling message on the display.
// Speed for the display is controlled by a pot on SPEED_IN analog in.

#include "jscroll.h"

namespace jscroll
{

MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

#define BUFLEN 100

char      mMsg[BUFLEN] = "Hello!";
uint16_t  mScrollDelay;  // in milliseconds
bool      mDone=true;
bool      mReset=false;        


#define CHAR_SPACING  1 // pixels between characters


bool isDone()
{
  return mDone;
}

void scrollDataSink(uint8_t dev, MD_MAX72XX::transformType_t t, uint8_t col)
// Callback function for data that is being scrolled off the display
{
}

uint8_t scrollDataSource(uint8_t dev, MD_MAX72XX::transformType_t t)
// Callback function for data that is required for scrolling into the display
{
  static uint8_t scrollstate = 0;
  static const char *p = jscroll::mMsg;
  static uint8_t  curLen, showLen;
  static uint8_t  cBuf[8];
  uint8_t colData;

  if (mReset)
    {
      showLen = 8*MAX_DEVICES;
      curLen=0;
      scrollstate=2;
      p = mMsg;
      mReset=false;
      mDone=false;
    }

  if (mDone)
    return 0;
    
  // finite state machine to control what we do on the callback
  switch(scrollstate)
  {
    case 0: // Load the next character from the font table
      if (*p=='\0')
      {
        mDone=true;
        return 0;
      } 
      else 
      {    
        showLen = mx.getChar(*p++, sizeof(cBuf)/sizeof(cBuf[0]), cBuf);
        curLen = 0;
        scrollstate = 1;
      }
      
      // !! deliberately fall through to next state to start displaying

    case 1: // display the next part of the character
        colData = cBuf[curLen++];
        if (curLen == showLen)
        {
          showLen = CHAR_SPACING;
          curLen = 0;
          scrollstate = 2;
        }        
      break;

    case 2: // display inter-character spacing (blank column)
      colData = 0;
      curLen++;
      if (curLen == showLen)
          scrollstate = 0;
      break;  
          
    default:
      // should never get here.
      colData = 0;
      scrollstate = 0;
  }

  return(colData);
}

uint16_t getScrollDelay(void)
{
  uint16_t  t;

  t = analogRead(SPEED_IN);
  t = map(sqrt(t), 0, 32, 25, 100);  // 32^2 = 1024. Map to 10ms to 100ms.

  return(t);
}

void loop(void)
{
  static uint32_t prevTime = 0;
  mScrollDelay = getScrollDelay();

  // Is it time to scroll the text?
  if (millis()-prevTime >= mScrollDelay)
  {
    mx.transform(MD_MAX72XX::TSL);  // scroll along - the callback will load all the data
    prevTime = millis();      // starting point for next time
  }
}

void set(const char * s) {
    strlcpy(mMsg,s,BUFLEN);
    mReset = true;
    mDone  = false;
}

void setup()
{
  mx.begin();
  mx.setShiftDataInCallback(scrollDataSource);
  mx.setShiftDataOutCallback(scrollDataSink);

  pinMode(SPEED_IN, INPUT);

  set("Hello!");
}

}
