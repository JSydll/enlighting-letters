#include <Arduino.h>

#include <memory>

#include "CommandInterface.hpp"
#include "GlobalState.hpp"
#include "LightingPulse.hpp"

std::shared_ptr<EnlightingLetters::GlobalState> globalState;
std::unique_ptr<EnlightingLetters::ILightingMode> activeMode;
std::unique_ptr<EnlightingLetters::CommandInterface> commandInterface;

void setup()
{
  using namespace EnlightingLetters;
  activeMode = std::make_unique<LightingPulse>(globalState);
}

void loop()
{
  // Cammand interface
  if (commandInterface->HasInput())
  {
    //
  }
  // FTT

  // Process and generate output
  activeMode->Next();
}