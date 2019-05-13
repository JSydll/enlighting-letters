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

#include "GlobalState.hpp"

namespace EnlightingLetters
{
/**
 * @brief
 *
 */
class MusicAnalyzer
{
 public:
  MusicAnalyzer(std::shared_ptr<GlobalState> state);
  ~MusicAnalyzer() = default;

  void UpdateState();

 private:
  std::shared_ptr<GlobalState> mState;
};
}  // namespace EnlightingLetters

#endif