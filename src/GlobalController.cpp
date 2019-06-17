#include "GlobalController.hpp"
#include "LedController.hpp"
#include "CommandInterface.hpp"
// Default lighting mode (rest is in command interface)
#include "LightingModes/Glow.hpp"
#include "MusicAnalyzer.hpp"

namespace EnlightingLetters
{
std::shared_ptr<GlobalController> GlobalController::Create()
{
  return std::shared_ptr<GlobalController>(new GlobalController());
}
void GlobalController::Init()
{
  ledController = LedController::Create(shared_from_this());
  commandInterface = CommandInterface::Create(shared_from_this());
  // Default the lightingProcessor to PULSE
  lightingProcessor = std::make_shared<Glow>(ledController);
  musicAnalyzer = std::make_shared<MusicAnalyzer>(shared_from_this());
}
}