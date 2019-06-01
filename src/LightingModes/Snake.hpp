/**
 * @file Snake.hpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2019-05-13
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef SNAKE_HPP
#define SNAKE_HPP

#include <memory>

#include "../LedController.hpp"
#include "../ILightingProcessor.hpp"

namespace EnlightingLetters
{
/**
 * @brief
 *
 */
class Snake final : public ILightingProcessor
{
 public:
  Snake(std::shared_ptr<LedController>& controller, CRGBPalette16 palette = CRGBPalette16(RainbowColors_p));
  virtual ~Snake() = default;

  virtual void PerformUpdate() override;

  virtual int GetStepsPerAnimation() override;

 private:
  std::shared_ptr<LedController> mController;
  CRGBPalette16 mColorPalette;
  uint8_t mCurrentStartColorIndex = 0;
};
}  // namespace EnlightingLetters

#endif