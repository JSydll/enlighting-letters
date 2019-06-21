#include "Spectrum.hpp"
#include <algorithm>
#include <cstdlib>

namespace EnlightingLetters
{

Spectrum::Spectrum(std::shared_ptr<GlobalController>& controller) : mController(controller)
{
  Fill(CRGBPalette16(MusicSpectrum_p));
}

Spectrum::Spectrum(std::shared_ptr<GlobalController>& controller, CRGBPalette16 palette)
    : mController(controller)
{
  Fill(palette);
}

void Spectrum::Fill(const CRGBPalette16& palette)
{
  int index = 0;
  // Fill base with first color in palette
  for (const auto& seg : kFloor)
  {
    for (index = seg.first; index <= seg.second; ++index)
    {
      mController->ledController->FillFromPalette(index, palette, 0);
    }
  }
  // Fill bands but don't show them
  for (const auto& band : kSegments)
  {
    auto lLower = band.left.first;
    auto lUpper = band.left.second;
    for (index = lLower; index <= lUpper; ++index)
    {
      mController->ledController->FillFromPalette(index, palette,
                                                  map(index, lLower, lUpper, 0, 15));
      mController->ledController->mLedAccessor[index].fadeToBlackBy(255);
    }
    auto rLower = band.right.second;
    auto rUpper = band.right.first;
    for (index = rLower; index <= rUpper; ++index)
    {
      mController->ledController->FillFromPalette(index, palette,
                                                  map(index, rLower, rUpper, 0, 15));
      mController->ledController->mLedAccessor[index].fadeToBlackBy(255);
    }
  }
  FastLED.show();
}

void Spectrum::PerformUpdate()
{
  static int currentValue = 0;
  static int mappedHeight = 0;
  static int index = 0, lLower = 0, lUpper = 0, rLower = 0, rUpper = 0;
  // For each frequency band calculate fill height
  for (auto freqNum = 0; freqNum < mController->data.mFrequencies.size(); ++freqNum)
  {
    currentValue = static_cast<int>(round(mController->data.mFrequencies[freqNum]));
    if (currentValue > mMaxFrequencies[freqNum])
    {
      mMaxFrequencies[freqNum] = currentValue;
    }
    mappedHeight = map(currentValue, 0, mMaxFrequencies[freqNum], 0, kSegments[freqNum].height);
    lLower = kSegments[freqNum].left.first;
    lUpper = kSegments[freqNum].left.second;
    index = lLower;
    while (index <= lLower + mappedHeight)
    {
      mController->ledController->mLedAccessor[index++].maximizeBrightness();
    }
    while (index <= lUpper)
    {
      mController->ledController->mLedAccessor[index++].fadeToBlackBy(255);
    }
    rLower = kSegments[freqNum].right.second;
    rUpper = kSegments[freqNum].right.first;
    index = rLower;
    while (index <= rLower + mappedHeight)
    {
      mController->ledController->mLedAccessor[index++].maximizeBrightness();
    }
    while (index <= rUpper)
    {
      mController->ledController->mLedAccessor[index++].fadeToBlackBy(255);
    }
  }
  FastLED.show();
}

int Spectrum::GetStepsPerAnimation() { return 400; }

}  // namespace EnlightingLetters