#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Encoder.h>
#include "fantasia.hpp"

////////////////////////////////////////////////////////////
// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=114.20000839233398,103.20000267028809
AudioSynthSimpleDrum     drum1;          //xy=183.20000076293945,261.59996604919434
AudioSynthNoiseWhite     noise1;         //xy=192.1999969482422,352.1999969482422
AudioSynthKarplusStrong  string1;        //xy=194.1999969482422,303.1999969482422
AudioAnalyzePeak         peak1;          //xy=276.1999969482422,100.19999694824219
AudioAnalyzeToneDetect   tone1;          //xy=302.1999969482422,195.1999969482422
AudioAnalyzeRMS          rms1;           //xy=305.1999969482422,156.1999969482422
AudioFilterStateVariable filter1;        //xy=346.20000076293945,397.2000036239624
AudioEffectEnvelope      envelope1;      //xy=445.20000076293945,340.2000026702881
AudioMixer4              mixer1;         //xy=546.200065612793,235.20000076293945
AudioOutputI2S           i2s2;           //xy=682.2000350952148,241.19999933242798
AudioConnection          patchCord1(i2s1, 0, peak1, 0);
AudioConnection          patchCord2(i2s1, 0, rms1, 0);
AudioConnection          patchCord3(i2s1, 0, tone1, 0);
AudioConnection          patchCord4(drum1, 0, mixer1, 0);
AudioConnection          patchCord5(noise1, 0, filter1, 0);
AudioConnection          patchCord6(string1, 0, mixer1, 1);
AudioConnection          patchCord7(filter1, 0, envelope1, 0);
AudioConnection          patchCord8(envelope1, 0, mixer1, 2);
AudioConnection          patchCord9(mixer1, 0, i2s2, 0);
AudioConnection          patchCord10(mixer1, 0, i2s2, 1);
// GUItool: end automatically generated code
////////////////////////////////////////////////////////////

Fantasia f;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); 
  f.setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  f.update();
  f.printState();

  static int count = 0;
  if (count % 125 == 0) {
    int beat = (count / 125) % 16;
    f.displayHex(beat, beat % 4 == 0);
    playSound(beat);
  }
  ++count;
  delayMicroseconds(1000);
}

void playSound(int beat) {
  if (beat % 4 == 0) {
    drum1.frequency(55);
    drum1.length(250);
    
    drum1.noteOn();
  }
  if (beat == 0) {
    string1.noteOff(0);
    string1.noteOn(110, 0.9);
  }
  if (beat > 4 && beat % 4 == 2) {
    string1.noteOff(0);
    string1.noteOn(220 + beat * 110, 0.5);
  }
}
