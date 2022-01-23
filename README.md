# Wifi Dimmer

!!!!! Please do not rebuild this device, in my case there was a serious issue with overheating !!!!!


This is all about my home build WiFi Dimmer
Becaus of it's my first github project, the master is still in Beta. 
For me the project works, but there are a lot off things to change, so keep watching for any change.

11-3-2019, final version, the hard and software is ready to use. 
soon i wil post a youtube movie from live controlling a light. 

![3d view 1](https://user-images.githubusercontent.com/2020670/50725586-ef5c0180-10ff-11e9-834a-b4fb0577eff1.jpg)

This is version 2, in the past i build a differt version which is not exactly what i want. 
Version 2 is one which fit in a simpel housing. 

There are 2 parts of the software, one for the ATTiny and one for the ESP8266
Both are posted here. 
# Word of Caution
Please be very carefull you are working with live voltage 230V, this voltage can realy harm you. 
By building the dimmer, i'm not responsible for any faults or what ever. I stoped development off this dimmer, on one night the triax was overheated. luckely i was at home to prevented real major damage. 

# ATtiny85 Software
The ATtiny85 software is for the soul purpose of dimming the light, there is a zerocross detction which is on interupt 0, the gate is on pin PB3. 
The communication with the main processor is done by softserial. 
The code is based on the forum post at arduino : https://forum.arduino.cc/index.php?topic=314773.0

# ESP8266 software
The main software for the ESP12 module is build with arduino, the wifi and custom settings can be done with the great library from Hieromon https://github.com/Hieromon/AutoConnect
After fist flash by serial, all next updates could be down by browser. 
find the IP-adres or host name and add /update to get to the update page. 
To alter wifi setting's and in the futher mqtt settings and i hope a lot more add /_ac

The file makes use of some thirth party library's which can be found here :
AutoConnect : https://github.com/Hieromon/AutoConnect
PageBuilder : https://github.com/Hieromon/PageBuilder
ArduinoJson : https://arduinojson.org/ version 5


# The hardware
For the main board there is a small mistake made with the Triac, there where 2 wires changes. A1 and A2 need to be turned. 
In a next updat of the print i will fix that, for now i just solderd 2 wires and cross connect them to te pcb.

Also the Fuse holder is to large, i bought a pcb fuse now it all fits well.

