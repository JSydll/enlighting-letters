#include "Rain.hpp"
#include <algorithm>
#include <cstdlib>

namespace EnlightingLetters
{

Rain::Rain(std::shared_ptr<LedController>& controller) : mController(controller)
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
  for (auto& seg : mSegmentControls)
  {
    if (not seg.mActiveSegment)
    {
      seg.mActiveSegment = (std::rand() % 1000 + 1) < 20;
      continue;
    }
    if (not seg.mIsReverse)
    {
      startIndex = std::min(seg.mEnd, std::max(seg.mVirtualBegin, seg.mBegin));
      firstModifiedLed = startIndex;
      endIndex = std::min(seg.mEnd, std::max(seg.mVirtualBegin + 16, seg.mBegin));
      for (animationIndex = 0; startIndex < endIndex; animationIndex++)
      {
        mController->FillWithColor(startIndex++, CHSV(255, 0, mAnimationPane[animationIndex]));
      }
      seg.mVirtualBegin++;
    }
    else
    {
      startIndex = std::min(seg.mBegin, std::max(seg.mVirtualBegin, seg.mEnd));
      firstModifiedLed = startIndex;
      endIndex = std::min(seg.mBegin, std::max(seg.mVirtualBegin - 16, seg.mEnd));
      for (animationIndex = 0; startIndex > endIndex; animationIndex++)
      {
        mController->FillWithColor(startIndex--, CHSV(255, 0, mAnimationPane[animationIndex]));
      }
      seg.mVirtualBegin--;
    }
    FastLED.show();
    mController->FillWithColor(firstModifiedLed, CRGB::Black);
    if (seg.mVirtualBegin == seg.mEnd)
    {
      seg.mVirtualBegin = (seg.mIsReverse ? seg.mBegin + 16 : seg.mBegin - 16);
      seg.mActiveSegment = false;
    }
  }
  // Animate drops on the floor
  mFloorDropFlags |= (1 << (std::rand() % 128));
  mFloorDropFlags &= ~((1 << (std::rand() % 64)) | (1 << (std::rand() % 64)));
  static uint8_t index = 0;
  for (index = 0; index < 39; index++)
  {
    (mFloorDropFlags & (1 << index) ? mController->FillWithColor(mFloorBegin + (2 * index), CRGB::White)
                                    : mController->FillWithColor(mFloorBegin + (2 * index), CRGB::Black));
  }
  FastLED.show();
}

int Rain::GetStepsPerAnimation() { return 400; }

}  // namespace EnlightingLetters