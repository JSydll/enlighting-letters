#include "SolidFill.hpp"

#include "../LedController.hpp"

namespace EnlightingLetters
{

SolidFill::SolidFill(std::shared_ptr<GlobalController> state) : mGlobalController(state)
{
  mCurrentColor = 0x000000;
}

void SolidFill::PerformUpdate()
{
  if (mCurrentColor != mGlobalController->data.color)
  {
    mCurrentColor = mGlobalController->data.color;
    mGlobalController->ledController->FillAllWithColor(mCurrentColor);
  }
  FastLED.show();
}

int SolidFill::GetStepsPerAnimation()
{
  // The base speed is assumed to be 1600 milliseconds so cutting that in half seems legit.
  return 2;
}

}  // namespace EnlightingLetters