#include <MD_Parola.h>
//   https://github.com/MajicDesigns/MD_Parola
#include <MD_MAX72xx.h>
//   https://github.com/MajicDesigns/MD_MAX72xx
#include <SPI.h>

#define MAX_DEVICES 1  // Number of modules connected
#define CLK_PIN   13   // SPI SCK pin on UNO
#define DATA_PIN  11   // SPI MOSI pin on UNO
#define CS_PIN    10   // connected to pin 10 on UNO

//#define slider_pin A0  // OTA or OTB pin on slider
int slider_val;  // used to hold the slider analog value
int slide_scroll_speed;   // used when changing scroll speed

MD_Parola P = MD_Parola(MD_MAX72XX::FC16_HW /*CS_PIN*/, MAX_DEVICES);  // SPI config

int scrollSpeed;    // used to set text scroll speed in Parola at start

// sets scrolling direction if slider in middle at start
textEffect_t scrollEffect = PA_SCROLL_LEFT;

textPosition_t scrollAlign = PA_LEFT;  // how to aligh the text
 
int scrollPause = 0; // ms of pause after finished displaying message

#define  BUF_SIZE  75  // Maximum of 75 characters
char curMessage[BUF_SIZE] = { "Brainy-Bits" };  // used to hold current message


void setup()
{

  //pinMode(slider_pin, INPUT);
  
  P.begin();  // Start Parola
  
  // configure Parola
  P.displayText(curMessage, scrollAlign, scrollSpeed, scrollPause, scrollEffect, scrollEffect);

}

void loop() {

  //slider_val = analogRead(A0);  // check slider analog value
  slider_val = 100;
  
  if (slider_val > 600) {  // if slider to the left
    
    scrollEffect = PA_SCROLL_LEFT;  // Change direction to Left
    
    P.setTextEffect(scrollEffect, scrollEffect); // set new direction in Parola (OLD, NEW)
    
    slide_scroll_speed = map(slider_val, 1023, 0, 15, 400); // map slider value to text scroll speed
    
    P.setSpeed(slide_scroll_speed);  // Set new text scroll speed
    
  }
  
  
  if (slider_val < 350) {  // if slider to the right
    
    scrollEffect = PA_SCROLL_RIGHT;
    
    P.setTextEffect(scrollEffect, scrollEffect);  
    
    slide_scroll_speed = map(slider_val, 1023, 0, 400, 15);
    
    P.setSpeed(slide_scroll_speed);
  }
  

  if (slider_val < 600 && slider_val > 350) {  // if slider in middle
    
    slide_scroll_speed = 500;
    
    P.setSpeed(slide_scroll_speed);
  }


  if (P.displayAnimate()) // If finished displaying message
  {
    P.displayReset();  // Reset and display it again
  }
  
}
