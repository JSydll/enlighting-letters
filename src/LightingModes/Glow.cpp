#include "Glow.hpp"

namespace EnlightingLetters
{

Glow::Glow(std::shared_ptr<LedController>& controller) : mController(controller)
{
  mColorPalette = RainbowColors_p;
}

void Glow::PerformUpdate()
{
  mController->FillAllFromPalette(mColorPalette, mCurrentStartColorIndex);
  mCurrentStartColorIndex++;
  FastLED.show();
}

int Glow::GetStepsPerAnimation()
{
  // The Glow will move one led further each cycle. The base speed is assumed to be 1 segment 
  // (16 leds) per 2 seconds which results in 125 steps.
  return 125;
}

}  // namespace EnlightingLetters