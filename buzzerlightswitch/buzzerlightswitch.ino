#define BLUE 3
#define GREEN 5 
#define RED 6
#define BUZZER 8
#define SWTICH 11

#include "pitches.h"
#include <Servo.h>

Servo myservo;  // create servo object to control a servo
int pos = 0;    // variable to store the servo position


void setled(int red, int green, int blue)
{ // values are 0 to 255.
  analogWrite(RED, red);
  analogWrite(GREEN, green);
  analogWrite(BLUE, blue);
}


void setup() {
  // put your setup code here, to run once:
  pinMode(SWTICH, INPUT_PULLUP);

  pinMode(RED, OUTPUT);
  pinMode(GREEN,OUTPUT); 
  pinMode(BLUE,OUTPUT);
  setled(0,0,0);

  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}


void loop() {
  static int phase = 0;
  static int buttonstate = 0;

  if (digitalRead(SWTICH) == LOW)
  {
    buttonstate = 1;
  }
  else // high
    if (buttonstate == 1) // released
    {
      buttonstate = 0;
  
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
