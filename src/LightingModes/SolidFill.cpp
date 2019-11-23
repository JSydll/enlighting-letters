#include "SolidFill.hpp"

namespace EnlightingLetters
{

SolidFill::SolidFill(std::shared_ptr<GlobalController> state) : mGlobalController(state)
{
}

void SolidFill::PerformUpdate()
{
  FastLED.show();
}

int SolidFill::GetStepsPerAnimation()
{
  // The SolidFill will move one led further each cycle. The base speed is assumed to be 1 segment 
  // (16 leds) per 2 seconds which results in 125 steps.
  return 125;
}

}  // namespace EnlightingLetters