#include <Arduino.h>

//const int LED = D1; // GPIO 16

const int RED=D5;
const int BLUE=D6;
const int GREEN=D7;


void setup() {
  // put your setup code here, to run once:

// Init LED pin as an output
 pinMode(RED, OUTPUT);
 pinMode(GREEN, OUTPUT);
 pinMode(BLUE, OUTPUT);

// Welcome message
 Serial.begin(9600);
 Serial.print("Hello world");
}

void rgb(int r, int g, int b)
{
  analogWrite(RED,r);
  analogWrite(GREEN,g);
  analogWrite(BLUE,b);
}

void blink(int howlong,int r, int g, int b)
{
  rgb(255-r,255-g,255-b);
  delay(howlong);
  rgb(255,255,255);
}

void loop() {
  // put your main code here, to run repeatedly:

  // blink(400,255,0,0);
  // blink(400,0,255,0);
  // blink(1000,0,0,255);
  // delay(1000);  
  static int c= millis();
  static int p=0;
  const int maxC=50;

  unsigned long now = millis();

  if (now-c>maxC)
  {
    c=now;
    ++p;
    if (p>749) p=0;

    if (p<250)
      rgb(p,255-p,255);
    else if (p<500)
      rgb(255,p-250,255-(p-250));
    else 
      rgb(255-(p-500),255,p-500);
  }

}