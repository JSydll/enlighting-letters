#include "Pulse.hpp"

namespace EnlightingLetters
{

Pulse::Pulse(std::shared_ptr<GlobalController> state, std::shared_ptr<LedController> controller)
    : mGlobalController(state), mController(controller)
{}

void Pulse::PerformUpdate()
{
  mCurrentBrightness = (mIsAscending ? mCurrentBrightness + 5 : mCurrentBrightness - 5);
  if (mCurrentBrightness == 150)
  {
    mIsAscending = false;
    return;
  }
  if (mCurrentBrightness == 0)
  {
    mIsAscending = true;
    if (mGlobalController->data.color != 0x000000)
    {
      mController->FillAllWithColor(mGlobalController->data.color);
      return;
    }
    mCurrentHue += 5;
    mController->FillAllWithColor(CHSV(mCurrentHue, 255, 255));
  }
  FastLED.show(mCurrentBrightness);
}

int Pulse::GetStepsPerAnimation()
{
  // The brightness increases in steps of 5 and decreases in steps of 5, from 0 to 255 and back.
  // So totally, we have around 50 steps per animation
  return 50;
}

}  // namespace EnlightingLetters