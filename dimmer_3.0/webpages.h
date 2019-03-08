//Display root page
void handleRoot() {
  String  content =
    "<html>"
    "<head>"
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
    "</head>"
    "<body>"
    "<centrer>WIFI Dimmer 2.0</center></p>"
    "<p style=\"padding-top:10px;text-align:center\">" AUTOCONNECT_LINK(COG_24) "</p>"
    "</body>"
    "</html>";
  webServer.send(200, "text/html", content);
}


// Loading from PROGMEM
const char settings[] PROGMEM = R"raw(
{
  "title" : "Dimmer Settings",
  "uri" : "/Settings",
  "menu" : true,
  "element" : [
    {
      "name" : "caption1",
      "type" : "ACText",
      "value" : "<H1>Settings</H1></br>"
    },
     {
      "name" : "caption2",
      "type" : "ACText",
      "value" : "<H2>Device settings</h2>"
    },
    { "name" : "devicename",
      "type" : "ACInput",
      "value" : "10",
      "label" : "Device name, also topic name for MQTT</br>"
    },    
    { "name" : "mqttServer",
      "type" : "ACInput",
      "value" : "127.0.0.1",
      "label" : "IP adres MQTT server</br>"
    },    
    { "name" : "mqttUsername",
      "type" : "ACInput",
      "value" : "",
      "label" : "MQTT user name</br>"
    },  
    { "name" : "mqttPassword",
      "type" : "ACInput",
      "value" : "",
      "label" : "MQTT user password</br>"
    },  
    {
      "name" : "caption3",
      "type" : "ACText",
      "value" : "<H2>Dimmer settings</h2>"
    },      
    { "name" : "minDim",
      "type" : "ACInput",
      "value" : "10",
      "pattern": "^[0-6]?[0-9]$",    //review
      "label" : "Minimum dimvalue</br>"
    },    
    { "name" : "maxDim",
      "type" : "ACInput",
      "value" : "10",
      "pattern": "^[0-6]?[0-9]$",    //review
      "label" : "Maximum dimvalue</br>"
    },
    { "name" : "dimCurve",
      "type" : "ACInput",
      "value" : "0",
      "label" : "Dimcurve</br>"
    },
    {
      "name" : "caption4",
      "type" : "ACText",
      "value" : "<H2>Advanced settings</h2>"
    },
    { "name" : "dimPrescale",
      "type" : "ACInput",
      "value" : "0",
      "label" : "Prescaller Attiny</br>"
    },
    {
      "name": "save",
      "type": "ACSubmit",
      "value": "Save",
      "uri": "/saveDim"
    }
  ]
}
)raw";

//when hitting save, here the values are stored. 
void saveSetting() {
  // Retrieve the value of a input-box named "feels"
  dimconfig.devicename    = webServer.arg("devicename");
  dimconfig.mqttServer    = webServer.arg("mqttServer");
  dimconfig.mqttUsername  = webServer.arg("mqttUsername");
  dimconfig.mqttPassword  = webServer.arg("mqttPassword");
  
    
  dimconfig.minDimValue   = webServer.arg("minDim").toInt();
  dimconfig.maxDimValue   = webServer.arg("maxDim").toInt();
  dimconfig.dimCurve      = webServer.arg("dimCurve").toInt();
  dimconfig.dimPrescale   = webServer.arg("dimPrescale").toInt();
    
  
  saveConfiguration(fileName, dimconfig);
  // Echo back the value

  String  content =
    "<html>"
    "<head>"
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
    "</head>"
    "<body>"
    "<centrer>Done saving !</center></p>"
    "<p style=\"padding-top:10px;text-align:center\">" AUTOCONNECT_LINK(COG_24) "</p>"
    "</body>"
    "</html>";
  webServer.send(200, "text/html", content);
}


// Here the values are loaded into the webpage
String settingsOn(AutoConnectAux& aux, PageArgument& args) {
  AutoConnectInput& devicename = aux.getElement<AutoConnectInput>("devicename");
  devicename.value = dimconfig.devicename;

  AutoConnectInput& mqttServer = aux.getElement<AutoConnectInput>("mqttServer");
  mqttServer.value = dimconfig.mqttServer;
  
  AutoConnectInput& mqttUsername = aux.getElement<AutoConnectInput>("mqttUsername");
  mqttUsername.value = dimconfig.mqttUsername;

  AutoConnectInput& mqttPassword = aux.getElement<AutoConnectInput>("mqttPassword");
  mqttPassword.value = dimconfig.mqttPassword;

    
  
  AutoConnectInput& minDim = aux.getElement<AutoConnectInput>("minDim");
  minDim.value = dimconfig.minDimValue;
  
  AutoConnectInput& maxDim = aux.getElement<AutoConnectInput>("maxDim");
  maxDim.value = dimconfig.maxDimValue;

  AutoConnectInput& dimCurve = aux.getElement<AutoConnectInput>("dimCurve");
  dimCurve.value = dimconfig.dimCurve;


  AutoConnectInput& dimPrescale = aux.getElement<AutoConnectInput>("dimPrescale");
  dimPrescale.value = dimconfig.dimPrescale;
  
  return String();
}
