#include <ESP8266WiFi.h>
#define GET_CHIPID()  (ESP.getChipId())
#include <ESP8266HTTPUpdateServer.h>
#include <PubSubClient.h>
#include <AutoConnect.h>
#include <FS.h>    

// Adjusting WebServer class with between ESP8266 and ESP32.
ESP8266WebServer  webServer;
AutoConnect  portal(webServer);
AutoConnectConfig config;
//PubSubClient mqttClient(wifiClient);
ESP8266HTTPUpdateServer UpdateServer;

#include "declarations.h"
#include "helper.h"
#include "filehandeling.h"
#include "webpages.h"

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();

  //allows serving of files from SPIFFS
  Serial.println("Mounting FS...");
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }
  if (loadConfiguration("/config.json", dimconfig)) {
    Serial.println("Config read");
  }
  // Dump config file
  Serial.println(F("Print config file after reading..."));
  printFile(fileName);

  config.apid = String("ESP") + "-" + String(GET_CHIPID(), HEX);

  if (dimconfig.devicename != "") {
    config.apid = dimconfig.devicename;
    config.hostName = dimconfig.devicename;
    Serial.println(F("Set custom name"));
  }

  Serial.print("APID : ");
  Serial.println(config.apid);
  
  config.bootUri = AC_ONBOOTURI_HOME;
  config.homeUri = "/";
  
  webServer.on("/saveDim", saveSetting);  // Register /feels handler
  webServer.on("/", handleRoot);
  
  portal.load(settings);
  portal.on("/Settings",settingsOn, AC_EXIT_AHEAD);
  portal.config(config);

  Serial.print("WiFi ");
  if (portal.begin()) {
    Serial.println("connected:" + WiFi.SSID());
    Serial.println("IP:" + WiFi.localIP().toString());
  } else {
    Serial.println("connection failed:" + String(WiFi.status()));
    while (1) {
      delay(100);
      yield();
    }
  }
}


void loop() {
  portal.handleClient();

}
