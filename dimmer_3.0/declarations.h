

// Settings stored in tiny eeprom
const byte SWversion = 1;


//Config dimmer structure
/* Set these to desired credentials in runtime */
struct DimConfig {
//Dim config parameters   
  byte version;
  byte minDimValue;           // min value 4
  byte maxDimValue;           // max value 64
  int  dimCurve;              // range goes -10 til 10
  byte dimPrescale;           // default 8 B00001000

//mqtt config parameters
  String devicename;
  String mqttServer;
  String mqttUsername;
  String mqttPassword;
};

const char *fileName = "/config.txt";  // <- SD library uses 8.3 filenames
DimConfig dimconfig;                   // <- global configuration object
