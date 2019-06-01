#include "CommandInterface.hpp"

#include "LightingModes/Glow.hpp"
#include "LightingModes/Pulse.hpp"
#include "LightingModes/Rain.hpp"
#include "LightingModes/Snake.hpp"

namespace EnlightingLetters
{

std::shared_ptr<CommandInterface> CommandInterface::Create(std::shared_ptr<GlobalController> state)
{
  return std::shared_ptr<CommandInterface>(new CommandInterface(state));
}

// As strings cannot be used in switch statements by default, one possible solution is to hash them
// using constexpr. A lightweight implementation is the one proposed at
// https://hbfs.wordpress.com/2017/01/10/strings-in-c-switchcase-statements/.
// The hash function is not strong but should be sufficient for this very usecase and also hash
// collisions will be detected on compile time.
namespace
{
uint64_t constexpr mix(char m, uint64_t s) { return ((s << 7) + ~(s >> 3)) + ~m; }
uint64_t constexpr hash(const char* m) { return (*m) ? mix(*m, hash(m + 1)) : 0; }
}  // namespace

void CommandInterface::Update()
{
  while (not mCommandQueue.empty())
  {
    auto cmd = mCommandQueue.front();
    mCommandQueue.pop();
    switch (hash(cmd.c_str()))
    {
      case hash("mode:glow"):
        if (mGlobalController->data.mMode != GlobalController::LightingMode::GLOW)
        {
          mGlobalController->data.mMode = GlobalController::LightingMode::GLOW;
          mGlobalController->lightingProcessor =
              std::make_shared<Glow>(mGlobalController->ledController);
        }
        break;
      case hash("mode:pulse"):
        if (mGlobalController->data.mMode != GlobalController::LightingMode::PULSE)
        {
          mGlobalController->data.mMode = GlobalController::LightingMode::PULSE;
          mGlobalController->lightingProcessor =
              std::make_shared<Pulse>(mGlobalController, mGlobalController->ledController);
        }
        break;
      case hash("mode:snake"):
        if (mGlobalController->data.mMode != GlobalController::LightingMode::SNAKE)
        {
          mGlobalController->data.mMode = GlobalController::LightingMode::SNAKE;
          mGlobalController->lightingProcessor =
              std::make_shared<Snake>(mGlobalController->ledController);
        }
        break;
      case hash("mode:rain"):
        if (mGlobalController->data.mMode != GlobalController::LightingMode::RAIN)
        {
          mGlobalController->data.mMode = GlobalController::LightingMode::RAIN;
          mGlobalController->lightingProcessor =
              std::make_shared<Rain>(mGlobalController->ledController);
        }
        break;
      default: continue;
    }
  }
}

CommandInterface::CommandInterface(std::shared_ptr<GlobalController> state)
    : mGlobalController(state)
{
  BLEDevice::init("L&J Light Letters Control");
  mBLEServer = BLEDevice::createServer();
  mBLEServer->setCallbacks(new ServerFunctor());

  // Create the BLE Service
  auto bleService = mBLEServer->createService(kServiceUuid);
  auto rxCharacteristic =
      bleService->createCharacteristic(kRxUuid, BLECharacteristic::PROPERTY_WRITE);

  rxCharacteristic->setCallbacks(new ReceivingFunctor(mCommandQueue));
  bleService->start();
  mBLEServer->getAdvertising()->addServiceUUID(bleService->getUUID());
  mBLEServer->getAdvertising()->start();
}

void CommandInterface::ServerFunctor::onConnect(BLEServer* server)
{
  // Do something on connect
}

void CommandInterface::ServerFunctor::onDisconnect(BLEServer* server)
{
  // Give the bluetooth stack the chance to get things ready
  delay(500);
  // Restart advertising
  server->startAdvertising();
}

CommandInterface::ReceivingFunctor::ReceivingFunctor(std::queue<std::string>& rxQueue)
    : mRxQueue(rxQueue)
{}

void CommandInterface::ReceivingFunctor::onWrite(BLECharacteristic* charac)
{
  std::string rxValue = charac->getValue();

  if (rxValue.length() > 0)
  {
    mRxQueue.push(rxValue);
  }
}

}  // namespace EnlightingLetters