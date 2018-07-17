#define PUSHBUTTON 2
#define RELAYSTEREO 3
#define RELAYPOWER 4
#define BUZZER 5
#define MEMBRANE 6

#include "jbuzzer.h"
#include "jbutton.h"
#include "jmembrane.h"
#include "jpassword.h"
#include "jlcd.h"

jbutton pushbutton1(PUSHBUTTON);
jbuzzer buzzer1(BUZZER);
jmembrane membrane1(MEMBRANE);
jpassword password("1341 1587 1234 172348 3691238227");
jlcd lcd1;


class jrelay
{
  public:
    jrelay(int pin) : mPin(pin), mIsOn(false) {}
    void setup() { pinMode(mPin, OUTPUT);}
    void off() { digitalWrite(mPin, HIGH); mIsOn=false; }
    void on() { digitalWrite(mPin, LOW); mIsOn=true; }
    bool ison() { return mIsOn;}
  private:
    int mPin;
    bool mIsOn;
};

jrelay relaystereo(RELAYSTEREO);
jrelay relaypower(RELAYPOWER);

void lock();


void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  
  pushbutton1.setup();
  buzzer1.setup();
  membrane1.setup();
  relaypower.setup();
  relaystereo.setup();
  lcd1.setup();

  lock();
}

void loop_locked();
void loop_unlocked();

void loop() {
  if (password.match()>0)
    loop_unlocked();
  else
    loop_locked();
}

void loop_locked() 
{
  char c;
  if (membrane1.getKey(c))
    {
      buzzer1.playnote(600+100*(c-'0'),8);
      password.addchar(c);

      if (password.match()>0)
         {
          relaypower.on();
          relaystereo.on();
          
            switch (password.match()) 
            {
              case 1:
              case 5:
                lcd1.setmessage("Hello Tommy!","* to lock"); // unlocked.
                break;
              case 2:
                 lcd1.setmessage("Hello Jack!","* to lock"); // unlocked.
                 break;
              case 3:
                lcd1.setmessage("Hello Daddy!","It's not yours!");
                break;
              case 4:
                lcd1.setmessage("Hi Mummy!","Tommy's turn!");
                break;
              default:
                lcd1.setmessage(">>!>!>!","Who are you?");
            }
                
            buzzer1.playnote(NOTE_D5,4);
         }
      else if (password.done())
      {
            lcd1.setmessage("Wrong password!","Try again.");
            password.erase();
            buzzer1.playsong(4);        
      }
      else
        lcd1.setmessage(password.userpassword(),NULL);
    }

  buzzer1.loop();
}


void loop_unlocked() 
{
  buzzer1.loop();

  if ( pushbutton1.state() == kPressed )
  {
    if (relaystereo.ison())
      relaystereo.off();
    else 
      relaystereo.on();
  }
  
  char c;
  if (membrane1.getKey(c))
  {
    if (c>='0' && c<='9')
      buzzer1.playnote(600+100*(c-'0'),8);

    if (c>='A' && c<='D')
      buzzer1.playsong(c-'A'+1);

    if (c=='#') {buzzer1.playnote(NOTE_D5,4);}
    if (c=='*') {lock(); return;}
  }

}

  void lock()
  {
    buzzer1.playnote(NOTE_D5,4);
    lcd1.setmessage("Enter password,","then press #");
    relaypower.off();
    relaystereo.off();
    password.erase();
  }
  
    

