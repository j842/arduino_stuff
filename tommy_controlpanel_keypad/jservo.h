#include <Servo.h>


class jservo 
{
  public:
    jservo(int pin) : mPin(pin), mAngle(0) {}
    void setup() {   
        mServo.attach(mPin);  // attaches the servo on pin 9 to the servo object
        mServo.write(mAngle);
    }

    void rotateby(int delta)
    {
      mAngle += delta;
      if (mAngle>180) mAngle=180;
      if (mAngle<0) mAngle=0;
      mServo.write(mAngle);
    }

    void rotateto(int a)
    {
      mAngle = a;
      rotateby(0);
    }
    
    
    int mPin;
    int mAngle;
    Servo mServo;  // create servo object to control a servo
};
