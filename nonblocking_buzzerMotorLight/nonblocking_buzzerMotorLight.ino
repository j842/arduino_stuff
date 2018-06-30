#define BLUE 3
#define GREEN 5 
#define RED 6
#define BUZZER 8
#define SERVO 9
#define BUTTON1 11
#define BUTTON2 12
#define BUTTON3 22

#define JOYSWITCH 2
#define JOYX 0
#define JOYY 1

#include "jbuzzer.h"
#include "jbutton.h"
#include "jservo.h"


void setled(int red, int green, int blue)
{ // values are 0 to 255.
  analogWrite(RED, red);
  analogWrite(GREEN, green);
  analogWrite(BLUE, blue);
}


jbutton button1(BUTTON1);
jbutton button2(BUTTON2);
jbutton button3(BUTTON3);
jbutton joyswitch1(JOYSWITCH);

jservo servo1(SERVO);

jbuzzer buzzer1(BUZZER);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  
  button1.setup();
  button2.setup();
  button3.setup();
  servo1.setup();
  buzzer1.setup();
  joyswitch1.setup();

  pinMode(RED, OUTPUT);
  pinMode(GREEN,OUTPUT); 
  pinMode(BLUE,OUTPUT);
  setled(0,0,0);

  pinMode(13,OUTPUT);

//  pinMode(JOYSWITCH, INPUT);
//  digitalWrite(JOYSWITCH, HIGH);
}

void loop() {
  static int phase = 0;

  static bool butmode = false;

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
      buzzer1.playsong(phase+1);

  if (button2.pressed())
      buzzer1.playsong(4);

  buzzer1.loop();

  if (button3.pressed())
  {  
      switch (phase)
      {
        case 0:
            setled(255,0,0);
            buzzer1.playnote(NOTE_C5,4);
            phase = 1;
            break;
  
        case 1:
            setled(0,255,0);
            buzzer1.playnote(NOTE_D5,4);
            phase = 2;
            break;        
  
        default:
            setled(0,0,255);
            buzzer1.playnote(NOTE_E5,4);
            phase = 0;
            break;       
      }
  } 
  
    
}
