# Network Scanner
## Table of contents
* [Project's aim](#Project's-aim")
* [Features](#Features)
* [Software and code](#Software-and-code)
* [Hardware](#Hardware)
* [Technologies](#Technologies)
* [Project's status](#Project's-status)

## Project's aim
Control Remote "Control Unit" (Arduino Uno), by getting commands from "Local Server" (ESP8266), which will be triggered by "Remote Server" (On pythonanywhere.com).

## Features
The features are described according to the components of the project:
* Remote Server - Landing page and Control Panel with GUI, which enables the user to control the "Control Unit" through different projects.
* Local Server - Getting a user request (for example turning on "AC") from the Remote server's GUI and sending it to the "Control Unit".
* Control Unit - Processing the user's request and perform the required action using peripheral components if needed.

Detailed features can be found on the remote server's page: http://genam.pythonanywhere.com/

## Software and code
* Control Unit (Arduino Uno and Arduino nano) - Under "master" folder (Arduino Uno), and "slave" folder ("Arduino nano")
* Local server (ESP8266) - Under the "server" folder
* Remote server (Pythonanywhere.com) - Please refer to the code in a different repository: https://github.com/gy-m/personal_website_ng

## Hardware
* Microcontroller - Arduino Uno
* Local Server - ESP8266
* Remote Server - Pythonanywhere

## Technologies
* C - For the ESP8266 and Arduino Uno
* Django (Python) - For the "Remote Server"
* Web - HTML, CSS
* Networking and additional protocols - TCP/IP, I2C

## Project's status
Beta - The code was tested and works. However, the GUI (in the project's page of http://genam.pythonanywhere.com/) stopped processing the request at the moment. To fix it, the following action must be taken:
* Renew the paid membership for the "pythonanywhere.com" service.
* Replace third party service which provided time-limited access "IP" for accessing the "Local Server" from the "Remote server". This can be done by configuring the router to process IP requests from the "Remote server", instead of using a third-party service.

TODO:
* Fix the code of "local server" to be more clear and usable
* Add Documentation folder - Wire Diagram, Demonstration, Inventory
* Renew the paid membership for the "pythonanywhere.com" service.
* Replace third party service, by configuring the router.