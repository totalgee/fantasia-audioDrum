#ifndef __FANTASIA_HPP__
#define __FANTASIA_HPP__

#include <array>

#include <Audio.h>
#include <Encoder.h>

// Fantasia buttons and pots
class Fantasia {
public:
  void setupAudio();
  void update();

  static constexpr int kNumPots = 4;
  enum class Pot { A, B, C, D };
  static std::array<Pot, kNumPots> const pots;
  float get(Pot pot) const { return potValues[static_cast<int>(pot)]; }

  static constexpr int kNumButtons = 4;
  enum class Button { A, B, C, D };
  static std::array<Button, kNumButtons> const buttons;
  bool get(Button button) const { return buttonValues[static_cast<int>(button)]; }

  // @todo get() for gates and encoder
  static constexpr int kNumGates = 2;

private:
  std::array<float, kNumPots>   potValues{};
  std::array<bool, kNumPots>    buttonValues{};
  // @todo                      gateValues

  Encoder                       encoder{ PIN_SERIAL_RX, PIN_SERIAL_TX };
  bool                          encoderPressed = false;
  int32_t                       encoderPosition;

  AudioControlSGTL5000          sgtl5000;

};

#endif // __FANTASIA_HPP__
