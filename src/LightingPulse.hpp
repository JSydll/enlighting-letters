/**
 * @file LightingPulse.hpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2019-05-13
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef LIGHTINGPULSE_HPP
#define LIGHTINGPULSE_HPP

#include <memory>
#include <chrono>

#include "GlobalState.hpp"
#include "LedController.hpp"
#include "ILightingMode.hpp"

namespace EnlightingLetters
{
/**
 * @brief
 *
 */
class LightingPulse final : public ILightingMode
{
 public:
  LightingPulse(std::shared_ptr<GlobalState>& state, std::shared_ptr<LedController>& controller);
  virtual ~LightingPulse() = default;

  virtual void Next() override;

  virtual void SetAnimationSpeed() override;

 private:
  std::shared_ptr<GlobalState> mState;
  std::shared_ptr<LedController> mController;
  std::chrono::steady_clock::time_point mLastUpdate;
  std::chrono::milliseconds mUpdateInterval;
  bool mIsAscending = true;
  uint8_t mCurrentHue = 0;
  uint8_t mCurrentBrightness = 0;
};
}  // namespace EnlightingLetters

#endif