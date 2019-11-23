/**
 * @file SolidFill.hpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2019-05-13
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef SOLIDFILL_HPP
#define SOLIDFILL_HPP

#include <memory>

#include "../GlobalController.hpp"
#include "../ILightingProcessor.hpp"
#include "../LedController.hpp"

namespace EnlightingLetters
{
/**
 * @brief
 *
 */
class SolidFill final : public ILightingProcessor
{
 public:
  SolidFill(std::shared_ptr<GlobalController> state);
  virtual ~SolidFill() = default;

  virtual void PerformUpdate() override;

  virtual int GetStepsPerAnimation() override;

 private:
  std::shared_ptr<GlobalController> mGlobalController;
  int mCurrentColor;
};
}  // namespace EnlightingLetters

#endif