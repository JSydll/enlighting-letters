#include "LightingPulse.hpp"

namespace EnlightingLetters
{
using namespace std::chrono;

LightingPulse::LightingPulse(std::shared_ptr<GlobalState>& state, std::shared_ptr<LedController>& controller) 
    : mState(state), mController(controller) 
{
  mLastUpdate = steady_clock::now();
  SetAnimationSpeed();
}

void LightingPulse::Next() { 
    if(duration_cast<milliseconds>(steady_clock::now() - mLastUpdate) > mUpdateInterval)
    {
      if(mCurrentBrightness == mController->kBrightness)
      {
        mIsAscending = false; 
      }
      else if(mCurrentBrightness == 0)
      {
        mIsAscending = true; 
        if(mState->mColor != GlobalState::LightingColor::RANDOM)
        {
          mController->FillAllWithEnumColor(mState->mColor);
        }
        else
        {
          mCurrentHue += 5;
          mController->FillAllWithColor(CHSV(mCurrentHue, 255, 255));
        }
      }
      mCurrentBrightness = (mIsAscending ? mCurrentBrightness + 5  : mCurrentBrightness - 5);
      FastLED.show(mCurrentBrightness);
      mLastUpdate = steady_clock::now();
    }
}

void LightingPulse::SetAnimationSpeed()
{
  // The brightness increases in steps of 5 and decreases in steps of 5, from 0 to 255 and back.
  // Therefore, the desired animation speed is only achieved when the update interval is accordingly.
  mUpdateInterval = milliseconds(mState->mAnimationSpeed / 50);
}

}  // namespace EnlightingLetters