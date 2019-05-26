#include "MusicAnalyzer.hpp"

#include <chrono>
#include "arduinoFFT/src/arduinoFFT.h"

namespace EnlightingLetters
{

MusicAnalyzer::MusicAnalyzer(std::shared_ptr<GlobalState> state) : mState(state)
{
  auto err = i2s_driver_install(kI2CPort, &kI2CConfig, 0, NULL);
  if (err != ESP_OK)
  {
    mOk = false;
    return;
  }
  err = i2s_set_pin(kI2CPort, &kPinConfig);
  if (err != ESP_OK)
  {
    mOk = false;
    return;
  }
  mOk = true;
}

MusicAnalyzer::~MusicAnalyzer() { i2s_driver_uninstall(kI2CPort); }

void MusicAnalyzer::UpdateState()
{
  // Read a new sample
  int32_t sample = 0;
  size_t bytes_read = 0;
  i2s_read(kI2CPort, &sample, sizeof(sample), &bytes_read, portMAX_DELAY);
  if(bytes_read == sizeof(sample))
  {
    mState->mSoundLevel = static_cast<int16_t>(sample>>16);
  }
  else
  {
    mState->mSoundLevel = 0;
  } 
}
}  // namespace EnlightingLetters