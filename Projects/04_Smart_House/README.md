# Smart House
## Table of contents
* [Project's aim](#Project's-aim)
* [Features](#Features)
* [Software and code](#Software-and-code)
* [Hardware](#Hardware)
* [Technologies](#Technologies)
* [Usage](#Usage)
* [Known issues](#Known-issues)
* [Project's status](#Project's-status)

## Project's aim
Controlling IR elements using a web page, from anywhere in the world.

## Features
* Web page - The web page can control (toggle on/off) on the bellow elements:
	* TV
	* AC
	* IROBOT
* Timer - The ESP holding a specific Time (day, hour, min) which will trigger a message to the Arduino ("BOOM" message). For demonstration purposes, this version toggling TV when getting the "BOOM" message.

## Software and code
* Control Unit (Arduino Uno) - Under "master" folder (Arduino uno)
* Server (ESP8266) - Under "server" folder

## Hardware
* Microcontroller (Master) - Arduino Uno
* Server - ESP8266
* Peripherials - IR receiver and transmitter

## Technologies
* C - For the ESP8266 and Arduino Uno
* Web - HTML, CSS
* Networking and additional protocols - TCP/IP, I2C, IR
* IOT

## Usage
* Building the circuit - Please refer to the documentation folder for additional details and wire diagram.
* Finding IR code - Before using the provided code, you should find out your own IR code of your device you wish to operate. For that purpose use the example code provided by Arduino's IDE, named "IRrecDump2". This is the reason why you should have the IR receiver. After burning this code to Arduino, open the terminal and use the IR remote to find out the relevant code.
* Insert WIFI credentials - Before using the code for the ESP, the user should update it with his credentials of the SSID and WIFI password.
* Program ESP8266 
	* Program ESP8266 using a USB adapter and make sure to open the terminal. You will find the local IP address which was allocated for your device. make sure this address is the same as the "send_to_client_html" function contains. Otherwise, edit the IP address in this function accordingly.
	* Write down this address because this is the address you will use in your browser.
	* Edit the "Boom" function to do your desired actions
	* Edit the "Boom" function call to perform those actions according to your schedule
* Program Arduino Uno
* Open the browser and enter your local IP of the ESP. You can use this IP from anywhere in the world, as long as you using a VPN for your local network.


<kbd>
  <img src="https://github.com/gy-m/Arduino_Uno/blob/master/Projects/04_Smart_House/Documentation/Demonstration_Browser.png">
</kbd>


## Known issues
Due to low memory (Arduino), the functionality which controls the AC and IROBOT is disabled (by comment). There is an option to enable it by uncommenting the relevant code and functions, but then you should disable other functionality.
Note - TV (on / off) can be used with AC (On only).

## Project's status
Done - The code was tested and works