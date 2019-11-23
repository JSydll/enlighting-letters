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
  std::shared_ptr<LedController> mLedController;

  const Segment kFloor = {257, 331};
  void FillFloor();

  const std::vector<Segment> kRandomnessSegments = {
      {15, 34}, {49, 68}, {115, 132}, {133, 148}, {157, 162}, {163, 171}, {190, 207}, {222, 242}};

  // Segment begin fixed, end is manipulated and limited by RandomnessSegments, index is used to
  // store target end
  std::vector<SegmentControl> mSegmentControls = {
      {false, false, {0, 34}, 34},     {false, true, {90, 49}, 49},
      {false, false, {91, 132}, 132},  {false, true, {150, 133}, 133},
      {false, false, {151, 162}, 162},  {false, true, {177, 163}, 163},
      {false, false, {178, 207}, 207}, {false, true, {256, 222}, 222}};

  void FillSegment(const SegmentControl &ctl);
};
}  // namespace EnlightingLetters

#endif