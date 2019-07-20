/**
 * @file Spectrum.hpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2019-05-13
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef SPECTRUM_HPP
#define SPECTRUM_HPP

#include <array>
#include <chrono>
#include <memory>
#include <vector>

#include "../GlobalController.hpp"
#include "../ILightingProcessor.hpp"
#include "../LedController.hpp"

namespace EnlightingLetters
{
const TProgmemRGBPalette16 MusicSpectrum_p FL_PROGMEM = {
    CRGB::LimeGreen, CRGB::LimeGreen, CRGB::LimeGreen,   CRGB::LimeGreen,
    CRGB::LimeGreen, CRGB::LimeGreen, CRGB::GreenYellow, CRGB::Yellow,
    CRGB::Yellow,    CRGB::Orange,    CRGB::Orange,      CRGB::Orange,
    CRGB::OrangeRed, CRGB::Red,       CRGB::Red,         CRGB::Red};

/**
 * @brief
 *
 */
class Spectrum final : public ILightingProcessor
{
 public:
  Spectrum(std::shared_ptr<GlobalController> controller);
  Spectrum(std::shared_ptr<GlobalController> controller, CRGBPalette16 palette);
  virtual ~Spectrum() = default;

  virtual void PerformUpdate() override;

  virtual int GetStepsPerAnimation() override;

 private:
  std::shared_ptr<GlobalController> mController;

  void Fill(const CRGBPalette16& palette);

  const std::vector<std::pair<int, int>> kFloor = {{0, 5}, {251, 330}};

  struct SegmentControl
  {
    std::pair<int, int> left;
    std::pair<int, int> right;
    int height;
  };

  // Segments from left to right
  const std::vector<SegmentControl> kSegments = {
      {{5, 40}, {76, 41}, 35},     {{77, 81}, {86, 82}, 4},      {{87, 91}, {97, 92}, 5},
      {{98, 104}, {109, 104}, 5},  {{110, 128}, {147, 129}, 18}, {{148, 155}, {163, 156}, 7},
      {{164, 171}, {179, 172}, 7}, {{180, 214}, {250, 215}, 35}};

  // Frequency bands:
  // [0] 150Hz - [1] 400Hz [2] 1kHz - [3] 2kHz
  // [4] 4kHz - [5] 8kHz - [6] 16kz - [7] 20kHz
  const std::vector<uint8_t> kBandToSegmentMapping = {3, 0, 2, 5, 6, 7, 4, 1};
};  // namespace EnlightingLetters
}  // namespace EnlightingLetters

#endif