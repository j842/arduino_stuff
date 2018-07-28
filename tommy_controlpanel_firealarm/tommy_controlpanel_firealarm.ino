#include <MD_MAX72xx.h>
#include <SPI.h>


#define REDLED 7
#define BUZZER 8
#define ALARM_SWITCH 9
#define BUTTON_BLUE 6
#define BUTTON_RED 5
#define POT A0
#define MAX7219_CS_PIN 10


#include "jbuzzer.h"
#include "jbutton.h"
#include "jled.h"
#include "jpot.h"
#include "jtext.h"
#include "jtimer.h"

jbutton button_alarm(ALARM_SWITCH);
jbuzzer buzzer1(BUZZER);
jled led1(REDLED);

jbutton button_blue(BUTTON_BLUE);
jbutton button_red(BUTTON_RED);

jtext text(MAX7219_CS_PIN,4); // 4 devices.

jpot pot1(POT);

jtimer timer(&buzzer1,&text,&pot1,&button_red,&button_blue);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);

  text.setup();
  
  button_alarm.setup();
  buzzer1.setup();
  led1.setup(); // starts off.

  button_blue.setup();
  button_red.setup();
  pot1.setup();
}


void loop() 
{  
  switch (button_alarm.state())
  {
    case kPressed:
      led1.seton(true);
      buzzer1.playsong(6,true); // repeat
      break; 
    case kReleased:
      buzzer1.finish();
      led1.seton(false);
      break;
    default: 
      ;
  }   

  // timer!
  timer.loop();
    
  buzzer1.loop();
}
  
    

