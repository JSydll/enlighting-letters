#include <Arduino.h>

#include <memory>

#include "CommandInterface.hpp"
#include "GlobalState.hpp"
#include "LightingPulse.hpp"
#include "MusicAnalyzer.hpp"
#include "SerialConsole.hpp"

auto globalState = std::make_shared<EnlightingLetters::GlobalState>();

auto serialConsole = EnlightingLetters::SerialConsole();
auto commandInterface = EnlightingLetters::CommandInterface();
auto musicAnalyzer = EnlightingLetters::MusicAnalyzer(globalState);

std::unique_ptr<EnlightingLetters::ILightingMode> activeMode;

void setup()
{
  using namespace EnlightingLetters;
  serialConsole.Get().print("#### Starting enlighting letters ####\n");

  activeMode = std::make_unique<LightingPulse>(globalState);
}

void loop()
{
  // Command interface
  if (commandInterface.HasInput())
  {
    //
  }
  // Get input
  musicAnalyzer.UpdateState();

  // Process and generate output
  activeMode->Next();

  serialConsole.Get().println(globalState->mSoundLevel);
}