#include <Arduino.h>

#include <memory>
#include <string>

//#include "CommandInterface.hpp"
#include "GlobalState.hpp"
#include "LightingModes/Pulse.hpp"
#include "LightingModes/Snake.hpp"
#include "LightingModes/Rain.hpp"
#include "LightingModes/Glow.hpp"
//#include "MusicAnalyzer.hpp"
#include "SerialConsole.hpp"

auto globalState = std::make_shared<EnlightingLetters::GlobalState>();

auto serialConsole = EnlightingLetters::SerialConsole();
// auto commandInterface = EnlightingLetters::CommandInterface();
// auto musicAnalyzer = EnlightingLetters::MusicAnalyzer(globalState);
std::shared_ptr<EnlightingLetters::LedController> ledController;

void setup()
{
  using namespace EnlightingLetters;
  serialConsole().println("#### Starting enlighting letters ####");
  delay(2000);
  ledController = LedController::Create(globalState);
  // ledController->SetProcessor(std::make_shared<Pulse>(globalState, ledController));
  /*CRGBPalette16 palette = RainbowColors_p;
  palette[0] = CRGB::Black;
  palette[1] = CRGB::Black;
  auto proc = std::make_shared<Snake>(ledController, palette);
  ledController->SetProcessor(proc);*/
  ledController->SetProcessor(std::make_shared<Rain>(ledController));
}

void loop()
{
  using namespace EnlightingLetters;
  // Command interface
  /*if (commandInterface.HasInput())
  {
    //
  }*/
  // Get input
  // musicAnalyzer.UpdateState();

  // Process and generate output
  ledController->Update();

  // serialConsole().println(globalState->mSoundLevel);
}