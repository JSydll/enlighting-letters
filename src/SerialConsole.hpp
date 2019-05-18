/**
 * @file SerialConsole.hpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2019-05-13
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef SERIALCONSOLE_HPP
#define SERIALCONSOLE_HPP

#include <Arduino.h>
#include <string>

namespace EnlightingLetters
{

class SerialConsole
{
 public:
  SerialConsole(int baudRate = 115200) { Serial.begin(baudRate); }
  ~SerialConsole() { Serial.flush(); };

  HardwareSerial& Get() { return Serial; }
};
}  // namespace EnlightingLetters

#endif  // SERIALCONSOLE_HPP