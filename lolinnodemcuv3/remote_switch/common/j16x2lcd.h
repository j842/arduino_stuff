#ifndef __16x2LCD_H
#define __16x2LCD_H

// see https://github.com/espressif/arduino-esp32/issues/4
// need to add the following to the LiquidCrystal_I2C.h library
// #define analogWrite ledcWrite
#include <LiquidCrystal_I2C.h> // Library for LCD

class displaystrings
{
    public:
        displaystrings() {}
        displaystrings(std::string s1, std::string s2) : mS1(s1),mS2(s2) {}
        bool equals(const displaystrings & other) const
        {
            return ((mS1.compare(other.getS1())==0) &&
                    (mS2.compare(other.getS2())==0));
        }
        std::string getS1() const {return mS1;}
        std::string getS2() const {return mS2;}
        std::string getS1_pad() const {std::string s1=mS1; padTo(s1); return s1;}
        std::string getS2_pad() const {std::string s2=mS2; padTo(s2); return s2;}
        void set(std::string s1, std::string s2) {mS1=s1; mS2=s2;}
        displaystrings& operator=(const displaystrings& other) // copy assignment
        {
            if (this != &other) { // self-assignment check expected
                mS1 = other.getS1();
                mS2 = other.getS2();
            }               
            return *this;
        }
    private:
        // right justify.
        static void padTo(std::string &str, const size_t num=16, const char paddingChar = ' ')
        {
            if(num > str.size())
                str.insert(0, num - str.size(), paddingChar);
        }


    private:
        std::string mS1,mS2;
};

inline bool operator==(const displaystrings& lhs, const displaystrings& rhs)
{
    return lhs.equals(rhs);
}
inline bool operator!=(const displaystrings& lhs, const displaystrings& rhs){ return !(lhs == rhs); }

class j16x2lcd
{
    public:
        j16x2lcd() : lcd(LiquidCrystal_I2C(PCF8574_ADDR_A21_A11_A01)), // 0x27)
            mDisplayTime(0)
        {
        }

        void setup()
        {
            static bool wirebegin=false;
            if (!wirebegin)
            {
                Wire.begin();
                wirebegin=true;
            }

            lcd.begin(16,2);
            lcd.backlight();

            updateTemp(displaystrings("Hello!",""));   
        }

        void loop()
        {
            if (!isNorm() && expired())
                setNorm();
        }

        void message(const displaystrings & s, bool tempOverride=false)
        {            
            if (tempOverride)
                updateTemp(s);
            else
                updateNorm(s);
        }

    private:
        void print(std::string s1,std::string s2)
        {
            lcd.setCursor(0,0);
            lcd.print(s1.c_str());
            lcd.setCursor(0,1);
            lcd.print(s2.c_str());
        }

        bool isNorm()
        {
            return mDisplayTime==0;
        }
        void setNorm()
        {
            mDisplayTime=0;
            print(mSNorm.getS1_pad(),mSNorm.getS2_pad());
        }
        void updateNorm(const displaystrings & s)
        {
            if (mSNorm==s)
                return; // nothing to update.
            mSNorm = s;
            if (isNorm())
                setNorm();
        }
        void updateTemp(const displaystrings & s)
        {
            mSTemp = s;
            mDisplayTime = millis()+kDelay;
            print(mSTemp.getS1_pad(),mSTemp.getS2_pad());
        }
        bool expired()
        {
            return millis()>mDisplayTime;
        }
        displaystrings & getCurrent()
        {
            return isNorm() ? mSNorm : mSTemp;
        }

    private:
        // connected via the Wire SCL and SDA pins (GPIO 22, 21)
        LiquidCrystal_I2C lcd;
        displaystrings mSNorm,mSTemp;
        unsigned long mDisplayTime;
        static const unsigned long kDelay = 2000;
};


#endif