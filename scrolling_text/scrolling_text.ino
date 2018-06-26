#include "jscroll.h"

// see also setup stuff in jscroll.h
#define BUTTON1_PIN  3

void setup()
{
  pinMode(BUTTON1_PIN,INPUT);

  jscroll::setup();
  jscroll::set("Hello Tommy!");
}

void loop()
{
  static int mn = 0;
  static int PrevState = LOW;
  
  int buttonState = digitalRead(BUTTON1_PIN);

  static int buttoncount=0;
  if (buttonState==HIGH && PrevState==LOW)
  {
    switch (buttoncount)
    {
      case 0: jscroll::set("Don't!"); break;
      case 1: jscroll::set("No button pushing!"); break;
      case 2: jscroll::set("Don't push the button!"); break;
      case 3: jscroll::set("NO NO NO NO button pushing!"); break;
      case 4: jscroll::set(" ** STOP PUSHING ME! ** "); break;
      case 5: jscroll::set("It's naughty to push the button"); break;
      case 6: jscroll::set("OUCH!!!"); break;
      case 7: jscroll::set("Ow ow ow ow!"); break;
      case 8: jscroll::set("... D O N ' T   P U S H ..."); break;
      default:
        jscroll::set("Squiggle factory.");
    }
    buttoncount++;
    if (buttoncount>8) buttoncount=0;
  }
  PrevState = buttonState;
  
  jscroll::loop();

  if (jscroll::isDone())
  {
    switch (mn) 
    {
      case 0: jscroll::set("Grognenferk is not a word!"); break;
      case 1: jscroll::set("One minute until we blastoff!"); break;
      case 2: jscroll::set("5 ... 4 ... 3 ...  2 ... 1 ... Blastoff!!"); break;
      case 3: jscroll::set("NO SMOKING"); break;
      case 4: jscroll::set("3 ...  2 ... 1 ... GO!!!"); break;
      case 5: jscroll::set("Grognenferk IS a word!!"); break;
      case 6: jscroll::set("Yanny! Laurel!"); break;
      case 7: jscroll::set("Yucky wine!"); // fall through
      default: mn=-1; break;
    }
    ++mn;      
  }
}


