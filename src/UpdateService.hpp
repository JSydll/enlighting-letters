/**
 * @file UpdateService.hpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2019-05-13
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef UPDATESERVICE_HPP
#define UPDATESERVICE_HPP

#include <Arduino.h>
#include <WebServer.h>

#include <memory>

#include "GlobalController.hpp"

namespace EnlightingLetters
{
/**
 * @brief
 *
 */
class UpdateService
{
 public:
  UpdateService(std::shared_ptr<GlobalController> ctl);
  ~UpdateService();

  void Process();

 private:
  std::shared_ptr<GlobalController> mController;
  WebServer mServer;
  IPAddress mServerIP;

  

  const std::string kWifiName = "LJ Letters";
  const std::string kWifiPassword = "updateserver";
  const int kServerPort = 80;
};

}  // namespace EnlightingLetters

#endif
