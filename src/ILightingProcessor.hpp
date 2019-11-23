/**
 * @file ILightingProcessor.hpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2019-05-13
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef ILIGHTINGPROCESSOR_HPP
#define ILIGHTINGPROCESSOR_HPP

namespace EnlightingLetters
{

class ILightingProcessor
{
 public:
  ILightingProcessor() = default;
  virtual ~ILightingProcessor() = default;

  virtual void PerformUpdate() = 0;

  virtual int GetStepsPerAnimation() = 0;

  struct Segment
  {
    int begin;
    int end;
  };

  struct SegmentControl
  {
    bool active;
    bool reverse;
    Segment seg;
    int index;
  };
};

}  // namespace EnlightingLetters

#endif