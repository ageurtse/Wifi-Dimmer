# Wifi Dimmer
This is all about my home build WiFi Dimmer

![3d view 1](https://user-images.githubusercontent.com/2020670/50725586-ef5c0180-10ff-11e9-834a-b4fb0577eff1.jpg)

This is version 2, in the past i build a differt version which is not exactly what i want. 
Version 2 is one which fit in a simpel housing. 

There are 2 parts of the software, one for the ATTiny and one for the ESP8266
Both are posted here. 

# ATtiny85 Software
The ATtiny85 software is for the soul purpose of dimming the light, there is a zerocross detction which is on interupt 0, the gate is on pin PB3. 
The communication with the main processor is done by softserial. 
The code is based on the forum post at arduino : https://forum.arduino.cc/index.php?topic=314773.0

# ESP8266 software
The main software for the ESP12 module is build with Sloeber an arduino like enviroment. i think the file will also load in arduino, did not try that. 
After fist flash by serial, all next updates could be down by browser. 
find the IP-adres or host name and add /update to get to the update page. 
To alter wifi setting's and in the futher mqtt settings and i hope a lot more add /_ac

The file makes use of some thirth party library's which can be found here :
AutoConnect : https://github.com/Hieromon/AutoConnect
PageBuilder : https://github.com/Hieromon/PageBuilder



# The hardware
For the main board there is a small mistake made with the Triac, there where 2 wires changes. A1 and A2 need to be turned. 
In a next updat of the print i will fix that, for now i just solderd 2 wires and cross connect them to te pcb.

Also the Fuse holder is to large, i bought a pcb fuse now it all fits well.

