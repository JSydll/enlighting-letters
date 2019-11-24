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
  UpdateService(std::shared_ptr<GlobalController> ctl, const std::string& SSID,
                const std::string& pwd);
  ~UpdateService();

  static constexpr const char* kDefaultWifiSSID = "WILLY.TEL-J5WHR275";
  static constexpr const char* kDefaultPassword = "ohanaheisstfamilie8818";

  bool IsOk() { return mInitialized; }

  void Process();

 private:
  std::shared_ptr<GlobalController> mController;
  bool mInitialized;
  WebServer mServer;

  const int kServerPort = 80;
  const char* kHostName = "LJupdate";
};

}  // namespace EnlightingLetters

#endif
