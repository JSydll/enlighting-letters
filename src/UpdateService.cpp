#include "UpdateService.hpp"

#include <ESPmDNS.h>
#include <Update.h>
#include <WiFi.h>
#include <WiFiClient.h>

#include "sites/LoginHtml.hpp"
#include "sites/UploadHtml.hpp"

namespace EnlightingLetters
{
namespace
{
}

UpdateService::UpdateService(std::shared_ptr<GlobalController> ctl, const std::string& SSID,
                             const std::string& pwd)
    : mController(ctl), mServer(kServerPort), mInitialized(false)
{
  mController->console().printf("Trying to connect to network %s...\n", SSID.c_str());
  WiFi.begin(SSID.c_str(), pwd.c_str());
  for (uint8_t count = 0;; ++count)
  {
    // Abort after 2 minutes of waiting
    if (count == 120)
    {
      mController->console().println(" Connection failed.");
      return;
    }
    if (WiFi.status() == WL_CONNECTED)
    {
      break;
    }
    delay(1000);
    mController->console().print(".");
  }
  mController->console().printf("\nConnected to %s with %s.\n", SSID.c_str(),
                                WiFi.localIP().toString().c_str());
  if (!MDNS.begin(kHostName))
  {  // http://<kHostName>.local
    mController->console().println("Error setting up mDNS server.");
    return;
  }
  mController->console().printf("mDNS server started with host name %s.\n", kHostName);
  mController->console().println("Starting Update Server…");
  // Configure server callbacks
  mServer.on("/", HTTP_GET, [this]() {
    mServer.sendHeader("Connection", "close");
    mServer.send(200, "text/html", kLoginPage.c_str());
  });
  mServer.on("/upload", HTTP_GET, [this]() {
    mServer.sendHeader("Connection", "close");
    mServer.send(200, "text/html", kUploadPage.c_str());
  });
  // Actual handling of update files
  mServer.on("/update", HTTP_POST,
             [this]() {
               mServer.sendHeader("Connection", "close");
               mServer.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
               ESP.restart();
             },
             [this]() {
               HTTPUpload& upload = mServer.upload();
               if (upload.status == UPLOAD_FILE_START)
               {
                 mController->console().printf("Update: %s\n", upload.filename.c_str());
                 if (!Update.begin(UPDATE_SIZE_UNKNOWN))
                 {
                   Update.printError(mController->console());
                 }
               }
               else if (upload.status == UPLOAD_FILE_WRITE)
               {
                 /* Flashing firmware */
                 if (Update.write(upload.buf, upload.currentSize) != upload.currentSize)
                 {
                   Update.printError(mController->console());
                 }
               }
               else if (upload.status == UPLOAD_FILE_END)
               {
                 if (Update.end(true))
                 {
                   mController->console().printf("Update Success: %u\nRebooting...\n",
                                                 upload.totalSize);
                 }
                 else
                 {
                   Update.printError(mController->console());
                 }
               }
             });
  // Start the server
  mServer.begin();
}

UpdateService::~UpdateService() { mServer.stop(); }

void UpdateService::Process()
{
  if (mInitialized)
  {
    mServer.handleClient();
  }
}
}