#define BLUE 3
#define GREEN 5 
#define RED 6
#define BUZZER 8
#define SERVO 9
#define BUTTON1 11
#define BUTTON2 12
#define BUTTON3 22

#include "pitches.h"
#include "xmas.h"
#include <Servo.h>

Servo myservo;  // create servo object to control a servo
int pos = 0;    // variable to store the servo position

class button 
{
  public:
    button(int pin) : mPin(pin), mState(HIGH) {}
    setup() {  pinMode(mPin, INPUT_PULLUP); }
    bool pressed() { 
      if (digitalRead(mPin)==LOW)
        mState=LOW;
      else // high
        if (mState==LOW && (millis()-mLastPush>200))
        {
          mState = HIGH;
          mLastPush = millis(); // debounce
          return true;
        }
      return false;
    }

    int mState;
    int mPin;
    unsigned long mLastPush=0;
      };



void setled(int red, int green, int blue)
{ // values are 0 to 255.
  analogWrite(RED, red);
  analogWrite(GREEN, green);
  analogWrite(BLUE, blue);
}


button button1(BUTTON1);
button button2(BUTTON2);
button button3(BUTTON3);


void setup() {
  // put your setup code here, to run once:
  button1.setup();
  button2.setup();
  button3.setup();

  pinMode(RED, OUTPUT);
  pinMode(GREEN,OUTPUT); 
  pinMode(BLUE,OUTPUT);
  setled(0,0,0);

  pinMode(13,OUTPUT);

  myservo.attach(SERVO);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  static int phase = 0;

  if (button1.pressed())
      xmas(phase);

  if (button2.pressed())
      playstuff();

  if (button3.pressed())
  {  
      switch (phase)
      {
        case 0:
            setled(255,0,0);
            tone(8, NOTE_C5, 200);
            phase = 1;
            myservo.write(0);
            delay(200);
            break;
  
        case 1:
            setled(0,255,0);
            tone(8, NOTE_D5, 200);
            phase = 2;
            myservo.write(90);
            delay(200);
            break;        
  
        default:
            setled(0,0,255);
            tone(8, NOTE_E5, 200);
            phase = 0;
            myservo.write(180);
            delay(200);
            break;       
      }
  } 
  
    
}
