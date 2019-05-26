/**
 * @file Pulse.hpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2019-05-13
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef PULSE_HPP
#define PULSE_HPP

#include <memory>

#include "../GlobalState.hpp"
#include "../ILightingProcessor.hpp"
#include "../LedController.hpp"

namespace EnlightingLetters
{
/**
 * @brief
 *
 */
class Pulse final : public ILightingProcessor
{
 public:
  Pulse(std::shared_ptr<GlobalState>& state, std::shared_ptr<LedController>& controller);
  virtual ~Pulse() = default;

  virtual void PerformUpdate() override;

  virtual int GetStepsPerAnimation() override;

 private:
  std::shared_ptr<GlobalState> mState;
  std::shared_ptr<LedController> mController;
  bool mIsAscending = true;
  uint8_t mCurrentHue = 0;
  uint8_t mCurrentBrightness = 0;
};
}  // namespace EnlightingLetters

#endif