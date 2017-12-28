/* The Singing Nutcrackers
 *  This is a quick holiday build using a Teensy 3.6,
 *  Teensy Sound Board, and 4 solenoid circuits to control
 *  the mouths of 4 nutcrackers. Each solenoid is controlled
 *  by the audio output from the Teensy Ausio Board - 2
 *  channels, left and right from each channel.
 *  Rob Reynolds, 2017, released under the Beerware License.
 */

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code from the
// Teensy Audio Library Audio System Design Tool

AudioPlaySdWav           playSdWav1;     //xy=141,314
AudioPlaySdWav           playSdWav2;     //xy=162,512
AudioMixer4              mixer1;         //xy=369,322
AudioMixer4              mixer2;         //xy=360,484
AudioAnalyzePeak         peak1;          //xy=375,164
AudioAnalyzePeak         peak2;          //xy=389,220
AudioAnalyzePeak         peak3;          //xy=360,583
AudioAnalyzePeak         peak4;          //xy=370,634
AudioOutputI2S           i2s1;           //xy=583,417
AudioConnection          patchCord1(playSdWav1, 0, mixer1, 0);
AudioConnection          patchCord2(playSdWav1, 0, peak1, 0);
AudioConnection          patchCord3(playSdWav1, 1, mixer2, 0);
AudioConnection          patchCord4(playSdWav1, 1, peak2, 0);
AudioConnection          patchCord5(playSdWav2, 0, mixer1, 1);
AudioConnection          patchCord6(playSdWav2, 0, peak3, 0);
AudioConnection          patchCord7(playSdWav2, 1, mixer2, 3);
AudioConnection          patchCord8(playSdWav2, 1, peak4, 0);
AudioConnection          patchCord9(mixer2, 0, i2s1, 1);
AudioConnection          patchCord10(mixer1, 0, i2s1, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=576,600

// GUItool: end automatically generated code

// Set pin assignments for 4 solenoids, start button
const int bassSolenoid = 15;
const int bariSolenoid = 16;
const int leadSolenoid = 17;
const int tenrSolenoid = 20;
const int buttonPin = 39;

void setup() {
  pinMode(bassSolenoid, OUTPUT);
  pinMode(bariSolenoid, OUTPUT);
  pinMode(leadSolenoid, OUTPUT);
  pinMode(tenrSolenoid, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  
  Serial.begin(9600);
  AudioMemory(10);  //Set memory for audio connections
  sgtl5000_1.enable(); //Enable the Audio Shield
  sgtl5000_1.volume(0.8);  //Set peak volume
  SPI.setMOSI(7);
  SPI.setSCK(14);
  if (!(SD.begin(10))) {
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
  delay(1000);

    while (digitalRead(buttonPin) == HIGH) {
    //do nothing until start button is pushed
  }
}

elapsedMillis msecs;

void loop() {


  if (playSdWav1.isPlaying() == false && playSdWav2.isPlaying() == false) {
    Serial.println("Start playing");
    //playSdWav1.play("SDTEST1.WAV");
    //playSdWav1.play("SDTEST2.WAV");
    //playSdWav1.play("SDTEST3.WAV");
    //playSdWav1.play("SDTEST4.WAV");
    //playSdWav1.play("SDTEST5.WAV");
    playSdWav1.play("DEMOLO3.WAV");
    playSdWav2.play("DEMOHI3.WAV");
    //playSdWav1.play("ICECRHI.WAV");
    //playSdWav2.play("ICECRLO.WAV");
    delay(10); // wait for library to parse WAV info
  }

  //  This section detects audio peaks for both channels, left and right
  if (msecs > 50) {
    if (peak1.available() && peak2.available()) {
      msecs = 0;
      float bassNumber = peak1.read();
      float bariNumber = peak2.read();
      float leadNumber = peak3.read();
      float tenrNumber = peak4.read();
      int bassPeak = bassNumber * 30.0;
      int bariPeak = bariNumber * 30.0;
      int leadPeak = bassNumber * 30.0;
      int tenrPeak = bariNumber * 30.0;
      int count;
      for (count=0; count < 30-bassPeak; count++) {
        Serial.print(" ");
      }
      
     //NOTE: Enable following section for debugging
/*      
      while (count++ < 30) {
        Serial.print("<");
      }
      Serial.print("||");
      for (count=0; count < bariPeak; count++) {
        Serial.print(">");
      }
      while (count++ < 30) {
        Serial.print(" ");
      }
      Serial.print(bassNumber);
      Serial.print(", ");
      Serial.print(bariNumber);
      Serial.println();

*/
      
      if (bassNumber > 0.05)
      {
        digitalWrite(bassSolenoid, HIGH);
        }
        else {
          digitalWrite(bassSolenoid, LOW);        
      }
     
      if (bariNumber > 0.03)
      {
        digitalWrite(bariSolenoid, HIGH);
        }
        else {
          digitalWrite(bariSolenoid, LOW);
       }

      if (leadNumber > 0.05)
      {
        digitalWrite(leadSolenoid, HIGH);
        }
        else {
          digitalWrite(leadSolenoid, LOW);        
      }

         if (tenrNumber > 0.03)
      {
        digitalWrite(tenrSolenoid, HIGH);
        }
        else {
          digitalWrite(tenrSolenoid, LOW);        
      }
        
    }
  }
}
