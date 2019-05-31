/**
 * @file Rain.hpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2019-05-13
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef RAIN_HPP
#define RAIN_HPP

#include <array>
#include <chrono>
#include <memory>
#include <vector>

#include "../ILightingProcessor.hpp"
#include "../LedController.hpp"

namespace EnlightingLetters
{
/**
 * @brief
 *
 */
class Rain final : public ILightingProcessor
{
 public:
  Rain(std::shared_ptr<LedController>& controller);
  virtual ~Rain() = default;

  virtual void PerformUpdate() override;

  virtual int GetStepsPerAnimation() override;

 private:
  std::shared_ptr<LedController> mController;

  struct SegmentControl
  {
    bool mActiveSegment;
    bool mIsReverse;
    int mBegin;
    int mEnd;
    int mVirtualBegin;
  };
  const std::array<uint8_t, 16> kAnimationPane{
      {10, 30, 80, 120, 180, 255, 255, 255, 255, 255, 255, 180, 120, 80, 30, 10}};
  const int kAnimationSize = static_cast<int>(kAnimationPane.size());
  std::vector<SegmentControl> mSegmentControls = {{true, true, 37, 0, 37 + kAnimationSize},
                                                  {false, false, 45, 78, 45 - kAnimationSize},
                                                  {false, true, 132, 109, 132 + kAnimationSize},
                                                  {true, false, 131, 150, 131 - kAnimationSize},
                                                  {false, true, 211, 178, 211 + kAnimationSize},
                                                  {false, false, 219, 252, 219 - kAnimationSize}};

  const int kFloorBegin = 254;
  uint64_t mFloorDropFlags = 0;
};
}  // namespace EnlightingLetters

#endif