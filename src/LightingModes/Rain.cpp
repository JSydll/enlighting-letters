#include "Rain.hpp"
#include <algorithm>
#include <cstdlib>

namespace EnlightingLetters
{

Rain::Rain(std::shared_ptr<LedController> controller) : mController(controller)
{
  mController->FillAllWithColor(CRGB::Black);
  FastLED.show();
}

void Rain::PerformUpdate()
{
  static int16_t startIndex = 0;
  static int16_t firstModifiedLed = 0;
  static int16_t endIndex = 0;
  static int8_t animationIndex = 0;
  // Animate rain drops
  for (auto& control : mSegmentControls)
  {
    if (not control.active)
    {
      control.active = (esp_random() % 1000 + 1) < 20;
      continue;
    }
    if (not control.reverse)
    {
      startIndex = std::min(control.seg.end, std::max(control.index, control.seg.begin));
      firstModifiedLed = startIndex;
      endIndex =
          std::min(control.seg.end, std::max(control.index + kAnimationSize, control.seg.begin));
      for (animationIndex = 0; startIndex < endIndex; animationIndex++)
      {
        mController->FillWithColor(startIndex++, CHSV(255, 0, kAnimationPane[animationIndex]));
      }
      control.index++;
    }
    else
    {
      startIndex = std::min(control.seg.begin, std::max(control.index, control.seg.end));
      firstModifiedLed = startIndex;
      endIndex =
          std::min(control.seg.begin, std::max(control.index - kAnimationSize, control.seg.end));
      for (animationIndex = 0; startIndex > endIndex; animationIndex++)
      {
        mController->FillWithColor(startIndex--, CHSV(255, 0, kAnimationPane[animationIndex]));
      }
      control.index--;
    }
    FastLED.show();
    mController->FillWithColor(firstModifiedLed, CRGB::Black);
    if (control.index == control.seg.end)
    {
      control.index = (control.reverse ? control.seg.begin + kAnimationSize
                                       : control.seg.begin - kAnimationSize);
      control.active = false;
    }
  }
  // Animate drops on the floor
  mFloorDropFlags |= (1 << (esp_random() % 128));
  mFloorDropFlags &= ~((1 << (esp_random() % 64)) | (1 << (esp_random() % 64)));
  static uint8_t index = 0;
  for (index = 0; index < 39; index++)
  {
    (mFloorDropFlags & (1 << index)
         ? mController->FillWithColor(kFloorBegin + (2 * index), CRGB::White)
         : mController->FillWithColor(kFloorBegin + (2 * index), CRGB::Black));
  }
  FastLED.show();
}

int Rain::GetStepsPerAnimation() { return 200; }

}  // namespace EnlightingLetters