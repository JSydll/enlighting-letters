/**
 * @file Fire.hpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2019-05-13
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef FIRE_HPP
#define FIRE_HPP

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
class Fire final : public ILightingProcessor
{
 public:
  Fire(std::shared_ptr<GlobalController> state);
  virtual ~Fire() = default;

  virtual void PerformUpdate() override;

  virtual int GetStepsPerAnimation() override;

 private:
  std::shared_ptr<GlobalController> mGlobalController;
};
}  // namespace EnlightingLetters

#endif