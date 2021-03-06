#include "CommandInterface.hpp"

#include <cstdlib>
#include <memory>

#include "UpdateService.hpp"
#include "lighting-modes/Chaser.hpp"
#include "lighting-modes/Fire.hpp"
#include "lighting-modes/Glow.hpp"
#include "lighting-modes/Pulse.hpp"
#include "lighting-modes/Rain.hpp"
#include "lighting-modes/Snake.hpp"
#include "lighting-modes/SolidFill.hpp"
#include "lighting-modes/Spectrum.hpp"

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

using Mode_t = GlobalController::LightingMode;

struct KeyValue
{
  std::string key;
  std::string val;
};
KeyValue ExtractKV(const std::string& cmd)
{
  auto splitPos = cmd.find(":");
  return {cmd.substr(0, splitPos), cmd.substr(splitPos + 1, cmd.size())};
}

int ExtractColor(std::string& value)
{
  value.erase(0, 1);
  return static_cast<int>(strtol(value.c_str(), NULL, 16));
}

int ExtractBrightness(std::string& value)
{
  return static_cast<int>(strtol(value.c_str(), NULL, 10));
}
}  // namespace

void CommandInterface::Update()
{
  while (not mCommandQueue.empty())
  {
    auto cmd = mCommandQueue.front();
    mCommandQueue.pop();
    mGlobalController->console().printf("BT cmd: <%s>\n", cmd.c_str());
    auto kv = ExtractKV(cmd);
    switch (hash(kv.key.c_str()))
    {
      case hash("visu"):
        switch (hash(kv.val.c_str()))
        {
          case hash("solid"): SetVisualization(Mode_t::SOLID); break;
          case hash("fire"): SetVisualization(Mode_t::FIRE); break;
          case hash("spectrum"): SetVisualization(Mode_t::SPECTRUM); break;
          case hash("glow"): SetVisualization(Mode_t::GLOW); break;
          case hash("pulse"): SetVisualization(Mode_t::PULSE); break;
          case hash("snake"): SetVisualization(Mode_t::SNAKE); break;
          case hash("chaser"): SetVisualization(Mode_t::CHASER); break;
          case hash("rain"): SetVisualization(Mode_t::RAIN); break;
          default: break;
        }
        break;
      case hash("color"):
        if (kv.val == std::string("default"))
        {
          mGlobalController->data.color = 0x000000;
        }
        else
        {
          mGlobalController->data.color = ExtractColor(kv.val);
          // Some lighting modes need to be restarted to respect new color
          if (mGlobalController->data.mode == Mode_t::CHASER)
          {
            CRGBPalette16 singleFillPalette(CRGB(mGlobalController->data.color));
            mGlobalController->lightingProcessor = std::make_shared<Chaser>(
                mGlobalController, mGlobalController->ledController, singleFillPalette);
          }
        }
        break;
      case hash("bright"):
      {
        mGlobalController->ledController->SetTotalBrightness(ExtractBrightness(kv.val));
        // Some lighting modes need to be restarted to work properly with the new brightness
        if (mGlobalController->data.mode == Mode_t::PULSE)
        {
          mGlobalController->lightingProcessor =
              std::make_shared<Pulse>(mGlobalController, mGlobalController->ledController);
        }
      }
      break;
      case hash("wifi"):
      {
        KeyValue network{UpdateService::kDefaultWifiSSID, UpdateService::kDefaultPassword};
        // Another configuration than the default one currently not supported
        mGlobalController->updateService = std::unique_ptr<UpdateService>(
            new UpdateService(mGlobalController, network.key, network.val));
        if (not mGlobalController->updateService->IsOk())
        {
          mGlobalController->updateService.release(); 
          break;
        }
        mGlobalController->data.updateActive = true;
      }
      break;
      default: continue;
    }
  }
}

void CommandInterface::SetVisualization(Mode_t desiredMode)
{
  if (mGlobalController->data.mode != desiredMode)
  {
    mGlobalController->data.mode = desiredMode;
    using m = Mode_t;
    switch (desiredMode)
    {
      case m::SOLID:
        mGlobalController->lightingProcessor = std::make_shared<SolidFill>(mGlobalController);
        break;
      case m::FIRE:
        mGlobalController->lightingProcessor = std::make_shared<Fire>(mGlobalController);
        break;
      case m::GLOW:
        mGlobalController->lightingProcessor =
            std::make_shared<Glow>(mGlobalController->ledController);
        break;
      case m::PULSE:
        mGlobalController->lightingProcessor =
            std::make_shared<Pulse>(mGlobalController, mGlobalController->ledController);
        break;
      case m::SNAKE:
        mGlobalController->lightingProcessor =
            std::make_shared<Snake>(mGlobalController->ledController);
        break;
      case m::CHASER:
        mGlobalController->lightingProcessor =
            std::make_shared<Chaser>(mGlobalController, mGlobalController->ledController);
        break;
      case m::RAIN:
        mGlobalController->lightingProcessor =
            std::make_shared<Rain>(mGlobalController->ledController);
        break;
      case m::SPECTRUM:
        mGlobalController->lightingProcessor = std::make_shared<Spectrum>(mGlobalController);
      default: break;
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
{
}

void CommandInterface::ReceivingFunctor::onWrite(BLECharacteristic* charac)
{
  std::string rxValue = charac->getValue();

  if (rxValue.length() > 0)
  {
    mRxQueue.push(rxValue);
  }
}

}  // namespace EnlightingLetters