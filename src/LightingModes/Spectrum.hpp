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
    CRGB(0, 90, 50),     CRGB(35, 132, 67),  CRGB(35, 132, 67),   CRGB(35, 132, 67),
    CRGB(65, 171, 93),   CRGB(65, 171, 93),  CRGB(153, 219, 184), CRGB(102, 204, 206),
    CRGB(153, 219, 184), CRGB(204, 230, 75), CRGB(252, 199, 7),   CRGB(248, 157, 14),
    CRGB(248, 157, 14),  CRGB(245, 114, 21), CRGB(241, 71, 28),   CRGB(219, 30, 38)};

/**
 * @brief
 *
 */
class Spectrum final : public ILightingProcessor
{
 public:
  Spectrum(std::shared_ptr<GlobalController>& controller);
  Spectrum(std::shared_ptr<GlobalController>& controller, CRGBPalette16 palette);
  virtual ~Spectrum() = default;

  virtual void PerformUpdate() override;

  virtual int GetStepsPerAnimation() override;

 private:
  std::shared_ptr<GlobalController> mController;

  void Fill(const CRGBPalette16& palette);

  const std::vector<std::pair<int, int>> kFloor = {{0, 5}, {253, 330}};

  struct SegmentControl
  {
    std::pair<int, int> left;
    std::pair<int, int> right;
    int height;
  };

  const std::vector<SegmentControl> kSegments = {
      {{6, 41}, {77, 42}, 36},     {{78, 80}, {84, 81}, 4},      {{85, 87}, {91, 88}, 4},
      {{92, 97}, {103, 98}, 6},    {{104, 121}, {140, 122}, 19}, {{141, 150}, {161, 151}, 10},
      {{162, 168}, {176, 169}, 9}, {{177, 214}, {252, 215}, 38}};

  // To show the frequency values, the maximum values need to be know. The initialization is made as
  // best guess
  std::vector<int> mMaxFrequencies = {200, 200, 200, 200, 200, 200, 200, 200};
};  // namespace EnlightingLetters
}  // namespace EnlightingLetters

#endif