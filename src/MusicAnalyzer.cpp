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

double MusicAnalyzer::CalculateAmplitude(std::array<double, 0>::iterator first, int startIndex,
                                         int numSamples)
{
  double val = (numSamples == 1
                    ? *(first + startIndex)
                    : (std::accumulate(first + startIndex, first + startIndex + numSamples, 0.0) /
                       static_cast<double>(numSamples)));
  return (val > kRationalThreshold ? val - kRationalThreshold : val);
}

void MusicAnalyzer::Update()
{
  using namespace std::chrono;
  static std::array<double, kSampleSize> real;
  static std::array<double, kSampleSize> imag;

  // Get a bunch of new samples
  static int sampleNum = 0;
  for (sampleNum = 0; sampleNum < kSampleSize; ++sampleNum)
  {
    real[sampleNum] = analogRead(kInputPin);  // Conversion is said to take approx. 1us
    imag[sampleNum] = 0.0;
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
  mGlobalController->data.mFrequencies[0] = CalculateAmplitude(real.begin(), 1, 1);
  mGlobalController->data.mFrequencies[1] = CalculateAmplitude(real.begin(), 2, 3);
  mGlobalController->data.mFrequencies[2] = CalculateAmplitude(real.begin(), 5, 4);
  mGlobalController->data.mFrequencies[3] = CalculateAmplitude(real.begin(), 9, 7);
  mGlobalController->data.mFrequencies[4] = CalculateAmplitude(real.begin(), 16, 20);
  mGlobalController->data.mFrequencies[5] = CalculateAmplitude(real.begin(), 36, 44);
  mGlobalController->data.mFrequencies[6] = CalculateAmplitude(real.begin(), 80, 36);
  mGlobalController->data.mFrequencies[7] = CalculateAmplitude(real.begin(), 116, 12);
}
}  // namespace EnlightingLetters