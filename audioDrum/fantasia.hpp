#pragma once

#include <array>

#include <Audio.h>
#include <Encoder.h>
#include <SegmentDisplay.h>         // use: https://github.com/dgduncan/SevenSegment

// Fantasia buttons and pots
class Fantasia {
public:
  void setup();
  void update();
  void printState();

  static constexpr int kNumPots = 4;
  enum class Pot { A, B, C, D };
  static std::array<Pot, kNumPots> const pots;
  float get(Pot pot) const { return potValues[static_cast<int>(pot)]; }

  static constexpr int kNumButtons = 4;
  enum class Button { A, B, C, D };
  static std::array<Button, kNumButtons> const buttons;
  bool get(Button button) const { return buttonValues[static_cast<int>(button)]; }

  static constexpr int kNumGates = 2;
  enum class Gate { GT1, GT2 };
  static std::array<Gate, kNumGates> const gates;
  bool get(Gate gate) const { return gateValues[static_cast<int>(gate)]; }

  int32_t getEncoder() const { return encoderPosition; }
  bool getEncoderPressed() const { return encoderPressed; }

  void displayHex(int num, bool decimalPoint) { segmentDisplay.displayHex(num, decimalPoint); }

private:
  void setupAudio();
  void setupPins();

  std::array<float, kNumPots>   potValues{};
  std::array<bool, kNumButtons> buttonValues{};
  std::array<bool, kNumGates>   gateValues{};

  Encoder                       encoder{ PIN_SERIAL_RX, PIN_SERIAL_TX };
  bool                          encoderPressed = false;
  int32_t                       encoderPosition;

  // Display LEDs
#define SEGMENT_PINS {31, 28, 33, 9, 32, 30, 26, 29}
  std::array<uint8_t, 8> const            kSegmentPins SEGMENT_PINS;
  SegmentDisplay                          segmentDisplay SEGMENT_PINS;
#undef SEGMENT_PINS

  AudioControlSGTL5000          sgtl5000;

};
