#include "jbuzzer.h"
#include "jbutton.h"
#include "jrelay.h"

jbuzzer buzzer_main( 6 );
jrelay relay_aux( 7 );
jbutton button_blue( 8 );
jbutton button_red( 9 );

void setup() {
  // put your setup code here, to run once:
  relay_aux.setup();
  button_blue.setup();
  button_red.setup();
  buzzer_main.setup();

  relay_aux.off();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (button_blue.state() == kPressed )
  {
    relay_aux.on();
    buzzer_main.playsong(7,false);
  }
  
  if (button_red.state()== kPressed)
  {
    relay_aux.off();
    buzzer_main.playsong(6,false);
  }

  buzzer_main.loop();
}
