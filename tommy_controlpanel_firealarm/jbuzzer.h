/*
  jBuzzer
  
  Based on a project and code by Dipto Pratyaksa, updated on 31/3/13
  Modified for Christmas by Joshi, on Dec 17th, 2017.
  Made into a non-blocking class by J.
*/

#include "pitches.h"






class jbuzzer
{
  public:
    jbuzzer(int buzzerPin);
    void setup() {pinMode(mBuzzerPin, OUTPUT);  }
    void loop();

    bool isPlaying();
    void playsong(int s, bool repeat = false);
    void finish();

  private:
    void playmNoteIndex(int freq, int dur);
  
    int * mNotePtr;
    int * mTempoPtr;
    int mNotesLen;
    bool mPlaying;
    int mNoteIndex;
    int mBuzzerPin;
    unsigned long waitto;
    int mRepeatSong;

    int m0[1];
    int t0[1];
};







// Jingle Bells

int Notes1[] = {
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5,
  NOTE_E5,
  NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5,
  NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_D5, NOTE_D5, NOTE_E5,
  NOTE_D5, NOTE_G5
};

int Tempos1[] = {
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

int Notes2[] = {
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

int Tempos2[] = {
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

int Notes3[] = {
  NOTE_G4,
  NOTE_E4, NOTE_F4, NOTE_G4, NOTE_G4, NOTE_G4,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, NOTE_C5,
  NOTE_E4, NOTE_F4, NOTE_G4, NOTE_G4, NOTE_G4,
  NOTE_A4, NOTE_G4, NOTE_F4, NOTE_F4,
  NOTE_E4, NOTE_G4, NOTE_C4, NOTE_E4,
  NOTE_D4, NOTE_F4, NOTE_B3,
  NOTE_C4
};

int Tempos3[] = {
  8,
  8, 8, 4, 4, 4,
  8, 8, 4, 4, 4,
  8, 8, 4, 4, 4,
  8, 8, 4, 2,
  4, 4, 4, 4,
  4, 2, 4,
  1
};


  int Notes4[] = {
  NOTE_C4, NOTE_G3,NOTE_G3, NOTE_GS3, NOTE_G3,0, NOTE_B3, NOTE_C4};                        
  // mNoteIndex durations: 4 = quarter mNoteIndex, 8 = eighth mNoteIndex, etc.:
  int Tempos4[] = {
    4, 8, 8, 4,4,4,4,4 
  };

int Notes5[] = {                            // specific mNoteIndexs in the mNotePtr
 NOTE_B3, NOTE_C5, NOTE_B5, NOTE_D5, NOTE_A4, NOTE_B4, NOTE_B5, NOTE_G4, NOTE_B4, NOTE_D4, NOTE_C4, NOTE_B4, NOTE_C5, NOTE_B5, 0, NOTE_C5, NOTE_B5, NOTE_G4, NOTE_B5, NOTE_FS4, NOTE_G4, NOTE_A5, NOTE_B5, NOTE_G3, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_D4, NOTE_G3, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_G3, NOTE_B4, NOTE_A4, NOTE_C5, NOTE_B5, NOTE_G4, NOTE_B5, NOTE_FS4, NOTE_G4, NOTE_A5, NOTE_B5, NOTE_G3, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_D4, NOTE_G3, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_G3, NOTE_B4, NOTE_A4 };
 
int Tempos5[] = {     // mNoteIndex durations: 4 = quarter mNoteIndex, 8 = eighth mNoteIndex, etc.:
  4, 1, 4, 4, 4, 1, 4, 4, 2, 2, 8, 1, 8, 3, 2, 4, 1, 4, 4, 3, 8, 9, 1, 4, 4, 5, 8, 8, 4, 4, 4, 8, 8, 3, 8, 1, 4, 1, 4, 4, 3, 8, 9, 1, 4, 4, 5, 8, 8, 4, 4, 4, 8, 8, 3, 8, 1 };
 

int Notes6[] = { NOTE_G5, NOTE_E4 };
int Tempos6[] = { 6, 4  };



jbuzzer::jbuzzer(int buzzerPin) : mBuzzerPin(buzzerPin), mRepeatSong(0) { finish(); }

bool jbuzzer::isPlaying() {return mPlaying;}

#define SONGMAP( index ) case index: mNotePtr = Notes##index ; mTempoPtr = Tempos##index ; mNotesLen = sizeof( Notes##index ) / sizeof(int); break;

void jbuzzer::playsong(int s, bool repeat)
    {
      finish();

      mPlaying=true;

      if (repeat)
        mRepeatSong = s;
      else
        mRepeatSong = 0;

      Serial.print("Playing song ");
      Serial.println(s);

      switch (s)
      {
        SONGMAP(1)
        SONGMAP(2)
        SONGMAP(3)
        SONGMAP(4)
        SONGMAP(5)
        SONGMAP(6)
        default:
          finish(); // play nothing.  
      }
    }

void jbuzzer::loop()
    {
      if (mPlaying && millis()>waitto)
        {
          if (mNoteIndex==mNotesLen)
          {
            if (mRepeatSong==0)
              finish();
            else 
              playsong(mRepeatSong,true);
          }
          else
          {
            int mNoteIndexDuration = 2000/(*mTempoPtr);

//            Serial.print(mNoteIndexDuration);
//            Serial.print("\t");
//            Serial.println(*mNotePtr);
            tone(mBuzzerPin, (*mNotePtr), mNoteIndexDuration);
            //pause for the mNoteIndex's duration plus 30 ms:
            waitto = millis()+mNoteIndexDuration+30;

            ++mNotePtr;
            ++mTempoPtr;
            ++mNoteIndex;
          }
        }
    }

void jbuzzer::finish()
{
  mPlaying=false; mNoteIndex=0; waitto=0; mNotesLen=0;
}


void jbuzzer::playmNoteIndex(int freq, int dur)
    {
      mNoteIndex =0;
      waitto =0;
      mPlaying=true;

      m0[0]=freq;
      t0[0]=dur;
      
      mNotePtr=m0;
      mTempoPtr=t0;
      mNotesLen = 1;
    }


  

