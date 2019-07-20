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
    CHASER = 3,
    RAIN = 4,
    SPECTRUM = 5
  };

  struct GlobalData
  {
    LightingMode mMode = LightingMode::PULSE;  // Active mode
    int mColor = 0xFFFFFF;                     // Active color, black being the respective default
    int mAnimationSpeed = 2000;  // Number of milliseconds, one full animation cycle should take
    bool mMusicActive = false;
    std::vector<long> mFrequencies;
  } data;

 private:
  GlobalController() = default;
};

}  // namespace EnlightingLetters

#endif