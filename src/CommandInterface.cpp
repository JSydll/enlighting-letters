#include "CommandInterface.hpp"

#include <cstdlib>

#include "LightingModes/SolidFill.hpp"
#include "LightingModes/Fire.hpp"
#include "LightingModes/Chaser.hpp"
#include "LightingModes/Glow.hpp"
#include "LightingModes/Pulse.hpp"
#include "LightingModes/Rain.hpp"
#include "LightingModes/Snake.hpp"
#include "LightingModes/Spectrum.hpp"

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

struct KeyValue
{
  std::string key;
  std::string val;
};
KeyValue ExtractKV(const std::string &cmd)
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
    auto kv = ExtractKV(cmd);
    switch (hash(kv.key.c_str()))
    {
      case hash("mode"):
      {
        mGlobalController->data.mMusicActive = (kv.val == "music");
        // Some lighting modes need to be restarted to work properly with the new mode
        if (mGlobalController->data.mMode == GlobalController::LightingMode::PULSE)
        {
          mGlobalController->lightingProcessor =
              std::make_shared<Pulse>(mGlobalController, mGlobalController->ledController);
        }
      }
      break;
      case hash("visu"):
        switch (hash(kv.val.c_str()))
        {
          case hash("solid"):
            SetVisualization(GlobalController::LightingMode::SOLID);
            break;
          case hash("fire"):
            SetVisualization(GlobalController::LightingMode::FIRE);
            break;
          case hash("spectrum"):
            SetVisualization(GlobalController::LightingMode::SPECTRUM);
            break;
          case hash("glow"):
            SetVisualization(GlobalController::LightingMode::GLOW);
            break;
          case hash("pulse"):
            SetVisualization(GlobalController::LightingMode::PULSE);
            break;
          case hash("snake"):
            SetVisualization(GlobalController::LightingMode::SNAKE);
            break;
          case hash("chaser"):
            SetVisualization(GlobalController::LightingMode::CHASER);
            break;
          case hash("rain"):
            SetVisualization(GlobalController::LightingMode::RAIN);
            break;
          default: break;
        }
        break;
      case hash("color"):
        if (kv.val == "default")
        {
          mGlobalController->data.mColor = 0x000000;
        }
        else
        {
          mGlobalController->data.mColor = ExtractColor(kv.val);
          // Some lighting modes need to be restarted to respect new color
          if (mGlobalController->data.mMode == GlobalController::LightingMode::CHASER)
          {
            CRGBPalette16 singleFillPalette(CRGB(mGlobalController->data.mColor));
            mGlobalController->lightingProcessor = std::make_shared<Chaser>(
                mGlobalController, mGlobalController->ledController, singleFillPalette);
          }
        }
        break;
      case hash("bright"):
      {
        mGlobalController->ledController->SetTotalBrightness(ExtractBrightness(kv.val));
        // Some lighting modes need to be restarted to work properly with the new brightness
        if (mGlobalController->data.mMode == GlobalController::LightingMode::PULSE)
        {
          mGlobalController->lightingProcessor =
              std::make_shared<Pulse>(mGlobalController, mGlobalController->ledController);
        }
      }
      break;
      default: continue;
    }
  }
}

void CommandInterface::SetVisualization(GlobalController::LightingMode desiredMode)
{
  if(mGlobalController->data.mMode != desiredMode)
  {
    mGlobalController->data.mMode = desiredMode;
    using m = GlobalController::LightingMode;
    switch(desiredMode)
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
      default:
        break;
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