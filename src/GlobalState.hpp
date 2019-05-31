/**
 * @file GlobalState.hpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2019-05-13
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef GLOBALSTATE_HPP
#define GLOBALSTATE_HPP

namespace EnlightingLetters
{

struct GlobalState
{
  enum class LightingMode : uint8_t
  {
    PULSE = 0,
    GLOWING_SNAKE = 1,
    RAIN_DROPS = 2,
    SPECTRUM = 3
  };
  enum class LightingColor : uint8_t
  {
    RANDOM = 0,
    RED = 1,
    GREEN = 2,
    YELLOW = 3,
    ORANGE = 4, 
    BLUE = 5, 
    VIOLET = 6,
    PINK = 7,
    WHITE = 8, 
    BLACK = 9
  };

  LightingMode mMode = LightingMode::PULSE;     // Currently active mode
  LightingColor mColor = LightingColor::RANDOM; // Currently active color
  int mAnimationSpeed = 2000;              // Number of milliseconds, one full animation cycle should take
  
  int16_t mSoundLevel;
  
};

}  // namespace EnlightingLetters

#endif