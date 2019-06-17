/**
 * @file GlobalController.hpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2019-05-13
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef GLOBALCONTROLLER_HPP
#define GLOBALCONTROLLER_HPP

#include <memory>

#include "ILightingProcessor.hpp"
#include "SerialConsole.hpp"

namespace EnlightingLetters
{
// Forward declarations
class LedController;
class CommandInterface;
class MusicAnalyzer;

class GlobalController : public std::enable_shared_from_this<GlobalController>
{
 public:
  static std::shared_ptr<GlobalController> Create();

  void Init();
  // --- Components ---
  SerialConsole console;
  std::shared_ptr<LedController> ledController;
  std::shared_ptr<CommandInterface> commandInterface;
  std::shared_ptr<ILightingProcessor> lightingProcessor;
  std::shared_ptr<MusicAnalyzer> musicAnalyzer;

  // --- Data ---
  enum class LightingMode : uint8_t
  {
    GLOW = 0,
    PULSE = 1,
    SNAKE = 2,
    RAIN = 3,
    SPECTRUM = 4
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

  struct GlobalData
  {
    LightingMode mMode = LightingMode::PULSE;      // Currently active mode
    LightingColor mColor = LightingColor::RANDOM;  // Currently active color
    int mAnimationSpeed = 2000;  // Number of milliseconds, one full animation cycle should take
    std::vector<double> mFrequencies;
  } data;
 private:
  GlobalController() = default;
};

}  // namespace EnlightingLetters

#endif