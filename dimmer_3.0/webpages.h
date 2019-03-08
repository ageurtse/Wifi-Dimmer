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
      "name" : "caption",
      "type" : "ACText",
      "value" : "<H1><center>Dimmer Settings</center></H1></br>"
    },
    { "name" : "minDim",
      "type" : "ACInput",
      "value" : "10",
      "pattern": "^[0-6]?[0-9]$",    //review
      "label" : "Minimum dimvalue"
    },    
    { "name" : "maxDim",
      "type" : "ACInput",
      "value" : "10",
      "pattern": "^[0-6]?[0-9]$",    //review
      "label" : "Maximum dimvalue"
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
  dimconfig.minDimValue = webServer.arg("minDim").toInt();
  dimconfig.maxDimValue = webServer.arg("maxDim").toInt();
  
  
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
  AutoConnectInput& minDim = aux.getElement<AutoConnectInput>("minDim");
  minDim.value = dimconfig.minDimValue;
  
  AutoConnectInput& maxDim = aux.getElement<AutoConnectInput>("maxDim");
  maxDim.value = dimconfig.maxDimValue;
  
  return String();
}
