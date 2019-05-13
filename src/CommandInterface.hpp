/**
 * @file CommandInterface.hpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2019-05-13
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef COMMANDINTERFACE_HPP
#define COMMANDINTERFACE_HPP

#include <string>

namespace EnlightingLetters
{
/**
 * @brief
 *
 */
class CommandInterface
{
 public:
  CommandInterface() = default;
  ~CommandInterface() = default;

  bool HasInput();

  std::string GetInput();
};

}  // namespace EnlightingLetters

#endif
