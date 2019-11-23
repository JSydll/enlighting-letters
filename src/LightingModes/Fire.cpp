#include "Fire.hpp"

namespace EnlightingLetters
{

namespace
{
const CRGBPalette16 kFirePalette(CRGB::Orange, CRGB::DarkRed, CRGB::Black, CRGB::Black);

int RandomFromInterval(int min, int max) { return (std::rand() % (max - min + 1) + min); }
}

void Fire::FillFloor()
{
  for (auto led = kFloor.begin; led <= kFloor.end; ++led)
  {
    mLedController->FillWithColor(led, CRGB::Yellow);
  }
}

void Fire::FillSegment(const SegmentControl& ctl)
{
  int16_t led = ctl.seg.begin;
  int16_t colorIndex = 1;
  int16_t colorIncrement = 220 / (abs(ctl.seg.begin - ctl.seg.end));
  do
  {
    mLedController->FillFromPalette(led, kFirePalette, colorIndex++ * colorIncrement);
    (ctl.reverse ? --led : ++led);
  } while (led != ctl.seg.end);
}

Fire::Fire(std::shared_ptr<GlobalController> state)
    : mGlobalController(state), mLedController(state->ledController)
{
}

void Fire::PerformUpdate()
{
  mLedController->FillAllWithColor(CRGB::Black);
  FillFloor();
  for (int i = 0; i < mSegmentControls.size(); ++i)
  {
    auto& ctl = mSegmentControls[i];
    FillSegment(ctl);
    if (not ctl.active)
    {
      ctl.active = (esp_random() % 4 == 0);
      continue;
    }
    if (ctl.seg.end == ctl.index)
    {
      ctl.active = false;
      ctl.index = RandomFromInterval(kRandomnessSegments[i].begin, kRandomnessSegments[i].end);
      continue;
    }
    ((ctl.seg.end < ctl.index) ? ++ctl.seg.end : --ctl.seg.end);
  }
  FastLED.show();
}

int Fire::GetStepsPerAnimation() { return 10; }

}  // namespace EnlightingLetters