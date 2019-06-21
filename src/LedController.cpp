#include "LedController.hpp"

namespace EnlightingLetters
{
using namespace std::chrono;

std::shared_ptr<LedController> LedController::Create(std::shared_ptr<GlobalController> state)
{
  return std::shared_ptr<LedController>(new LedController(state));
}

void LedController::Update()
{
  if (duration_cast<milliseconds>(steady_clock::now() - mLastUpdate) > mUpdateInterval)
  {
    if (mGlobalController->lightingProcessor)
    {
      mGlobalController->lightingProcessor->PerformUpdate();
    }
    mLastUpdate = steady_clock::now();
  }
}

void LedController::SetAnimationSpeed()
{
  if (mGlobalController->lightingProcessor)
  {
    mUpdateInterval = milliseconds(mGlobalController->data.mAnimationSpeed /
                                   mGlobalController->lightingProcessor->GetStepsPerAnimation());
  }
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

void LedController::FillAllWithColor(GlobalController::LightingColor& color)
{
  FillAllWithColor(Convert(color));
}

void LedController::FillWithColor(uint16_t ledIndex, CRGB color)
{
  mLedAccessor[ledIndex] =
      ((ledIndex > kLastLedStripeOne) and (ledIndex <= kLastLedStripeTwo) ? CorrectColor(color)
                                                                          : color);
}

void LedController::FillWithColor(uint16_t ledIndex, GlobalController::LightingColor color)
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

void LedController::FillFromPalette(uint16_t ledIndex, const CRGBPalette16& palette,
                                    uint8_t colorIndex)
{
  mLedAccessor[ledIndex] =
      ((ledIndex > kLastLedStripeOne) and (ledIndex <= kLastLedStripeTwo)
           ? CorrectColor(ColorFromPalette(palette, colorIndex, 255, LINEARBLEND))
           : ColorFromPalette(palette, colorIndex, 255, LINEARBLEND));
}

void LedController::FillAllFromPalette(const CRGBPalette16& palette, uint8_t index)
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

void LedController::InitHSVApproximation()
{
  if (not mHSVApprox.empty())
  {
    mHSVApprox.clear();
  }
  mHSVApprox.reserve(sizeof(mLedAccessor));
  for (const auto& led : mLedAccessor)
  {
    mHSVApprox.push_back(rgb2hsv_approximate(led));
  }
}

void LedController::IncreaseBrightness(uint16_t ledIndex, uint8_t amount)
{
  if (not mHSVApprox.empty())
  {
    mHSVApprox[ledIndex].value = clip(static_cast<uint8_t>(mHSVApprox[ledIndex].value + amount),
                                      mHSVApprox[ledIndex].value, static_cast<uint8_t>(255));
    mLedAccessor[ledIndex] = mHSVApprox[ledIndex];
  }
}

void LedController::ReduceBrightness(uint16_t ledIndex, uint8_t amount)
{
  if (not mHSVApprox.empty())
  {
    mHSVApprox[ledIndex].value = clip(static_cast<uint8_t>(mHSVApprox[ledIndex].value - amount),
                                      static_cast<uint8_t>(0), mHSVApprox[ledIndex].value);
    mLedAccessor[ledIndex] = mHSVApprox[ledIndex];
  }
}

LedController::LedController(std::shared_ptr<GlobalController>& state) : mGlobalController(state)
{
  FastLED.addLeds<WS2812B, kLedPin>(mLedAccessor, kTotalLedCount).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(kBrightness);
}

CRGB LedController::Convert(const GlobalController::LightingColor& color)
{
  using lc = GlobalController::LightingColor;
  switch (color)
  {
    case lc::RED: return CRGB::Red;
    case lc::YELLOW: return CRGB::Yellow;
    case lc::ORANGE: return CRGB::Orange;
    case lc::GREEN: return CRGB::Green;
    case lc::BLUE: return CRGB::Blue;
    case lc::VIOLET: return CRGB::Violet;
    case lc::PINK: return CRGB::Pink;
    case lc::BLACK: return CRGB::Black;
    default:;
  }
  return CRGB::White;
}

CRGB LedController::CorrectColor(CRGB colorIn)
{
  if (colorIn.r == colorIn.g and colorIn.g == colorIn.b)
  {
    return colorIn;
  }
  return CRGB((colorIn.g > 29 ? colorIn.g - 30 : 0), (colorIn.r < 241 ? colorIn.r + 15 : 255),
              colorIn.b);
}

}  // namespace EnlightingLetters