/**
 * @file Chaser.hpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2019-05-13
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef CHASER_HPP
#define CHASER_HPP

#include <memory>

#include "../ILightingProcessor.hpp"
#include "../LedController.hpp"

namespace EnlightingLetters
{
/**
 * @brief
 *
 */
class Chaser final : public ILightingProcessor
{
 public:
  Chaser(std::shared_ptr<GlobalController> state, std::shared_ptr<LedController> controller,
         CRGBPalette16 palette = CRGBPalette16(RainbowColors_p));
  virtual ~Chaser() = default;

  virtual void PerformUpdate() override;

  virtual int GetStepsPerAnimation() override;

 private:
  std::shared_ptr<GlobalController> mState;
  std::shared_ptr<LedController> mController;
  CRGBPalette16 mColorPalette;
  uint8_t mCurrentStartColorIndex = 0;
};
}  // namespace EnlightingLetters

#endif