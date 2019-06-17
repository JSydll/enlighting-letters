/**
 * @file MusicAnalyzer.hpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2019-05-13
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef MUSICANALYZER_HPP
#define MUSICANALYZER_HPP

#include <memory>
#include "arduinoFFT.h"

#include "GlobalController.hpp"

namespace EnlightingLetters
{
/**
 * @brief
 *
 */
class MusicAnalyzer
{
 public:
  MusicAnalyzer(std::shared_ptr<GlobalController> globalController);
  ~MusicAnalyzer();

  void Update();

 private:
  std::shared_ptr<GlobalController> mGlobalController;
  arduinoFFT mFFT;
  bool mOk;

  const int kInputPin = 36;
  static const uint16_t kSampleSize = 256;
  const uint16_t kSampleRate = 40000;
  const int kMicrosBetweenSamples = 1000000 / kSampleRate;
  const uint8_t kFrequencyBands = 8;
  // According to Nyquist-Theorem, only half of the samples represents the freq. from 0 to
  // kSampleRate/2 Hz.
  const int kFrequenciesPerBand = kSampleSize / 2 / kFrequencyBands;

  const double kRationalThreshold = 6000.0;
  double CalculateAmplitude(std::array<double, 0>::iterator first, int startIndex, int numSamples);
};

}  // namespace EnlightingLetters

#endif