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
#include <math.h>
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
  const static int kTotalLedCount = 332;
  const static int kLastLedStripeOne = 149;
  const static int kLastLedStripeTwo = 209;
  const static int kLastLedStripeThree = 331;
  // Initial brightness
  uint8_t mBrightness = 50;

  static std::shared_ptr<LedController> Create(std::shared_ptr<GlobalController> state);

  ~LedController() = default;

  void Update();

  void SetAnimationSpeed();

  void SetTotalBrightness(int brightness);

  void FillWithColor(uint16_t ledIndex, CRGB color);

  void FillAllWithColor(CRGB color);

  void CreateRandomPalette(CRGBPalette16& palette);

  void FillFromPalette(uint16_t ledIndex, const CRGBPalette16& palette, uint8_t colorIndex,
                       bool blend = true);

  void FillAllFromPalette(const CRGBPalette16& palette, uint8_t index);

  void InitHSVApproximation();

  void IncreaseBrightness(uint16_t ledIndex, uint8_t amount);

  void ReduceBrightness(uint16_t ledIndex, uint8_t amount);

  CRGB mLedAccessor[kTotalLedCount];

 private:
  std::shared_ptr<GlobalController> mGlobalController;
  std::chrono::steady_clock::time_point mLastUpdate;
  std::chrono::milliseconds mUpdateInterval;
  // To use HSV operations on the LEDs, a shadow array of HSV values must be stored. Ideally, this
  // is only seldomly build from scratch, as this takes quite some time.
  std::vector<CHSV> mHSVApprox;

  LedController(std::shared_ptr<GlobalController>& state);

  CRGB CorrectColor(CRGB colorIn);
};

}  // namespace EnlightingLetters

#endif