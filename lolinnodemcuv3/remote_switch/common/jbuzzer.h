/*
  jBuzzer
  
  Based on a project and code by Dipto Pratyaksa, updated on 31/3/13
  Modified for Christmas by Joshi, on Dec 17th, 2017.
  Made into a non-blocking class by J.
*/

#ifndef __JBUZZER_H
#define __JBUZZER_H

#include <string>
#include "pitches.h"

class cMelody 
{
  public:
    cMelody(std::string name, std::vector<int> notes, std::vector<int> tempos) :
      mName(name), mNotes(notes), mTempo(tempos)
    {
    }

    std::string mName;
    std::vector<int> mNotes;
    std::vector<int> mTempo;
};



class jbuzzer
{
  public:
    jbuzzer(int buzzerPin) : mBuzzerPin(buzzerPin) 
      { 
        loadMelodies();       
        finish(); 
      }

    void playsong(unsigned int s)
    {
      note = 0;
      waitto = 0;

      if (s<1 || s>Melodies.size())
      {
        Serial.println("Invalid melody number.");
        return;
      }

      playing=true;
      Serial.print("Playing song ");
      Serial.println(s);

      melody = s-1;   
    }

    #ifdef __USETONE
      void starttone(unsigned int frequency, unsigned long duration)
      {
        tone(mBuzzerPin,frequency,duration);
      }
      void stoptone()
      {
      }
      void customsetup()
      {
      }
    #else
      #define ledChannel 0
      void customsetup()
      {
        int freq = 2000;
        int resolution = 8;
        ledcSetup(ledChannel, freq, resolution);
        ledcAttachPin(mBuzzerPin, ledChannel);
      }
      void starttone(unsigned int frequency, unsigned long duration)
      {  // https://community.platformio.org/t/tone-not-working-on-espressif32-platform/7587/2
          ledcWriteTone(ledChannel, frequency);
      }
      void stoptone()
      {
          ledcWriteTone(ledChannel, 0);
      }
    #endif

    void setup() 
    {
      customsetup();
    }


    void loop()
    {
      if (playing && millis()>waitto)
        {
          stoptone();
          if (note==Melodies[melody].mNotes.size())
            finish();
          else
          {
            unsigned long noteDuration = 2000/(Melodies[melody].mTempo[note]);
            unsigned int noteFrequency = Melodies[melody].mNotes[note];
            starttone(noteFrequency, noteDuration);
            //pause for the note's duration plus 30 ms:
            waitto = millis()+noteDuration+30;
            ++note;
          }
        }
    }

    void finish()
    {
      playing=false; note=0; waitto=0;
    }



  void loadMelodies()
  {
    Melodies.push_back(cMelody(
      "Jingle Bells",
      {   NOTE_E5, NOTE_E5, NOTE_E5,
          NOTE_E5, NOTE_E5, NOTE_E5,
          NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5,
          NOTE_E5, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5,
          NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5,
          NOTE_E5, NOTE_D5, NOTE_D5, NOTE_E5,
          NOTE_D5, NOTE_G5
      },
      {
        8, 8, 4,
        8, 8, 4,
        8, 8, 8, 8,
        2,
        8, 8, 8, 8,
        8, 8, 8, 16, 16,
        8, 8, 8, 8,
        4, 4
      }
    ));

    Melodies.push_back(cMelody(
      "We wish you a merry Christmas",
      {
        NOTE_B3, 
        NOTE_F4, NOTE_F4, NOTE_G4, NOTE_F4, NOTE_E4,
        NOTE_D4, NOTE_D4, NOTE_D4,
        NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_F4,
        NOTE_E4, NOTE_E4, NOTE_E4,
        NOTE_A4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_G4,
        NOTE_F4, NOTE_D4, NOTE_B3, NOTE_B3,
        NOTE_D4, NOTE_G4, NOTE_E4,
        NOTE_F4      
      },
      {
        4,
        4, 8, 8, 8, 8,
        4, 4, 4,
        4, 8, 8, 8, 8,
        4, 4, 4,
        4, 8, 8, 8, 8,
        4, 4, 8, 8,
        4, 4, 4,
        2
      }
    ));

    Melodies.push_back(cMelody(
      "Santa Claus is coming to town",
      {
        NOTE_G4,
        NOTE_E4, NOTE_F4, NOTE_G4, NOTE_G4, NOTE_G4,
        NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, NOTE_C5,
        NOTE_E4, NOTE_F4, NOTE_G4, NOTE_G4, NOTE_G4,
        NOTE_A4, NOTE_G4, NOTE_F4, NOTE_F4,
        NOTE_E4, NOTE_G4, NOTE_C4, NOTE_E4,
        NOTE_D4, NOTE_F4, NOTE_B3,
        NOTE_C4      
      },
      {
        8,
        8, 8, 4, 4, 4,
        8, 8, 4, 4, 4,
        8, 8, 4, 4, 4,
        8, 8, 4, 2,
        4, 4, 4, 4,
        4, 2, 4,
        1      
      }
    ));

    Melodies.push_back(cMelody(
      "Death March",
      {NOTE_C4, NOTE_G3,NOTE_G3, NOTE_GS3, NOTE_G3,0, NOTE_B3, NOTE_C4},
      {4, 8, 8, 4,4,4,4,4}
    ));

    Melodies.push_back(cMelody(
      "I forget what this is.",
      {NOTE_B3, NOTE_C5, NOTE_B5, NOTE_D5, NOTE_A4, NOTE_B4, NOTE_B5, NOTE_G4, NOTE_B4, NOTE_D4, NOTE_C4, NOTE_B4, NOTE_C5, NOTE_B5, 0, NOTE_C5, NOTE_B5, NOTE_G4, NOTE_B5, NOTE_FS4, NOTE_G4, NOTE_A5, NOTE_B5, NOTE_G3, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_D4, NOTE_G3, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_G3, NOTE_B4, NOTE_A4, NOTE_C5, NOTE_B5, NOTE_G4, NOTE_B5, NOTE_FS4, NOTE_G4, NOTE_A5, NOTE_B5, NOTE_G3, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_D4, NOTE_G3, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_G3, NOTE_B4, NOTE_A4 },
      {4, 1, 4, 4, 4, 1, 4, 4, 2, 2, 8, 1, 8, 3, 2, 4, 1, 4, 4, 3, 8, 9, 1, 4, 4, 5, 8, 8, 4, 4, 4, 8, 8, 3, 8, 1, 4, 1, 4, 4, 3, 8, 9, 1, 4, 4, 5, 8, 8, 4, 4, 4, 8, 8, 3, 8, 1 }
    ));

    Melodies.push_back(cMelody(
      "Boo Bee",
      {NOTE_B3, NOTE_C5},
      {8,8}
    ));

    Melodies.push_back(cMelody(
      "Bee Boo",
      {NOTE_C5, NOTE_B3},
      {8,8}
    ));
  }

  private:
    unsigned int melody;
    bool playing;
    unsigned int note;
    int mBuzzerPin;
    unsigned long waitto;

    std::vector<cMelody> Melodies;
};

#endif