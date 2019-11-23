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

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <memory>
#include <queue>

#include "GlobalController.hpp"

namespace EnlightingLetters
{
/**
 * @brief
 *
 */
class CommandInterface
{
 public:
  static std::shared_ptr<CommandInterface> Create(std::shared_ptr<GlobalController> state);
  ~CommandInterface() = default;

  void Update();

 private:
  std::shared_ptr<GlobalController> mGlobalController;
  BLEServer* mBLEServer;

  CommandInterface(std::shared_ptr<GlobalController> state);

  const std::string kServiceUuid = "f77176d2-d8e8-436e-a4bd-0a7b4a4b6774";
  const std::string kRxUuid = "af283540-39bb-4421-ac09-7dd3584e94b2";

  class ServerFunctor : public BLEServerCallbacks
  {
   private:
    void onConnect(BLEServer* server);
    void onDisconnect(BLEServer* server);
  };

  class ReceivingFunctor : public BLECharacteristicCallbacks
  {
   public:
    ReceivingFunctor(std::queue<std::string>& rxQueue);

   private:
    std::queue<std::string>& mRxQueue;
    void onWrite(BLECharacteristic* charac);
  };

  std::queue<std::string> mCommandQueue;

  /**
   * 
   */
  void SetVisualization(GlobalController::LightingMode desiredMode);
};

}  // namespace EnlightingLetters

#endif
