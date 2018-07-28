#define RED 7
#define BUZZER 8
#define BUTTON 9

#include "jbuzzer.h"
#include "jbutton.h"
#include "jled.h"

jbutton button1(BUTTON);
jbuzzer buzzer1(BUZZER);
jled led1(RED);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  
  button1.setup();
  buzzer1.setup();
  led1.setup(); // starts off.

}

void loop() 
{
  switch (button1.state())
  {
    case kPressed:
      led1.seton(true);
      buzzer1.playsong(6,true); // repeat
      break; 
    case kReleased:
      buzzer1.finish();
      led1.seton(false);
      break;
    default: ;
  }   
    
  buzzer1.loop();
}
  
    

