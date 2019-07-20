#include "Chaser.hpp"

namespace EnlightingLetters
{

Chaser::Chaser(std::shared_ptr<LedController> controller, CRGBPalette16 palette) : mController(controller), mColorPalette(palette)
{
  mColorPalette[0] = CRGB::Black;
  mColorPalette[1] = CRGB::Black;
  mColorPalette[14] = CRGB::Black;
  mColorPalette[15] = CRGB::Black;
}

void Chaser::PerformUpdate()
{
  auto colorIndex = mCurrentStartColorIndex;
  for(uint16_t ledIndex = 0; ledIndex < mController->kTotalLedCount; ledIndex++)
  {
    mController->FillFromPalette(ledIndex, mColorPalette, colorIndex++);
  }
  mCurrentStartColorIndex++;
  FastLED.show();
}

int Chaser::GetStepsPerAnimation()
{
  // The Chaser will move one led further each cycle. The base speed is assumed to be 1 segment 
  // (16 leds) per 2 seconds which results in 125 steps.
  return 250;
}

}  // namespace EnlightingLetters