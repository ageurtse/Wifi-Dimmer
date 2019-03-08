// ==========================================
// Saves the configuration to a file
// ==========================================
bool saveConfiguration(const char *filename, const DimConfig &saveconfig) {
  // Delete existing file, otherwise the configuration is appended to the file
  
  SPIFFS.remove(fileName);
  
  // Open file for writing
  File file = SPIFFS.open(fileName, "w");
  if (!file) {
    Serial.println(F("Failed to create file"));
    return false;
  }

  // Allocate the memory pool on the stack
  // Don't forget to change the capacity to match your JSON document.
  // Use https://arduinojson.org/assistant/ to compute the capacity.
  StaticJsonBuffer<256> jsonBuffer;

  // Parse the root object
  JsonObject &root = jsonBuffer.createObject();

  // Set the values
  root["dimversion"]    = SWversion;
  root["minDimValue"]   = saveconfig.minDimValue;  
  root["maxDimValue"]   = saveconfig.maxDimValue;  
  root["dimCurve"]      = saveconfig.dimCurve;  
  root["dimPrescale"]    = saveconfig.dimPrescale;  

  root["devicename"]    = saveconfig.devicename;  
  root["mqttServer"]    = saveconfig.mqttServer;  
  root["mqttUsername"]  = saveconfig.mqttUsername;  
  root["mqttPassword"]  = saveconfig.mqttPassword;  
  
  // Serialize JSON to file
  if (root.printTo(file) == 0) {
    Serial.println(F("Failed to write to file"));
  } else
  { 
    Serial.println(F("File saved")); 
  }

  // Close the file (File's destructor doesn't close the file)
  file.close();
}


// ==========================================
// Loads the configuration from a file
// ==========================================
bool loadConfiguration(const char *filename, DimConfig &loadconfig) {
  // Open file for reading
  File file = SPIFFS.open(fileName, "r");
  if (!file) {
    Serial.println("Failed to open config file");
    loadconfig.minDimValue  = 15;  
    loadconfig.maxDimValue  = 48;  
    loadconfig.dimCurve     = 6;  
    loadconfig.dimPrescale  = 8;  

    saveConfiguration(fileName, dimconfig);
    return false;
  }

  
  // Allocate the memory pool on the stack.
  // Don't forget to change the capacity to match your JSON document.
  // Use arduinojson.org/assistant to compute the capacity.
    StaticJsonBuffer<512> jsonBuffer;

    // Parse the root object
    JsonObject &root = jsonBuffer.parseObject(file);

    if (!root.success())
      Serial.println(F("Failed to read file, using default configuration"));

// Copy values from the JsonObject to the Config

  loadconfig.dimversion   = root["dimversion"];
  Serial.println(F("Valid config file."));
  
  loadconfig.minDimValue  = root["minDimValue"] | 15;  
  loadconfig.maxDimValue  = root["maxDimValue"] | 48 ;  
  loadconfig.dimCurve     = root["dimCurve"] | 6;  
  loadconfig.dimPrescale  = root["dimPrescale"] | 8;  

  loadconfig.devicename   = (const char*)root["devicename"]; 
  loadconfig.mqttServer   = (const char*)root["mqttServer"]; 
  loadconfig.mqttUsername = (const char*)root["mqttUsername"]; 
  loadconfig.mqttPassword = (const char*)root["mqttPassword"]; 

  // Close the file (File's destructor doesn't close the file)
  file.close();
}

// ==========================================
// Prints the content of a file to the Serial
// ==========================================
bool printFile(const char *filename) {
  // Open file for reading
  File file = SPIFFS.open(fileName, "r");

  if (!file) {
    Serial.println(F("Failed to read file"));
    return false;
  }

  // Extract each characters by one by one
  while (file.available()) {
    Serial.print((char)file.read());
  }
  Serial.println();

  // Close the file (File's destructor doesn't close the file)
  file.close();
}
// End file handeling
