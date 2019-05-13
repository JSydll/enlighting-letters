#include "MusicAnalyzer.hpp"

#include "arduinoFFT/src/arduinoFFT.h"

namespace EnlightingLetters
{

MusicAnalyzer::MusicAnalyzer(std::shared_ptr<GlobalState> state) : mState(state) {}

void MusicAnalyzer::UpdateState() {}
}  // namespace EnlightingLetters