# WeatherStation
A simple indoor weather station project which captures temperature, air pressure and humidity (as of now).

## Hardware
The weatherstation consists of an ESP8266 from NodeMCU (Amica). The Wifi chip is an ESP8266MOD from AI-THINKER vendor.

Temperature and Humidity Sensor: AM2320
Datasheet: https://cdn-shop.adafruit.com/product-files/3721/AM2320.pdf

Temperature and Pressure sensor: BMP388
Datasheet: https://wiki.dfrobot.com/BMP388_Digital_Pressure_Sensor_Module_SKU_SEN0371

The components are mounted on a 40x60mm experiment board according to the image and circuit diagram below.

## Software
Software for the ESP8266 was built in Arduino IDE version 1.8.19
Link: https://www.arduino.cc/en/software

Libraries used can be seen in the .ino file.

## Box
The weatherstation box was made in FreeCAD version 0.20.0 in Linux.
It was 3D printed using a Flashforge Finder 3D printer.

## Quickstart Guide
Following steps should be sufficient to get going
- Install Arduino IDE
- Install necessary libraries in Arduino IDE
-  Go to Arduino IDE preferences (in your project) and put the following link in the "Additional Boards Manager URLs" to enable the ESP8266 for the IDE.
https://arduino.esp8266.com/stable/package_esp8266com_index.json
- Connect according to schematics (Some pins should not be connected during code upload, upload won't work. See "espModuleBoot.png")

LittleFS lets you put the javascript, html, and css files in the ESP8266 filesystem.
Put the (your) javascript, css, and html files in a folder called "data" next to your .ino file.
Then press "Tools" -> "ESP8266 LittleFS Data Upload" to upload the files to the ESP8266. 
(Serial Monitor has to be closed during upload in order not to block the serial interface)
## Future improvements
Some improvements which I, or you, may add in the future:
- UART touch display for interacting with the device and see livedata
- Carbon dioxide sensor
- Improve placement for sensors to get higher accuracy measurements
- Implement deep sleep to make the weatherstation efficient when running on a battery
- Clean up / Structure code



