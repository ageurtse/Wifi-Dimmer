
// ==========================================
// All dimmer settings goes in here
// ==========================================
const byte SWversion = 2.1;


//Config dimmer structure
struct DimConfig {
//Dim config parameters   
  float dimversion;
  byte minDimValue;           // min value 4
  byte maxDimValue;           // max value 64
  int  dimCurve;              // range goes -10 til 10
  byte dimPrescale;           // default 8 B00001000

//device and mqtt config parameters
  String devicename;
  String mqttServer;
  String mqttUsername;
  String mqttPassword;
};

const char *fileName = "/config.txt";  // filename of config file.
DimConfig dimconfig;                   // global configuration object
