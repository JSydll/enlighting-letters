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

#include "GlobalState.hpp"
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
  LightingPulse(std::shared_ptr<GlobalState> state);
  virtual ~LightingPulse() = default;

  virtual void Next() override;

 private:
  std::shared_ptr<GlobalState> mState;
};
}  // namespace EnlightingLetters

#endif