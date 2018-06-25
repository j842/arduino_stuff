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
bool msgReset=true;
bool msgDone=false;

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

  if (strlen(curMessage)<1)
    strcpy(curMessage,"BROKEN!");

  if (msgReset)
    {
      showLen = 8*MAX_DEVICES;
      curLen=0;
      scrollstate=2;
      p = curMessage;
      msgReset=false;
      msgDone=false;
    }

  // finite state machine to control what we do on the callback
  switch(scrollstate)
  {
    case 0: // Load the next character from the font table
      showLen = mx.getChar(*p++, sizeof(cBuf)/sizeof(cBuf[0]), cBuf);
      curLen = 0;
      scrollstate++;

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
      {
        if (*p=='\0')
        {
          msgDone=true;
          scrollstate=3;
        } 
        else
          scrollstate = 0;
      }
      break;  

    case 3: // msgDone - needs reset to get out of this.
      colData = 0;
      break;
          
    default:
      // should never get here.
      colData = 0;
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
  t = map(sqrt(t), 0, 32, 25, 100);  // 32^2 = 1024. Map to 10ms to 100ms.

  return(t);
}


void setMessage(const char * msg) {
  strlcpy(curMessage,msg,BUF_SIZE);
  msgReset=true;
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
  static int PrevState = LOW;
  
  int buttonState = digitalRead(BUTTON1_PIN);

  static int buttoncount=0;
  if (buttonState==HIGH && PrevState==LOW)
  {
    switch (buttoncount)
    {
      case 0: setMessage("Don't!"); break;
      case 1: setMessage("No button pushing!"); break;
      case 2: setMessage("Don't push the button!"); break;
      case 3: setMessage("NO NO NO NO button pushing!"); break;
      case 4: setMessage(" ** STOP PUSHING ME! ** "); break;
      case 5: setMessage("It's naughty to push the button"); break;
      case 6: setMessage("OUCH!!!"); break;
      case 7: setMessage("Ow ow ow ow!"); break;
      case 8: setMessage("... D O N ' T   P U S H ..."); break;
      default:
        setMessage("Squiggle factory.");
    }
    buttoncount++;
    if (buttoncount>8) buttoncount=0;
  }
  PrevState = buttonState;
  
  scrollText();

  if (msgDone)
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


