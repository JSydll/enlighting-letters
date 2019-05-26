#include "Pulse.hpp"

namespace EnlightingLetters
{

Pulse::Pulse(std::shared_ptr<GlobalState>& state, std::shared_ptr<LedController>& controller)
    : mState(state), mController(controller)
{}

void Pulse::PerformUpdate()
{
  if (mCurrentBrightness == mController->kBrightness)
  {
    mIsAscending = false;
  }
  else if (mCurrentBrightness == 0)
  {
    mIsAscending = true;
    if (mState->mColor != GlobalState::LightingColor::RANDOM)
    {
      mController->FillAllWithColor(mState->mColor);
    }
    else
    {
      mCurrentHue += 5;
      mController->FillAllWithColor(CHSV(mCurrentHue, 255, 255));
    }
  }
  mCurrentBrightness = (mIsAscending ? mCurrentBrightness + 5 : mCurrentBrightness - 5);
  FastLED.show(mCurrentBrightness);
}

int Pulse::GetStepsPerAnimation()
{
  // The brightness increases in steps of 5 and decreases in steps of 5, from 0 to 255 and back.
  // So totally, we have around 50 steps per animation
  return 50;
}

}  // namespace EnlightingLetters