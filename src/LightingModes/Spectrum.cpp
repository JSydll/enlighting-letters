#include "Spectrum.hpp"
#include <algorithm>
#include <cstdlib>

namespace EnlightingLetters
{
Spectrum::Spectrum(std::shared_ptr<GlobalController> controller) : mController(controller)
{
  Fill(CRGBPalette16(PartyColors_p));
}

Spectrum::Spectrum(std::shared_ptr<GlobalController> controller, CRGBPalette16 palette)
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
      mController->ledController->FillFromPalette(index, palette, 0, false);
    }
  }
  // Fill bands but don't show them
  for (const auto& band : kChannels)
  {
    auto lLower = band.left.first;
    auto lUpper = band.left.second;
    for (index = lLower; index <= lUpper; ++index)
    {
      mController->ledController->FillFromPalette(index, palette, map(index, lLower, lUpper, 0, 15),
                                                  false);
    }
    auto rLower = band.right.second;
    auto rUpper = band.right.first;
    for (index = rLower; index <= rUpper; ++index)
    {
      mController->ledController->FillFromPalette(index, palette, map(index, rLower, rUpper, 15, 0),
                                                  false);
    }
  }
  // To preserve colors while adjusting brightness, operations on the HSV representation are needed
  mController->ledController->InitHSVApproximation();
  FastLED.show();
}

void Spectrum::PerformUpdate()
{
  static int freqNum = 0;
  static int mappedHeight = 0;
  static int index = 0;
  // For each frequency band calculate fill height
  for (freqNum = 0; freqNum < mController->data.mFrequencies.size(); ++freqNum)
  {
    auto& seg = kChannels[kBandToChannelMapping[freqNum]];
    mappedHeight = map(mController->data.mFrequencies[freqNum], 0,
                       mController->data.mMaxFrequencies[freqNum], 0, seg.height);
    index = seg.left.first;
    while (index < seg.left.first + mappedHeight)
    {
      mController->ledController->IncreaseBrightness(index++, 255);
    }
    while (index <= seg.left.second)
    {
      mController->ledController->ReduceBrightness(index++, 255);
    }
    index = seg.right.first;
    while (index > seg.right.first - mappedHeight)
    {
      mController->ledController->IncreaseBrightness(index--, 255);
    }
    while (index >= seg.right.second)
    {
      mController->ledController->ReduceBrightness(index--, 255);
    }
  }
  FastLED.show();
}

int Spectrum::GetStepsPerAnimation() { return 2; }

}  // namespace EnlightingLetters