#define BLUE 3
#define GREEN 5 
#define RED 6
#define BUZZER 8
#define BUTTON1 11
#define BUTTON2 12

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
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);

  pinMode(RED, OUTPUT);
  pinMode(GREEN,OUTPUT); 
  pinMode(BLUE,OUTPUT);
  setled(0,0,0);

  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}


void playstuff() {
int melody[] = {
NOTE_C4, NOTE_G3,NOTE_G3, NOTE_GS3, NOTE_G3,0, NOTE_B3, NOTE_C4};
// note durations: 4 = quarter note, 8 = eighth note, etc.:

int noteDurations[] = {
   4, 8, 8, 4,4,4,4,4 
};

 for (int thisNote = 0; thisNote < 8; thisNote++) {
      // to calculate the note duration, take one second
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000/noteDurations[thisNote];
      tone(8, melody[thisNote],noteDuration);
      //pause for the note's duration plus 30 ms:
      delay(noteDuration +30);
   }

}


void loop() {
  static int phase = 0;
  static int button1state = HIGH;
  static int button2state = HIGH;

  if (digitalRead(BUTTON2)==LOW)
    button2state=LOW;
  else // high
    if (button2state==LOW)
    {
      button2state = HIGH;
      playstuff();
    }


  if (digitalRead(BUTTON1) == LOW)
    button1state = LOW;
  else // high
    if (button1state == LOW) // released
    {
      button1state = HIGH;
  
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
