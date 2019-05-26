#include "LedController.hpp"

namespace EnlightingLetters
{
using namespace std::chrono;

std::shared_ptr<LedController> LedController::Create(std::shared_ptr<GlobalState>& state)
{
  return std::shared_ptr<LedController>(new LedController(state));
}

void LedController::Update()
{
  if (duration_cast<milliseconds>(steady_clock::now() - mLastUpdate) > mUpdateInterval)
  {
    if (mProcessor)
    {
      mProcessor->PerformUpdate();
    }
    mLastUpdate = steady_clock::now();
  }
}

void LedController::SetProcessor(std::shared_ptr<ILightingProcessor> processor)
{
  mProcessor = processor;
  SetAnimationSpeed();
}

void LedController::SetAnimationSpeed()
{
  mUpdateInterval = milliseconds(mState->mAnimationSpeed / mProcessor->GetStepsPerAnimation());
}

void LedController::FillAllWithColor(CRGB color)
{
  int i = 0;
  while (i <= kLastLedStripeOne)
  {
    mLedAccessor[i++] = color;
  }
  while (i <= kLastLedStripeTwo)
  {
    mLedAccessor[i++] = CorrectColor(color);
  }
  while (i <= kLastLedStripeThree)
  {
    mLedAccessor[i++] = color;
  }
}

void LedController::FillAllWithColor(GlobalState::LightingColor& color)
{
  FillAllWithColor(Convert(color));
}

void LedController::FillWithColor(uint16_t ledIndex, CRGB color)
{
  mLedAccessor[ledIndex] =
      ((ledIndex > kLastLedStripeOne) and (ledIndex <= kLastLedStripeTwo) ? CorrectColor(color)
                                                                          : color);
}

void LedController::FillWithColor(uint16_t ledIndex, GlobalState::LightingColor& color)
{
  FillWithColor(ledIndex, Convert(color));
}

void LedController::CreateRandomPalette(CRGBPalette16& palette)
{
  for (int i = 0; i < 16; i++)
  {
    palette[i] = CHSV(random8(), 255, 255);
  }
}

void LedController::FillFromPalette(uint16_t ledIndex, CRGBPalette16& palette, uint8_t colorIndex)
{
  mLedAccessor[ledIndex] =
      ((ledIndex > kLastLedStripeOne) and (ledIndex <= kLastLedStripeTwo)
           ? CorrectColor(ColorFromPalette(palette, colorIndex, 255, LINEARBLEND))
           : ColorFromPalette(palette, colorIndex, 255, LINEARBLEND));
}

void LedController::FillAllFromPalette(CRGBPalette16& palette, uint8_t index)
{
  int i = 0;
  while (i <= kLastLedStripeOne)
  {
    mLedAccessor[i++] = ColorFromPalette(palette, index, 255, LINEARBLEND);
  }
  while (i <= kLastLedStripeTwo)
  {
    mLedAccessor[i++] = CorrectColor(ColorFromPalette(palette, index, 255, LINEARBLEND));
  }
  while (i <= kLastLedStripeThree)
  {
    mLedAccessor[i++] = ColorFromPalette(palette, index, 255, LINEARBLEND);
  }
}

LedController::LedController(std::shared_ptr<GlobalState>& state) : mState(state)
{
  FastLED.addLeds<WS2812B, kLedPin>(mLedAccessor, kTotalLedCount).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(kBrightness);
}

CRGB LedController::Convert(const GlobalState::LightingColor& color)
{
  using lc = GlobalState::LightingColor;
  switch (color)
  {
    case lc::RED: return CRGB::Red;
    case lc::YELLOW: return CRGB::Yellow;
    case lc::ORANGE: return CRGB::Orange;
    case lc::GREEN: return CRGB::Green;
    case lc::BLUE: return CRGB::Blue;
    case lc::VIOLET: return CRGB::Violet;
    case lc::PINK: return CRGB::Pink;
    default:;
  }
  return CRGB::White;
}

CRGB LedController::CorrectColor(CRGB colorIn)
{
  return CRGB((colorIn.g > 29 ? colorIn.g - 30 : 0), (colorIn.r < 241 ? colorIn.r + 15 : 255),
              colorIn.b);
}

}  // namespace EnlightingLetters