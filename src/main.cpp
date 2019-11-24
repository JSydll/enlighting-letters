#include <Arduino.h>

#include <memory>
#include <string>

#include "CommandInterface.hpp"
#include "GlobalController.hpp"
#include "LedController.hpp"
#include "MusicAnalyzer.hpp"
#include "SerialConsole.hpp"

auto globalController = EnlightingLetters::GlobalController::Create();

void setup()
{
  using namespace EnlightingLetters;
  globalController->Init();
  globalController->console().println("#### Starting enlighting letters ####");
}

void loop()
{
  using namespace EnlightingLetters;
  // Command interface
  globalController->commandInterface->Update();
  // As the analysis is expensive, only do it when needed
  if (globalController->data.mMode == GlobalController::LightingMode::SPECTRUM)
  {
    // Get input
    globalController->musicAnalyzer->Update();
  }

  // Process and generate output
  globalController->ledController->Update();
}