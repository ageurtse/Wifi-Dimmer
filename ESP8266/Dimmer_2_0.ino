/*
  Simple.ino, Example for the AutoConnect library.
  Copyright (c) 2018, Hieromon Ikasamo
  https://github.com/Hieromon/AutoConnect

  This software is released under the MIT License.
  https://opensource.org/licenses/MIT
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <AutoConnect.h>
#include <ArduinoOTA.h>
#include <ESP8266HTTPUpdateServer.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>                     // to use pin 4 and 5 as RX and TX ports


//Server stuff
ESP8266WebServer 				Server;
AutoConnect      				Portal(Server);
AutoConnectConfig 			config;
ESP8266HTTPUpdateServer UpdateServer;

#define MinDim                15
#define MaxDim                250
#define RX										14
#define TX										12

SoftwareSerial DimSerial(RX,TX,false,256); // RX, TX


//MQTT stuff
const char* password 		= "";
const char* mqtt_server = "192.168.1.100";
String hostname					= "arnold";

int led_pin 					= 2;
int relay_pin					= 16;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
long lastMsg = 0;
char msg[50];
int value = 0;


void callback(char* topic, byte* payload, unsigned int length) {
  int DimValue;
  String sendstr;

	Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

	//Convert payload to a int. value so it could be sent to the attiny by serial.
	payload[length] = '\0';
	String s = String((char*)payload);
  DimValue = String((char*)payload).toInt();
  DimValue = DimValue *2.55;
  //Serial.println(DimValue);

  if (String(topic) == "/arnold/dim/") {
  		Serial.println("Dim recieved");
      if (DimValue<MinDim) { DimValue = 0; };
      if (DimValue>MaxDim) { DimValue = 100; };
      DimValue = map(DimValue, 0, 255, 255, 0);
      DimSerial.write(DimValue);
  }
  if (String(topic) == "/arnold/switch/") {
    		Serial.println("Switch recieved");
    		if (DimValue == 0){
    			digitalWrite(led_pin, HIGH);
    			digitalWrite(relay_pin, LOW);
    		} else {
    			digitalWrite(led_pin, LOW);
    			digitalWrite(relay_pin, HIGH);
    			}
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect(hostname.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      mqttClient.publish(("/"+hostname+"/status").c_str(), "Sonoff1 booted");
      // ... and resubscribe
      mqttClient.subscribe(("/"+hostname+"/#").c_str());
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      for(int i = 0; i<5000; i++){

      }
    }
  }
}

void rootPage() {
  String  content =
    "<html>"
    "<head>"
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
    "</head>"
    "<body>"
    "<h2 align=\"center\" style=\"color:blue;margin:20px;\">Hello, world 2</h2>"
    "<p style=\"padding-top:10px;text-align:center\">" AUTOCONNECT_LINK(COG_32) "</p>"
    "</body>"
    "</html>";
  Server.send(200, "text/html", content);
}

void StartOTAServer (){
	//Starting Update Server
	     ArduinoOTA.onStart([]() {
	       String type;
	       if (ArduinoOTA.getCommand() == U_FLASH) {
	         type = "sketch";
	       } else { // U_SPIFFS
	         type = "filesystem";
	       }

	       // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
	       Serial.println("Start updating " + type);
	     });
	     ArduinoOTA.onEnd([]() {
	       Serial.println("\nEnd");
	     });
	     ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
	       Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
	     });
	     ArduinoOTA.onError([](ota_error_t error) {
	       Serial.printf("Error[%u]: ", error);
	       if (error == OTA_AUTH_ERROR) {
	         Serial.println("Auth Failed");
	       } else if (error == OTA_BEGIN_ERROR) {
	         Serial.println("Begin Failed");
	       } else if (error == OTA_CONNECT_ERROR) {
	         Serial.println("Connect Failed");
	       } else if (error == OTA_RECEIVE_ERROR) {
	         Serial.println("Receive Failed");
	       } else if (error == OTA_END_ERROR) {
	         Serial.println("End Failed");
	       }
	     });
	     ArduinoOTA.begin();

	     Serial.println("OTA server started.");
}


void setup() {
	delay(1000);
 	Serial.begin(115200);
 	DimSerial.begin(9600);

  Serial.println();

  UpdateServer.setup(&Server);
  Server.on("/", rootPage);

  //config.apid = String(AUTOCONNECT_APID)+"_"+String(ESP.getChipId(),HEX);
  config.apid = hostname;
  config.hostName = hostname;
  Portal.config(config);
  Portal.begin();

  if (Portal.begin()) {
    Serial.println("WiFi connected: " + WiFi.localIP().toString());

    //Start update server
    StartOTAServer();
    Serial.println("OTA Server started");
    //Start MQTT Client
    mqttClient.setServer(mqtt_server, 1883);
    mqttClient.setCallback(callback);

    Serial.println("MQTT Client started");
    pinMode(led_pin, OUTPUT);
    pinMode(relay_pin, OUTPUT);

    digitalWrite(led_pin, HIGH);
		digitalWrite(relay_pin, LOW);
    delay(1000);
		digitalWrite(led_pin, LOW);
		digitalWrite(relay_pin, HIGH);
  }
}

void loop() {
  Portal.handleClient();
	ArduinoOTA.handle();
	//if (DimSerial.available()) {
		//String instelwaarde = DimSerial.readstring();
//		mqttClient.publish(("/"+hostname+"/status").c_str(), DimSerial.readString().c_str());
//	}

	if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();
}

