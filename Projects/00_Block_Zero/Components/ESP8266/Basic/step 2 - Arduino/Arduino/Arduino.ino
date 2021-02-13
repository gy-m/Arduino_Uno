/*
Author              : Genady M (GitHub user name: gy-m)
Arduino IDE         : 1.8.12
GitHub Repository   : https://github.com/gy-m/Arduino_Uno

Notes:

 1. Upload this code and open the serial monitor
 2. On the serial monitor enter the command AT 
 3. If the response is OK, your esp8266 serial baud rate is 115200 
    and you have to change it to 9600. Enter the command AT+UART_CUR=9600,8,1,0,0
    Then on the void setup make this change: espSerial.begin(9600); and upload again the code.
    Test the communication. Enter again the command AT 
    if the response is OK you are ready to run the example code.

 4. Otherwise if you can't see anything on the serial display 
    change the esp-01 serial baud rate to 9600 or 115200 or other, until you get a response.
    Then enter the command AT+UART_CUR=9600,8,1,0,0 to change the speed to 9600 
 5. If you ESP-01 works only at 115200 don't use Software Serial. Use hardware serial with ARDUINO MREGA board. 

==============================================================================
*/

#include <SoftwareSerial.h>                             
SoftwareSerial espSerial =  SoftwareSerial(2,3);   // arduino RX pin=2  arduino TX pin=3    connect the arduino RX pin to esp8266 TX pin   -  connect the arduino TX pin to esp8266 RX pin. 

// AT                             Command to display the OK message
// AT+GMR                         Command to display ESP-01 version info
// AT+UART_DEF=9600,8,1,0,0       Command to change the default ESP-01 serial baud rate  to 9600    
// AT+RST                         Command to reset
//=============================
void setup() {
 Serial.begin(9600);
 //espSerial.begin(115200);         // Change this value to 9600 or 115200
 espSerial.begin(9600);         // Change this value to 9600 or 115200

}
//==============================
void loop() {
  while (Serial.available()) { 
      char ch = Serial.read();
      espSerial.print(ch);
    }
  
  while (espSerial.available()) { 
      char ch = espSerial.read();
      Serial.print(ch);
    }
}
