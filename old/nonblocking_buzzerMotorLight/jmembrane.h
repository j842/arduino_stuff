#include <Keypad.h>

// Keypad library is standard, add to libraries in Arduino IDE.

// 4x4 membrane keyboard
#define ROWS 4
#define COLS 4

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
    jmembrane(int pinstart) : mPinStart(pinstart) 
    {
      for (int i=0;i<ROWS;++i)
      {
        colPins[i] = 4+i+mPinStart;
        rowPins[i] = i+mPinStart;
      }

      customKeypad = new Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
    }

    ~jmembrane()
    {
      delete customKeypad;
    }

    setup() {
      
    }

    bool getKey(char & c)
    {
      static char prev=0;
      c = customKeypad->getKey();
      if (c==prev)
        c=0;
      else
        prev=c;
      return c;
    }

  private:
    int mPinStart;
    byte rowPins[ROWS];
    byte colPins[COLS];
    Keypad * customKeypad;
};

