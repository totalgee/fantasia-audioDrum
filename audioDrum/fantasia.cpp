#include "fantasia.hpp"

std::array<Fantasia::Pot, Fantasia::kNumPots> const Fantasia::pots{ Pot::A, Pot::B, Pot::C, Pot::D };
std::array<Fantasia::Button, Fantasia::kNumButtons> const Fantasia::buttons{ Button::A, Button::B, Button::C, Button::D };

namespace {
  constexpr std::array<uint8_t, Fantasia::kNumPots>     kPotPins{ A0, A1, A2, A3 };
  constexpr std::array<uint8_t, Fantasia::kNumButtons>  kButtonPins{ 2, 5, 25, 27 };
  constexpr std::array<uint8_t, Fantasia::kNumGates>    kGatePins{ 3, 6 };
  constexpr uint8_t                                     kEncoderButtonPin = 4;

} // unnamed namespace

void Fantasia::setupAudio()
{
  Serial.println("Setting up audio...");
  AudioMemory(24);
  sgtl5000.enable();
  sgtl5000.volume(1);
  sgtl5000.inputSelect(AUDIO_INPUT_LINEIN);
  sgtl5000.micGain(36); //NEEDED?
 
  update();

  Serial.println("Audio ready");
}

void Fantasia::update()
{
  for (int i = 0; i < kNumPots; ++i) {
    potValues[i] = analogRead(kPotPins[i]) / 1023.0f;
  }
  for (int i = 0; i < kNumButtons; ++i) {
    buttonValues[i] = !digitalRead(kButtonPins[i]);
  }
  encoderPressed = !digitalRead(kEncoderButtonPin);
  encoderPosition = encoder.read();
}
