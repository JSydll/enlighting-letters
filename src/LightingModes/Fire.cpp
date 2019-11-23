#include "Fire.hpp"

namespace EnlightingLetters
{

Fire::Fire(std::shared_ptr<GlobalController> state) : mGlobalController(state)
{
}

void Fire::PerformUpdate()
{
  FastLED.show();
}

int Fire::GetStepsPerAnimation()
{
  // The Fire will move one led further each cycle. The base speed is assumed to be 1 segment 
  // (16 leds) per 2 seconds which results in 125 steps.
  return 125;
}

}  // namespace EnlightingLetters