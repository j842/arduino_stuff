#define BLUE 3
#define GREEN 5 
#define RED 6
#define BUZZER 8
#define SERVO 9
#define BUTTON1 11
#define BUTTON2 12
#define BUTTON3 22
#define BUTTON4 24

// Motion detector and indication LED.
#define LED1 23
#define PIRPIN 7

#define JOYSWITCH 2
#define JOYX 0
#define JOYY 1

#define MEMBRANE 32

#include "jbuzzer.h"
#include "jbutton.h"
#include "jservo.h"
#include "jmembrane.h"
#include "jlcd.h"
#include "jpassword.h"

void setled(int red, int green, int blue)
{ // values are 0 to 255.
  analogWrite(RED, red);
  analogWrite(GREEN, green);
  analogWrite(BLUE, blue);
}


jbutton button1(BUTTON1);
jbutton button2(BUTTON2);
jbutton button3(BUTTON3);
jbutton button4(BUTTON4);
jbutton joyswitch1(JOYSWITCH);

jservo servo1(SERVO);

jbuzzer buzzer1(BUZZER);

jmembrane membrane1(MEMBRANE);

jlcd lcd1;

jpassword password("1341 1587 1234 172348");

void lock();


void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  
  button1.setup();
  button2.setup();
  button3.setup();
  button4.setup();
  servo1.setup();
  buzzer1.setup();
  joyswitch1.setup();
  membrane1.setup();
  lcd1.setup();

  pinMode(RED, OUTPUT);
  pinMode(GREEN,OUTPUT); 
  pinMode(BLUE,OUTPUT);
  pinMode(13,OUTPUT);   // onboard LED.
  pinMode(LED1, OUTPUT);

  pinMode(PIRPIN, INPUT);

  digitalWrite(LED1, LOW);

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
            switch (password.match()) 
            {
              case 1:
                lcd1.setmessage("Hello Tommy!","Choose a song!"); // unlocked.
                break;
              case 2:
                 lcd1.setmessage("Hello Jack!","Choose a song!"); // unlocked.
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
            setled(0,255,0);
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
  static bool butmode = false;

  // Motion Detector and LED
  int pirValue = digitalRead(PIRPIN);
  digitalWrite(LED1, pirValue);


  if (joyswitch1.pressed())
    butmode = !butmode;

  // joystick
  int joyx = analogRead(JOYX);
  int joyy = analogRead(JOYY);
  if (joyx<100 || joyy<100)
    { servo1.rotateby(-1 * (butmode ? 5 : 2)); delay(20); }
  if (joyx>924 || joyy>924)
    { servo1.rotateby(1 * (butmode ? 5 : 2)); delay(20); }

  if (button1.pressed())
      buzzer1.playsong(1);

  if (button2.pressed())
      buzzer1.playsong(4);

  if (button4.pressed())
      buzzer1.playsong(5);

  buzzer1.loop();

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
    setled(255,0,0);
    password.erase();
  }
  
    

