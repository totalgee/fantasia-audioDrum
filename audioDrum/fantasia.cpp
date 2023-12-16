#include "fantasia.hpp"

std::array<Fantasia::Pot, Fantasia::kNumPots> const       Fantasia::pots{ Pot::A, Pot::B, Pot::C, Pot::D };
std::array<Fantasia::Button, Fantasia::kNumButtons> const Fantasia::buttons{ Button::A, Button::B, Button::C, Button::D };
std::array<Fantasia::Gate, Fantasia::kNumGates> const     Fantasia::gates{ Gate::GT1, Gate::GT2 };

namespace {
  constexpr std::array<uint8_t, Fantasia::kNumPots>     kPotPins{ A0, A1, A2, A3 };
  constexpr std::array<uint8_t, Fantasia::kNumButtons>  kButtonPins{ 2, 5, 25, 27 };
  constexpr std::array<uint8_t, Fantasia::kNumGates>    kGatePins{ 3, 6 };
  constexpr uint8_t                                     kEncoderButtonPin = 4;

} // unnamed namespace

void Fantasia::setup()
{
  setupPins();
  setupAudio();
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
  encoderPosition = encoder.read() / 4;
}

void Fantasia::printState()
{
  Serial.print("Pots: ");
  for (auto pot : pots) {
    Serial.print(get(pot));
    Serial.print(' ');
  }

  Serial.print("\nButtons: ");
  for (auto button : buttons) {
    Serial.print(get(button));
    Serial.print(' ');
  }

  Serial.print("\nEncoder position: ");
  Serial.print(getEncoder());
  Serial.print(" pressed: ");
  Serial.println(getEncoderPressed());
}

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

void Fantasia::setupPins()
{
  // Setup digital inputs
  for (int i = 0; i < kNumButtons; ++i) {
    pinMode(kButtonPins[i], INPUT_PULLUP);
  }
  for (int i = 0; i < kNumGates; ++i) {
    pinMode(kGatePins[i], INPUT_PULLUP);
  }
  pinMode(kEncoderButtonPin, INPUT_PULLUP);

  // Set 8-segment display LEDs pins as outputs
  for (auto pin : kSegmentPins) {
    pinMode(pin, OUTPUT);
  }
}
