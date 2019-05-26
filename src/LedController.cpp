#include "LedController.hpp"

namespace EnlightingLetters
{

LedController::LedController()
{
  FastLED.addLeds<WS2812B, kLedPin>(mLedAccessor, kTotalLedCount).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(kBrightness);
}

void LedController::CreateRandomPalette(CRGBPalette16& palette)
{
  for (int i = 0; i < 16; i++)
  {
    palette[i] = CHSV(random8(), 255, 255);
  }
}

void LedController::FillAllFromPalette(CRGBPalette16& palette, uint8_t index)
{
  int i = 0;
  while(i <= kLastLedStripeOne)
  {
    mLedAccessor[i++] =  ColorFromPalette(palette, index, 255, LINEARBLEND);
  }
  while(i <= kLastLedStripeTwo)
  {
    mLedAccessor[i++] = CorrectColor(ColorFromPalette(palette, index, 255, LINEARBLEND));
  }
  while(i <= kLastLedStripeThree)
  {
    mLedAccessor[i++] = ColorFromPalette(palette, index, 255, LINEARBLEND);
  }
}

void LedController::FillAllWithColor(CRGB color)
{
  int i = 0;
  while(i <= kLastLedStripeOne)
  {
    mLedAccessor[i++] = color;
  }
  while(i <= kLastLedStripeTwo)
  {
    mLedAccessor[i++] = CorrectColor(color);
  }
  while(i <= kLastLedStripeThree)
  {
    mLedAccessor[i++] = color;
  }
}

void LedController::FillAllWithEnumColor(GlobalState::LightingColor& color)
{
  using lc = GlobalState::LightingColor;
  CRGB fastLedColor;
  switch(color)
  {
    case lc::RED: fastLedColor = CRGB::Red; break;
    case lc::YELLOW: fastLedColor = CRGB::Yellow; break;
    case lc::ORANGE: fastLedColor = CRGB::Orange; break;
    case lc::GREEN: fastLedColor = CRGB::Green; break;
    case lc::BLUE: fastLedColor = CRGB::Blue; break;
    case lc::VIOLET: fastLedColor = CRGB::Violet; break;
    case lc::PINK: fastLedColor = CRGB::Pink; break;
    default: fastLedColor = CRGB::White;
  }
  FillAllWithColor(fastLedColor);
}

CRGB LedController::CorrectColor(CRGB colorIn)
{
  return CRGB((colorIn.g > 29 ? colorIn.g - 30 : 0), (colorIn.r < 241 ? colorIn.r + 15 : 255), colorIn.b);
}

}  // namespace EnlightingLetters