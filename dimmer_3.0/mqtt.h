int led_pin           = 2;
int relay_pin         = 16;

void callback(char* topic, byte* payload, unsigned int length) {
  int DimValue;
  String sendstr;

  //Convert payload to a int. value so it could be sent to the attiny by serial.
  payload[length] = '\0';
  String s = String((char*)payload);
  Serial.println("Topic");
  DimValue = String((char*)payload).toInt();
  int OldDimValue = DimValue;

  //below when a dim topic recived
  if (String(topic) == dimconfig.devicename+"/dim/set") {
  Serial.println("Dim set");
        if (DimValue == 0){
        digitalWrite(led_pin, HIGH);
        digitalWrite(relay_pin, LOW);
        mqttClient.publish((dimconfig.devicename+"/switch/status").c_str(), "0");
      } else {
        digitalWrite(led_pin, LOW);
        digitalWrite(relay_pin, HIGH);
        mqttClient.publish((dimconfig.devicename+"/switch/status").c_str(), "1");
        }
      DimValue=fscale(0,100,dimconfig.maxDimValue,dimconfig.minDimValue,DimValue, dimconfig.dimCurve);
      mqttClient.publish((dimconfig.devicename+"/dim/status").c_str(), String(OldDimValue,DEC).c_str());
      DimSerial.write(DimValue);
  }

  Serial.println(String(topic));
  Serial.println((dimconfig.devicename+"/switch/set"));
  // Below when a switch topic arrived
  if (String(topic) == dimconfig.devicename+"/switch/set") {
  Serial.println("Switch set");
    if (DimValue == 0){
      digitalWrite(led_pin, HIGH);
      digitalWrite(relay_pin, LOW);
      mqttClient.publish((dimconfig.devicename+"/switch/status").c_str(), "0");
    } else {
      digitalWrite(led_pin, LOW);
      digitalWrite(relay_pin, HIGH);
      mqttClient.publish((dimconfig.devicename+"/switch/status").c_str(), "1");
    }
  }
}


void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect(dimconfig.devicename.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      mqttClient.publish((dimconfig.devicename+"/device/status").c_str(), "Dimmer booted");
      // ... and resubscribe
      mqttClient.subscribe((dimconfig.devicename+"/#").c_str());
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 1 seconds before retrying
      for(int i = 0; i<1000; i++){

      }
    }
  }
}
