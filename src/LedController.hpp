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
#include "GlobalState.hpp"

namespace EnlightingLetters
{

struct LedController
{
  const static uint8_t kLedPin = 12;
  const static uint8_t kBrightness = 100;
  const static int kTotalLedCount = 331;
  const static int kLastLedStripeOne = 149;
  const static int kLastLedStripeTwo = 209;
  const static int kLastLedStripeThree = 330;

  LedController();
  ~LedController() = default;
   
  CRGB mLedAccessor[kTotalLedCount];

  //### Public methods ###

  void CreateRandomPalette(CRGBPalette16& palette);

  void FillAllFromPalette(CRGBPalette16& palette, uint8_t index);

  void FillAllWithColor(CRGB color);

  void FillAllWithEnumColor(GlobalState::LightingColor& color);

  [[nodiscard]] CRGB CorrectColor(CRGB colorIn);
};

}  // namespace EnlightingLetters

#endif