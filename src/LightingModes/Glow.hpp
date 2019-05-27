/**
 * @file Glow.hpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2019-05-13
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef GLOW_HPP
#define GLOW_HPP

#include <memory>

#include "../LedController.hpp"
#include "../ILightingProcessor.hpp"

namespace EnlightingLetters
{
/**
 * @brief
 *
 */
class Glow final : public ILightingProcessor
{
 public:
  Glow(std::shared_ptr<LedController>& controller, CRGBPalette16 palette = RainbowColors_p);
  virtual ~Glow() = default;

  virtual void PerformUpdate() override;

  virtual int GetStepsPerAnimation() override;

 private:
  std::shared_ptr<LedController> mController;
  CRGBPalette16 mColorPalette;
  uint8_t mCurrentStartColorIndex = 0;
};
}  // namespace EnlightingLetters

#endif