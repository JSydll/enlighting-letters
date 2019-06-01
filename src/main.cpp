#include <Arduino.h>

#include <memory>
#include <string>

#include "CommandInterface.hpp"
#include "GlobalController.hpp"
#include "LightingModes/Glow.hpp"
#include "LightingModes/Pulse.hpp"
#include "LightingModes/Rain.hpp"
#include "LightingModes/Snake.hpp"
//#include "MusicAnalyzer.hpp"
#include "SerialConsole.hpp"

auto globalController = EnlightingLetters::GlobalController::Create();

void setup()
{
  using namespace EnlightingLetters;
  globalController->Init();

  globalController->console().println("#### Starting enlighting letters ####");
  delay(2000);
  /*CRGBPalette16 palette = RainbowColors_p;
  palette[0] = CRGB::Black;
  palette[1] = CRGB::Black;
  GlobalController->mLightingProcessor = std::make_shared<Snake>(ledController, palette);*/
  // globalController->lightingProcessor = std::make_shared<Rain>(ledController);
}

void loop()
{
  using namespace EnlightingLetters;
  // Command interface
  globalController->commandInterface->Update();
  // Get input
  // musicAnalyzer.UpdateState();

  // Process and generate output
  globalController->ledController->Update();

  // serialConsole().println(GlobalController->mSoundLevel);
}