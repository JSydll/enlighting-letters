#include "UpdateService.hpp"

#include <Update.h>
#include <WiFiClient.h>
#include "WiFi.h"

#include "Sites/LoginHtml.hpp"
#include "Sites/UploadHtml.hpp"

namespace EnlightingLetters
{
UpdateService::UpdateService(std::shared_ptr<GlobalController> ctl)
    : mController(ctl), mServer(kServerPort)
{
  mController->console().println("Starting Update Server…");
  WiFi.softAP(kWifiName.c_str(), kWifiPassword.c_str());
  mServerIP = WiFi.softAPIP();
  mController->console().print("Server address: ");
  mController->console().println(mServerIP);
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

UpdateService::~UpdateService() {}

void UpdateService::Process() { mServer.handleClient(); }
}