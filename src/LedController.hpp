/**
 * @file LedController.hpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2019-05-13
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef LEDCONTROLLER_HPP
#define LEDCONTROLLER_HPP

#include <FastLED.h>
#include <chrono>
#include <memory>

#include "GlobalState.hpp"
#include "ILightingProcessor.hpp"

namespace EnlightingLetters
{

class LedController final
{
 public:
  const static uint8_t kLedPin = 12;
  const static uint8_t kBrightness = 100;
  const static int kTotalLedCount = 331;
  const static int kLastLedStripeOne = 149;
  const static int kLastLedStripeTwo = 209;
  const static int kLastLedStripeThree = 330;

  static std::shared_ptr<LedController> Create(std::shared_ptr<GlobalState>& state);

  ~LedController() = default;

  void Update();

  void SetProcessor(std::shared_ptr<ILightingProcessor> processor);

  void SetAnimationSpeed();

  void FillWithColor(uint16_t ledIndex, CRGB color);

  void FillWithColor(uint16_t ledIndex, GlobalState::LightingColor color);

  void FillAllWithColor(CRGB color);

  void FillAllWithColor(GlobalState::LightingColor& color);

  void CreateRandomPalette(CRGBPalette16& palette);

  void FillFromPalette(uint16_t ledIndex, CRGBPalette16& palette, uint8_t colorIndex);

  void FillAllFromPalette(CRGBPalette16& palette, uint8_t index);

private:
  CRGB mLedAccessor[kTotalLedCount];
  std::shared_ptr<GlobalState> mState;
  std::shared_ptr<ILightingProcessor> mProcessor;
  std::chrono::steady_clock::time_point mLastUpdate;
  std::chrono::milliseconds mUpdateInterval;

  LedController(std::shared_ptr<GlobalState>& state);

  CRGB Convert(const GlobalState::LightingColor& color);

  CRGB CorrectColor(CRGB colorIn);
};

}  // namespace EnlightingLetters

#endif