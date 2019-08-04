#include "Pulse.hpp"

namespace EnlightingLetters
{

Pulse::Pulse(std::shared_ptr<GlobalController> state, std::shared_ptr<LedController> controller)
    : mGlobalController(state), mController(controller)
{}

void Pulse::PerformUpdate()
{
  (mGlobalController->data.mMusicActive ? PrepareMusicMode() : PrepareStaticMode());
  mCurrentBrightness = (mIsAscending ? mCurrentBrightness + 5 : mCurrentBrightness - 5);
  FastLED.show(mCurrentBrightness);
}

int Pulse::GetStepsPerAnimation()
{
  // The brightness increases in steps of 5 and decreases in steps of 5, from 0 to 255 and back.
  // So totally, we have around 50 steps per animation
  return 50;
}

void Pulse::PrepareMusicMode()
{
  if (mGlobalController->data.mHasBeat)
  {
    mIsAscending != mIsAscending;
  }
}

void Pulse::PrepareStaticMode()
{
  if (mCurrentBrightness == mController->mBrightness)
  {
    mIsAscending = false;
    return;
  }
  if (mCurrentBrightness == 0)
  {
    mIsAscending = true;
    if (mGlobalController->data.mColor != 0x000000)
    {
      mController->FillAllWithColor(mGlobalController->data.mColor);
      return;
    }
    mCurrentHue += 5;
    mController->FillAllWithColor(CHSV(mCurrentHue, 255, 255));
  }
}

}  // namespace EnlightingLetters