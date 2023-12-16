// "Audio drum" example for Befaco/Familiar's "Fantasia" hardware (custom Teensy-based device)
// by Glen Fraser (totalgee)
//
// This is a work in progress, started during the December 2023 hackathon in Barcelona.
//
// Currently it only plays beats/sounds according to the clock, and prints out
// some debug info on the serial port. The LED display updates continuously with
// the beat number.
//
// The A pot controls the "hit detection" threshold, for audio coming in
// on the left input.
// The B pot controls the "hit detection" tone, for audio coming in on the
// left input.

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Encoder.h>
#include "fantasia.hpp"

////////////////////////////////////////////////////////////
// From Teensy Audio System Design Tool
// https://www.pjrc.com/teensy/gui/index.html
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

struct {
  float peakToPeakAmp = 0;
  float toneLevel = 0;
  bool toneHit = false;
  float rmsLevel = 0;
} data;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); 
  f.setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  f.update();
  tone1.threshold(f.get(Fantasia::Pot::A));
  float const freq = midicps(map(f.get(Fantasia::Pot::B), 0, 1, 24, 120));
  tone1.frequency(freq);
  analyzeAudio();

  f.printState();
  printAudioInfo();

  static int count = 0;
  if (count % 125 == 0) {

    int beat = (count / 125) % 16;
    f.displayHex(beat, beat % 4 == 0);
    playSound(beat);
  }
  ++count;
  delayMicroseconds(1000);
}

void analyzeAudio() {
    if (peak1.available()) {
      data.peakToPeakAmp = peak1.readPeakToPeak();
    }
    if (tone1.available()) {
      data.toneLevel = tone1.read();
      data.toneHit = tone1;
    }
    if (rms1.available()) {
      data.rmsLevel = rms1.read();
    }
}

void printAudioInfo() {
  Serial.print("Audio Peak: ");
  Serial.print(data.peakToPeakAmp);
  Serial.print(" / RMS: ");
  Serial.print(data.rmsLevel);
  Serial.print(" / Tone: ");
  Serial.print(data.toneLevel);
  Serial.println(data.toneHit ? " HIT!" : "");
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

float midicps(float midinote) {
  return 440 * pow(2.0, (midinote - 69) / 12.0);
}

float cpsmidi(float cps) {
  return 12 * log(cps/220) / log(2.0) + 57;
}
