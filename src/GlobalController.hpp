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
class UpdateService;

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
  std::unique_ptr<UpdateService> updateService;

  // --- Data ---
  enum class LightingMode : uint8_t
  {
    SOLID = 0,
    FIRE = 1,
    GLOW = 2,
    PULSE = 3,
    SNAKE = 4,
    CHASER = 5,
    RAIN = 6,
    SPECTRUM = 7
  };

  struct GlobalData
  {
    LightingMode mode = LightingMode::SOLID;  // Active mode
    int color = 0xFFC850;                     // Active color, warm white being the respective default (255, 200, 80)
    int animationSpeed = 1600;  // Number of milliseconds one full animation cycle should take
    std::vector<long> frequencies;
    // Must not be 0!
    std::vector<long> maxFrequencies = {1, 1, 1, 1, 1, 1, 1, 1};
    bool updateActive = false;
  } data;

 private:
  GlobalController() = default;
};

}  // namespace EnlightingLetters

#endif