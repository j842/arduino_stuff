#define RED 7
#define BUZZER 8
#define BUTTON 9

#include "jbuzzer.h"
#include "jbutton.h"


class jled {
  public:
  jled(int pin) : mPin(pin), mOn(false) {}
  void setup(bool on) { pinMode(mPin,OUTPUT); mOn=!on; set(on);}
  void set(bool on) { if (mOn!=on) digitalWrite(mPin, on?HIGH:LOW); mOn=on;}
  void loop() {}
  private:
    int mPin;
    bool mOn;
};

jbutton button1(BUTTON);
jbuzzer buzzer1(BUZZER);
jled led1(RED);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  
  button1.setup();
  buzzer1.setup();
  led1.setup(false); // start off.

}

void loop() 
{
  switch (button1.state())
  {
    case kPressed:
      led1.set(true);
      buzzer1.playsong(4,true); // repeat
      break; 
    case kReleased:
      buzzer1.finish();
      led1.set(false);
      break;
    default: ;
  }   
    
  buzzer1.loop();
}
  
    

