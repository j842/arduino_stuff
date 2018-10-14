#define BUZZER 11

#include "jbuzzer.h"

jbuzzer buzzer1(BUZZER);

void setup() {
// if analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  randomSeed(analogRead(0));
 buzzer1.setup();
//  digitalWrite(LED1, LOW);
}


void loop() {
  static unsigned long pause_waitto=0;
  
  if (!buzzer1.isPlaying())
  {
    if (millis()-pause_waitto>1000)
      buzzer1.playsong( random(1,7) );  
  }
  else pause_waitto = millis();

  buzzer1.loop();

}
    
