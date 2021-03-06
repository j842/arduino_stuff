/*
  jBuzzer
  
  Based on a project and code by Dipto Pratyaksa, updated on 31/3/13
  Modified for Christmas by Joshi, on Dec 17th, 2017.
  Made into a non-blocking class by J.
*/

#include "pitches.h"


// Jingle Bells

int melody1[] = {
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5,
  NOTE_E5,
  NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5,
  NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_D5, NOTE_D5, NOTE_E5,
  NOTE_D5, NOTE_G5
};

int tempo1[] = {
  8, 8, 4,
  8, 8, 4,
  8, 8, 8, 8,
  2,
  8, 8, 8, 8,
  8, 8, 8, 16, 16,
  8, 8, 8, 8,
  4, 4
};

// We wish you a merry Christmas

int melody2[] = {
  NOTE_B3, 
  NOTE_F4, NOTE_F4, NOTE_G4, NOTE_F4, NOTE_E4,
  NOTE_D4, NOTE_D4, NOTE_D4,
  NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_F4,
  NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_A4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_G4,
  NOTE_F4, NOTE_D4, NOTE_B3, NOTE_B3,
  NOTE_D4, NOTE_G4, NOTE_E4,
  NOTE_F4
};

int tempo2[] = {
  4,
  4, 8, 8, 8, 8,
  4, 4, 4,
  4, 8, 8, 8, 8,
  4, 4, 4,
  4, 8, 8, 8, 8,
  4, 4, 8, 8,
  4, 4, 4,
  2
};

// Santa Claus is coming to town

int melody3[] = {
  NOTE_G4,
  NOTE_E4, NOTE_F4, NOTE_G4, NOTE_G4, NOTE_G4,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, NOTE_C5,
  NOTE_E4, NOTE_F4, NOTE_G4, NOTE_G4, NOTE_G4,
  NOTE_A4, NOTE_G4, NOTE_F4, NOTE_F4,
  NOTE_E4, NOTE_G4, NOTE_C4, NOTE_E4,
  NOTE_D4, NOTE_F4, NOTE_B3,
  NOTE_C4
};

int tempo3[] = {
  8,
  8, 8, 4, 4, 4,
  8, 8, 4, 4, 4,
  8, 8, 4, 4, 4,
  8, 8, 4, 2,
  4, 4, 4, 4,
  4, 2, 4,
  1
};


  int melody4[] = {
  NOTE_C4, NOTE_G3,NOTE_G3, NOTE_GS3, NOTE_G3,0, NOTE_B3, NOTE_C4};
  // note durations: 4 = quarter note, 8 = eighth note, etc.:
  int tempo4[] = {
    4, 8, 8, 4,4,4,4,4 
  };

int melody5[] = {                            // specific notes in the melody
 NOTE_B3, NOTE_C5, NOTE_B5, NOTE_D5, NOTE_A4, NOTE_B4, NOTE_B5, NOTE_G4, NOTE_B4, NOTE_D4, NOTE_C4, NOTE_B4, NOTE_C5, NOTE_B5, 0, NOTE_C5, NOTE_B5, NOTE_G4, NOTE_B5, NOTE_FS4, NOTE_G4, NOTE_A5, NOTE_B5, NOTE_G3, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_D4, NOTE_G3, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_G3, NOTE_B4, NOTE_A4, NOTE_C5, NOTE_B5, NOTE_G4, NOTE_B5, NOTE_FS4, NOTE_G4, NOTE_A5, NOTE_B5, NOTE_G3, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_D4, NOTE_G3, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_G3, NOTE_B4, NOTE_A4 };
 
int tempo5[] = {     // note durations: 4 = quarter note, 8 = eighth note, etc.:
  4, 1, 4, 4, 4, 1, 4, 4, 2, 2, 8, 1, 8, 3, 2, 4, 1, 4, 4, 3, 8, 9, 1, 4, 4, 5, 8, 8, 4, 4, 4, 8, 8, 3, 8, 1, 4, 1, 4, 4, 3, 8, 9, 1, 4, 4, 5, 8, 8, 4, 4, 4, 8, 8, 3, 8, 1 };
 




class jbuzzer
{
  public:
    jbuzzer(int buzzerPin) : mBuzzerPin(buzzerPin) { finish(); }

    void setup() {}

    void playnote(int freq, int dur)
    {
      note =0;
      waitto =0;
      playing=true;

      m0[0]=freq;
      t0[0]=dur;
      
      melody=m0;
      tempo=t0;
      len = 1;
    }

    void playsong(int s)
    {
      note = 0;
      waitto = 0;
      playing=true;

      Serial.print("Playing song ");
      Serial.println(s);
      
      switch (s)
      {
        case 1:
          melody = melody1;
          tempo = tempo1;
          len = sizeof(melody1) / sizeof(int);
          break;
        case 2:
          melody = melody2;
          tempo = tempo2;
          len = sizeof(melody2) / sizeof(int);
          break;
        case 3:
          melody = melody3;
          tempo = tempo3;
          len = sizeof(melody3) / sizeof(int);
          break;
        case 4:
          melody = melody4;
          tempo = tempo4;
          len = sizeof(melody4) / sizeof(int);
          break;
        case 5:
          melody = melody5;
          tempo = tempo5;
          len = sizeof(melody5) / sizeof(int);
          break;
        default:
          finish(); // play nothing.  
      }
    }

    void loop()
    {
      if (playing && millis()>waitto)
        {
          if (note==len)
            finish();
          else
          {
            int noteDuration = 2000/(*tempo);

//            Serial.print(noteDuration);
//            Serial.print("\t");
//            Serial.println(*melody);
            tone(mBuzzerPin, (*melody), noteDuration);
            //pause for the note's duration plus 30 ms:
            waitto = millis()+noteDuration+30;

            ++melody;
            ++tempo;
            ++note;
          }
        }
    }

    void finish()
    {
      playing=false; note=0; waitto=0; len=0;
    }


  private:
    int * melody;
    int * tempo;
    int len;
    bool playing;
    int note;
    int mBuzzerPin;
    unsigned long waitto;

    int m0[1];
    int t0[1];
};




