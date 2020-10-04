

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define CHAR_SPACING  1 // pixels between characters

/*
    PAROLA_HW,    ///< Use the Parola style hardware modules.
    GENERIC_HW,   ///< Use 'generic' style hardware modules commonly available.
    ICSTATION_HW, ///< Use ICStation style hardware module.
    FC16_HW       ///< Use FC-16 style hardware module.
*/

// just display static text on the MAX7219.

class jtext
{

public:
  jtext(int pin, int numDevices=4) : mx(MD_MAX72XX(HARDWARE_TYPE, pin, numDevices)), mNumDevices(numDevices)  {}
  void setup() { mx.begin(); }
  void printText(const char *pMsg);
  void setIntensity(uint8_t i); // i in [0..16]
  void erase() { printText(" ");} 

private: 
  MD_MAX72XX mx;
  int mNumDevices;
};



    void jtext::printText(const char *pMsg)
// Print the text string to the LED matrix modules specified.
// Message area is padded with blank columns after printing.
{
  uint8_t modStart = 0;
  uint8_t modEnd = mNumDevices-1;
  
  uint8_t   state = 0;
  uint8_t   curLen;
  uint16_t  showLen;
  uint8_t   cBuf[8];
  int16_t   col = ((modEnd + 1) * COL_SIZE) - 1;

  mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);

  do     // finite state machine to print the characters in the space available
  {
    switch(state)
    {
      case 0: // Load the next character from the font table
        // if we reached end of message, reset the message pointer
        if (*pMsg == '\0')
        {
          showLen = col - (modEnd * COL_SIZE);  // padding characters
          state = 2;
          break;
        }

        // retrieve the next character form the font file
        showLen = mx.getChar(*pMsg++, sizeof(cBuf)/sizeof(cBuf[0]), cBuf);
        curLen = 0;
        state++;
        // !! deliberately fall through to next state to start displaying

      case 1: // display the next part of the character
        mx.setColumn(col--, cBuf[curLen++]);

        // done with font character, now display the space between chars
        if (curLen == showLen)
        {
          showLen = CHAR_SPACING;
          state = 2;
        }
        break;

      case 2: // initialize state for displaying empty columns
        curLen = 0;
        state++;
        // fall through

      case 3:  // display inter-character spacing or end of message padding (blank columns)
        mx.setColumn(col--, 0);
        curLen++;
        if (curLen == showLen)
          state = 0;
        break;

      default:
        col = -1;   // this definitely ends the do loop
    }
  } while (col >= (modStart * COL_SIZE));

 // mx.transform(MD_MAX72XX::TRC); // rotate 90o.

  mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
}

void jtext::setIntensity(uint8_t i) // i in [0..16]
{ 
  static uint8_t prev = MAX_INTENSITY/2;
  if (i<0) i=0;
  if (i>MAX_INTENSITY) i=MAX_INTENSITY;
  
  if (i==prev)
    return;
     
  mx.control(MD_MAX72XX::INTENSITY, i);
 
  prev = i;
}

