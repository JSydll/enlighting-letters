/**
 * @file ILightingMode.hpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2019-05-13
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef ILIGHTINGMODE_HPP
#define ILIGHTINGMODE_HPP

namespace EnlightingLetters
{

class ILightingMode
{
 public:
  ILightingMode() = default;
  virtual ~ILightingMode() = default;

  virtual void Next() = 0;

  virtual void SetAnimationSpeed() = 0;
};

}  // namespace EnlightingLetters

#endif