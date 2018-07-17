#include <Wire.h>
#include <LiquidCrystal_I2C.h>

class jlcd
{
  public:
 jlcd() : lcd(0x27,16,2) {}  // set the LCD address to 0x27 for a 16 chars and 2 line display

 void setup() 
    { 
      lcd.init();
      lcd.noBacklight();
      lcd.noDisplay();
    }

  void setmessage(const char * line1, const char * line2)
  {
    if (line1==NULL && line2==NULL)
    {
      lcd.clear();
      lcd.noDisplay();
    }
    else
    {
      lcd.clear();
      if (line1)
      {
        lcd.setCursor(0,0);
        lcd.print(line1);
      }
      if (line2)
      {
        lcd.setCursor(0,1);
        lcd.print(line2);        
      }
      lcd.display();
    }
  }

  private:
    LiquidCrystal_I2C lcd;
};

