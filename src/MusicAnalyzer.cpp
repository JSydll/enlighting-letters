#include "MusicAnalyzer.hpp"

#include <chrono>
#include <numeric>
#include <thread>

namespace EnlightingLetters
{

MusicAnalyzer::MusicAnalyzer(std::shared_ptr<GlobalController> globalController)
    : mGlobalController(globalController)
{
  mGlobalController->data.mFrequencies.resize(kFrequencyBands);
  mOk = true;
}

MusicAnalyzer::~MusicAnalyzer() {}

long MusicAnalyzer::CalculateAmplitude(std::array<double, 0>::iterator first, int startIndex,
                                       int numSamples, long lastValue, long offset)
{
  long val = (numSamples == 1 ? static_cast<long>(*(first + startIndex))
                              : (static_cast<long>(std::accumulate(
                                     first + startIndex, first + startIndex + numSamples, 0.0)) /
                                 numSamples)) - offset;
  // Employ TP filter with beta = 2
  return (val + (lastValue << 2) - lastValue) >> 2;
}

void MusicAnalyzer::Update()
{
  using namespace std::chrono;
  static std::array<double, kSampleSize> real;
  static std::array<double, kSampleSize> imag;

  // Get a bunch of new samples
  static int num = 0;
  for (num = 0; num < kSampleSize; ++num)
  {
    real[num] = analogRead(kInputPin);  // Conversion is said to take approx. 1us
    imag[num] = 0.0;
    std::this_thread::sleep_for(microseconds(kMicrosBetweenSamples - 1));
  }

  // Calculate FFT on them
  mFFT.Windowing(real.data(), kSampleSize, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  mFFT.Compute(real.data(), imag.data(), kSampleSize, FFT_FORWARD);
  mFFT.ComplexToMagnitude(real.data(), imag.data(), kSampleSize);
  // Update data in global controller
  // Aggregate the frequencies (discarding the first value [0Hz])
  // 150Hz [1] - 400Hz [2-4] - 1kHz [5-8] - 2kHz [9-15]
  // 4kHz [16-35] - 8kHz [36-79] - 16kz [80-115] - 20kHz [116-127]
  static const std::vector<long> kOffsets = {81500, 150, 60, 40, 40, 40, 40, 10};
  static const std::vector<std::pair<int, int>> mFreqRanges = {
      {1, 1}, {2, 3}, {5, 4}, {9, 7}, {16, 20}, {36, 44}, {80, 36}, {116, 12}};
  for (num = 0; num < mGlobalController->data.mFrequencies.size(); ++num)
  {
    mGlobalController->data.mFrequencies[num] =
        CalculateAmplitude(real.begin(), mFreqRanges[num].first, mFreqRanges[num].second,
                           mGlobalController->data.mFrequencies[num], kOffsets[num]);
  }
}
}  // namespace EnlightingLetters