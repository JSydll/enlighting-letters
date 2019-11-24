#include "GlobalController.hpp"
#include "CommandInterface.hpp"
#include "LedController.hpp"
// Default lighting mode (rest is in command interface)
#include "LightingModes/SolidFill.hpp"
#include "MusicAnalyzer.hpp"
#include "UpdateService.hpp"

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
  // Default the lightingProcessor to SolidFill
  lightingProcessor = std::make_shared<SolidFill>(shared_from_this());
  musicAnalyzer = std::make_shared<MusicAnalyzer>(shared_from_this());
  updateService = std::make_shared<UpdateService>(shared_from_this());
}
}