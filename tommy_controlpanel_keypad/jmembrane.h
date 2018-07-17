#include <Keypad.h>

// Keypad library is standard, add to libraries in Arduino IDE.

// 4x4 membrane keyboard
#define ROWS 4
#define COLS 4

#define DEBOUNCE_MS (50)

//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

class jmembrane
{
  public:
    jmembrane(int pinstart) : mPinStart(pinstart) , mLastChange(0)
    {
      for (int i=0;i<ROWS;++i)
      {
        colPins[i] = 3-i+mPinStart;
        rowPins[i] = 7-i+mPinStart;
      }

      customKeypad = new Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
    }

    ~jmembrane()
    {
      delete customKeypad;
    }

    void setup() {
      
    }

    bool getKey(char & c)
    {
      static char prev=0;
      c = customKeypad->getKey();
      if (c==prev || (millis()-mLastChange<DEBOUNCE_MS))
        c=0;
      else
      {
        prev=c;
        mLastChange=DEBOUNCE_MS;
      }
      return c;
    }

  private:
    int mPinStart;
    byte rowPins[ROWS];
    byte colPins[COLS];
    Keypad * customKeypad;

    unsigned long mLastChange;
};

