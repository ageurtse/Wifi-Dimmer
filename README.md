# Wifi-Dimmer
This is all about my home build WiFi Dimmer

This is version 2, in the past i build a differt version which is not exactly what i want. 
Version 2 is one which fit in a simpel housing. 

There are 2 parts of the software, one for the ATTiny and one for the ESP8266
Both are posted here. 


The ATtiny85 software is for the soul purpose of dimming the light, there is a zerocross detction which is on interupt 0, the gate is on pin PB3. 
The communication with the main processor is done by softserial. 
The code is based on the forum post at arduino : https://forum.arduino.cc/index.php?topic=314773.0
