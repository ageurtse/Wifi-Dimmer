#include <ESP8266WiFi.h>
#define GET_CHIPID()  (ESP.getChipId())
#include <ESP8266HTTPUpdateServer.h>
#include <PubSubClient.h>
#include <AutoConnect.h>
#include <ESP8266HTTPUpdateServer.h>
#include <FS.h>    
#include <SoftwareSerial.h>

#define RX                    14
#define TX                    12
SoftwareSerial DimSerial(RX,TX,false,256); // RX, TX

// ESP8266 webserver
ESP8266WebServer  webServer;
//Autoconnect wifi manager
AutoConnect  portal(webServer);
AutoConnectAux update("/update", "Update");
AutoConnectConfig config;
// MQTT Stuf
WiFiClient   wifiClient;
PubSubClient mqttClient(wifiClient);
// updating
ESP8266HTTPUpdateServer UpdateServer;

#include "declarations.h"
#include "Dimmath.h"
#include "filehandeling.h"
#include "webpages.h"
#include "mqtt.h"

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();

  pinMode(led_pin, OUTPUT);
  pinMode(relay_pin, OUTPUT);

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

  config.apid = String("ESP") + String(GET_CHIPID(), HEX);

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
  portal.join ({update});
  portal.config(config);

  UpdateServer.setup(&webServer);

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

  if (dimconfig.mqttServer != "") {
    mqttClient.setServer(dimconfig.mqttServer.c_str(), 1883);
    mqttClient.setCallback(callback);  
  }

  digitalWrite(led_pin, HIGH);
  digitalWrite(relay_pin, LOW);
  delay(1000);
  digitalWrite(led_pin, LOW);
  digitalWrite(relay_pin, HIGH);
}


void loop() {
  portal.handleClient();

  if (dimconfig.mqttServer != "") {
    if (!mqttClient.connected()) {
      reconnect();
    }
    mqttClient.loop();
  }
}
