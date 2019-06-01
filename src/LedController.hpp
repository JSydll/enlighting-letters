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

#include "GlobalController.hpp"
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

  static std::shared_ptr<LedController> Create(std::shared_ptr<GlobalController> state);

  ~LedController() = default;

  void Update();

  void SetAnimationSpeed();

  void FillWithColor(uint16_t ledIndex, CRGB color);

  void FillWithColor(uint16_t ledIndex, GlobalController::LightingColor color);

  void FillAllWithColor(CRGB color);

  void FillAllWithColor(GlobalController::LightingColor& color);

  void CreateRandomPalette(CRGBPalette16& palette);

  void FillFromPalette(uint16_t ledIndex, CRGBPalette16& palette, uint8_t colorIndex);

  void FillAllFromPalette(CRGBPalette16& palette, uint8_t index);

 private:
  CRGB mLedAccessor[kTotalLedCount];
  std::shared_ptr<GlobalController> mGlobalController;
  std::chrono::steady_clock::time_point mLastUpdate;
  std::chrono::milliseconds mUpdateInterval;

  LedController(std::shared_ptr<GlobalController>& state);

  CRGB Convert(const GlobalController::LightingColor& color);

  CRGB CorrectColor(CRGB colorIn);
};

}  // namespace EnlightingLetters

#endif