#include "Snake.hpp"

namespace EnlightingLetters
{

Snake::Snake(std::shared_ptr<LedController> controller, CRGBPalette16 palette) : mController(controller), mColorPalette(palette)
{
}

void Snake::PerformUpdate()
{
  auto colorIndex = mCurrentStartColorIndex;
  for(uint16_t ledIndex = 0; ledIndex < mController->kTotalLedCount; ledIndex++)
  {
    mController->FillFromPalette(ledIndex, mColorPalette, colorIndex++);
  }
  mCurrentStartColorIndex++;
  FastLED.show();
}

int Snake::GetStepsPerAnimation()
{
  // The snake will move one led further each cycle. The base speed is assumed to be 1 segment 
  // (16 leds) per 2 seconds which results in 125 steps.
  return 250;
}

}  // namespace EnlightingLetters