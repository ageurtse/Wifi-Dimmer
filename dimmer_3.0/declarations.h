

// Settings stored in tiny eeprom
const byte SWversion = 1;


//Config structure
/* Set these to desired credentials in runtime */
struct DimConfig {
//Dim config parameters   
  byte version;
  byte minDimValue;
  byte maxDimValue;
  int  dimCurve;              // range goes -10 til 10
  byte prescaller;

//mqtt config parameters
String mqtt;
};

const char *fileName = "/config.txt";  // <- SD library uses 8.3 filenames
DimConfig dimconfig;                   // <- global configuration object
