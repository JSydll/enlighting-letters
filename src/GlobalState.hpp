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
    enum class LightingMode : short
    {
      PULSE = 0,
      ROTATE = 1,
      RAIN_DROPS = 2,
      SPECTRUM = 3
    };

    LightingMode mMode;
};

}

#endif