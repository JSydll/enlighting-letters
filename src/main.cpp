#include <Arduino.h>

#include <memory>
#include <string>

//#include "CommandInterface.hpp"
#include "GlobalState.hpp"
#include "LightingPulse.hpp"
//#include "MusicAnalyzer.hpp"
#include "SerialConsole.hpp"

auto globalState = std::make_shared<EnlightingLetters::GlobalState>();

auto serialConsole = EnlightingLetters::SerialConsole();
//auto commandInterface = EnlightingLetters::CommandInterface();
//auto musicAnalyzer = EnlightingLetters::MusicAnalyzer(globalState);

std::shared_ptr<EnlightingLetters::LedController> ledController;
std::unique_ptr<EnlightingLetters::ILightingMode> activeMode;

void setup()
{
  using namespace EnlightingLetters;
  serialConsole().println("#### Starting enlighting letters ####");
  serialConsole().println("Default global state: ");
  serialConsole().print("\tLighting mode: ");
  serialConsole().println(static_cast<int>(globalState->mMode));
  serialConsole().print("\tLighting color: ");
  serialConsole().println(static_cast<int>(globalState->mColor));
  serialConsole().print("\tAnimation speed: ");
  serialConsole().println(globalState->mAnimationSpeed);
  delay(2000);
  ledController = std::make_shared<LedController>();
  activeMode = std::make_unique<LightingPulse>(globalState, ledController);
}

void loop()
{
  // Command interface
  /*if (commandInterface.HasInput())
  {
    //
  }*/
  // Get input
  //musicAnalyzer.UpdateState();

  // Process and generate output
  activeMode->Next();

  //serialConsole().println(globalState->mSoundLevel);
}