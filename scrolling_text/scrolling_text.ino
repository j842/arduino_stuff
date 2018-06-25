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

#define USE_POT_CONTROL 1
#define PRINT_CALLBACK  0

#define PRINT(s, v) { Serial.print(F(s)); Serial.print(v); }



// Define the number of devices we have in the chain and the hardware interface
#define HARDWARE_TYPE MD_MAX72XX::GENERIC_HW
#define MAX_DEVICES 2
#define CLK_PIN   10  // or SCK
#define DATA_PIN  12  // or MOSI
#define CS_PIN    11  // or SS
MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

#define BUTTON1_PIN  3
#define SPEED_IN    A5

#define CHAR_SPACING  1 // pixels between characters

// Global message buffers shared by Serial and Scrolling functions
#define BUF_SIZE  200
char curMessage[BUF_SIZE];

typedef enum {
  kState_NoMessage = 0,
  kState_Reset,
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
  static uint8_t scrollstate = 0;
  static char   *p = curMessage;
  static uint8_t  curLen, showLen;
  static uint8_t  cBuf[8];
  uint8_t colData;

  if (msgDone==kState_Reset)
    {
      scrollstate=0;
      msgDone=kState_Original_Message;
    }

  // finite state machine to control what we do on the callback
  switch(scrollstate)
  {
    case 0: // Load the next character from the font table
      showLen = mx.getChar(*p++, sizeof(cBuf)/sizeof(cBuf[0]), cBuf);
      curLen = 0;
      scrollstate++;

      // if we reached end of message, reset the message pointer
      if (*p == '\0')
      {
        if (msgDone == kState_Original_Message)
        { // add enough spaces to scroll text off screen.
          msgDone = kState_Clearing;
        }
        else 
        {
          p = curMessage;
          msgDone = kState_Done;
        }
      }
      // !! deliberately fall through to next state to start displaying

    case 1: // display the next part of the character
      if (msgDone==kState_Clearing)
      {
          showLen = 8*MAX_DEVICES;
          curLen=0;
          scrollstate=2;
          break;
      } else {
        colData = cBuf[curLen++];
        if (curLen == showLen)
        {
          showLen = CHAR_SPACING;
          curLen = 0;
          scrollstate = 2;
        }        
      }
      break;

    case 2: // display inter-character spacing (blank column)
      colData = 0;
      curLen++;
      if (curLen == showLen)
        scrollstate = 0;
      break;     
      
    default:
      scrollstate = 0;
  }

  return(colData);
}

 void scrollText(void)
{
  static uint32_t prevTime = 0;
  scrollDelay = getScrollDelay();

  // Is it time to scroll the text?
  if (millis()-prevTime >= scrollDelay)
  {
    mx.transform(MD_MAX72XX::TSL);  // scroll along - the callback will load all the data
    prevTime = millis();      // starting point for next time
  }
}

uint16_t getScrollDelay(void)
{
  uint16_t  t;

  t = analogRead(SPEED_IN);
  t = map(sqrt(t), 0, 32, 10, 100);  // 32^2 = 1024. Map to 10ms to 100ms.

  return(t);
}


void setMessage(char * msg) {
  strlcpy(curMessage,msg,BUF_SIZE);
  msgDone=kState_Reset;
}

void setup()
{
  mx.begin();
  mx.setShiftDataInCallback(scrollDataSource);
  mx.setShiftDataOutCallback(scrollDataSink);

  pinMode(SPEED_IN, INPUT);
  pinMode(BUTTON1_PIN,INPUT);

  setMessage("Hello Tommy!");

  Serial.begin(57600);
  Serial.print("\nScrolling text!\n");
}

void loop()
{
  static int mn = 0;
  
  if (msgDone > kState_NoMessage && msgDone < kState_Done )
  {
    int buttonState = digitalRead(BUTTON1_PIN);

    if (buttonState==HIGH)
      setMessage(" ** STOP PUSHING ME!");
    
    scrollText();

    if (msgDone == kState_Done)
    {
      Serial.print("\nDone!\n");

      switch (mn) 
      {
        case 0: setMessage("Grognenferk is not a word!"); break;
        case 1: setMessage("One minute until we blastoff!"); break;
        case 2: setMessage("5 ... 4 ... 3 ...  2 ... 1 ... Blastoff!!"); break;
        case 3: setMessage("NO SMOKING"); break;
        case 4: setMessage("3 ...  2 ... 1 ... GO!!!"); break;
        case 5: setMessage("Grognenferk IS a word!!"); break;
        case 6: setMessage("Yanny! Laurel!"); break;
        case 7: setMessage("Yucky wine!"); // fall through
        default: mn=-1; break;
      }
      ++mn;      
    }
  }
}


