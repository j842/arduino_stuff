// Use the MD_MAX72XX library to scroll text on the display
//
// Demonstrates the use of the callback function to control what
// is scrolled on the display text.
//
// User can enter text on the serial monitor and this will display as a
// scrolling message on the display.
// Speed for the display is controlled by a pot on SPEED_IN analog in.

#include <MD_MAX72xx.h>
#include <SPI.h>

#define USE_POT_CONTROL 0
#define PRINT_CALLBACK  0

#define PRINT(s, v) { Serial.print(F(s)); Serial.print(v); }



// Define the number of devices we have in the chain and the hardware interface
#define HARDWARE_TYPE MD_MAX72XX::GENERIC_HW
#define MAX_DEVICES 1
#define CLK_PIN   10  // or SCK
#define DATA_PIN  12  // or MOSI
#define CS_PIN    11  // or SS
MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);



// Scrolling parameters
#if USE_POT_CONTROL
#define SPEED_IN  A5
#else
#define SCROLL_DELAY  75  // in milliseconds
#endif // USE_POT_CONTROL

#define CHAR_SPACING  1 // pixels between characters

// Global message buffers shared by Serial and Scrolling functions
#define BUF_SIZE  200
char curMessage[BUF_SIZE];

typedef enum {
  kState_NoMessage = 0,
  kState_Original_Message,
  kState_Clearing,
  kState_Done
} tMsgStateEnum;

tMsgStateEnum msgDone=kState_NoMessage;

uint16_t  scrollDelay;  // in milliseconds


void scrollDataSink(uint8_t dev, MD_MAX72XX::transformType_t t, uint8_t col)
// Callback function for data that is being scrolled off the display
{
#if PRINT_CALLBACK
  Serial.print("\n cb ");
  Serial.print(dev);
  Serial.print(' ');
  Serial.print(t);
  Serial.print(' ');
  Serial.println(col);
#endif
}

uint8_t scrollDataSource(uint8_t dev, MD_MAX72XX::transformType_t t)
// Callback function for data that is required for scrolling into the display
{
  static char   *p = curMessage;
  static uint8_t  state = 0;
  static uint8_t  curLen, showLen;
  static uint8_t  cBuf[8];
  uint8_t colData;

  // finite state machine to control what we do on the callback
  switch(state)
  {
    case 0: // Load the next character from the font table
      showLen = mx.getChar(*p++, sizeof(cBuf)/sizeof(cBuf[0]), cBuf);
      curLen = 0;
      state++;

      // if we reached end of message, reset the message pointer
      if (*p == '\0')
      {
        if (msgDone == kState_Original_Message)
        {
          curMessage[0]='\0';
          int i;
          for (i=0;i<=8*MAX_DEVICES/CHAR_SPACING;++i)
            curMessage[i]=' ';
          curMessage[i]='\0';
          p = curMessage;
          msgDone = kState_Clearing;
        }
        else 
        {
          msgDone = kState_Done;
          p = curMessage;
        }
      }
      // !! deliberately fall through to next state to start displaying

    case 1: // display the next part of the character
      colData = cBuf[curLen++];
      if (curLen == showLen)
      {
        showLen = CHAR_SPACING;
        curLen = 0;
        state = 2;
      }
      break;

    case 2: // display inter-character spacing (blank column)
      colData = 0;
      curLen++;
      if (curLen == showLen)
        state = 0;
      break;

    default:
      state = 0;
  }

  return(colData);
}

 void scrollText(void)
{
  static uint32_t prevTime = 0;

  // Is it time to scroll the text?
  if (millis()-prevTime >= scrollDelay)
  {
    mx.transform(MD_MAX72XX::TSL);  // scroll along - the callback will load all the data
    prevTime = millis();      // starting point for next time
  }
}

uint16_t getScrollDelay(void)
{
#if USE_POT_CONTROL
  uint16_t  t;

  t = analogRead(SPEED_IN);
  t = map(t, 0, 1023, 25, 250);

  return(t);
#else
  return(SCROLL_DELAY);
#endif
}

void setup()
{
  mx.begin();
  mx.setShiftDataInCallback(scrollDataSource);
  mx.setShiftDataOutCallback(scrollDataSink);

#if USE_POT_CONTROL
  pinMode(SPEED_IN, INPUT);
#else
  scrollDelay = SCROLL_DELAY;
#endif

  //strncpy(curMessage, "New Yucky Wine with White Vinegar and Salt,   and Marmite Twigs,    and Bird Seeds!   To Mr Bean From Tommy.",BUF_SIZE-1);
  strncpy(curMessage, "Hello!",BUF_SIZE-1);
  curMessage[BUF_SIZE-1]='\0';
  msgDone = kState_Original_Message;

  Serial.begin(57600);
  Serial.print("\nScrolling text!\n");
}

void loop()
{
  static int mn = 0;
  
  if (msgDone > kState_NoMessage && msgDone < kState_Done )
  {
    scrollDelay = getScrollDelay();
    scrollText();

    if (msgDone == kState_Done)
    {
      Serial.print("\nDone!\n");

      switch (mn%7) 
      {
        case 0: strncpy(curMessage, "Grognenferk!",BUF_SIZE-1); break;
        case 1: strncpy(curMessage, "Yucky wine!",BUF_SIZE-1); break;
        case 2: strncpy(curMessage, "Neighbourhood",BUF_SIZE-1); break;
        case 3: strncpy(curMessage, "NO SMOKING",BUF_SIZE-1); break;
        case 4: strncpy(curMessage, "3 ...  2 ... 1 ... GO!!!",BUF_SIZE-1); break;
        case 5: curMessage[0]=3; curMessage[1]=' ';curMessage[2]=3; curMessage[3]=' ';curMessage[4]=3; curMessage[5]='\0'; break;
        case 6: strncpy(curMessage, "Yanny! Laurel!",BUF_SIZE-1); break;
      }
      msgDone = kState_Original_Message;
      ++mn;
      
    }
  }
}


