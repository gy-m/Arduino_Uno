/*
Author              : Genady M (GitHub user name: gy-m)
Arduino IDE         : 1.8.12
GitHub Repository:  : https://github.com/gy-m/Arduino_Uno
==============================================================================
*/

#include <Wire.h>

#define SLAVE_NUM_ARD 8

char msg_to_slave[] = {"I am your master"};
int send_event_number = 0;

void setup() {
  ////////////////////// set communication /////////////////
  Serial.begin(9600);           // start serial for output
  
  ////////////////////// set receiver //////////////////////
  Wire.begin(); // join to i2c bus as a master (address optional for master)
}


void loop() {
  // information for the terminal
  Serial.print("sending event_number: ");
  Serial.print(send_event_number);
  Serial.print(" number of bytes: ");
  Serial.println(sizeof(msg_to_slave));
  Serial.print("msg to send: ");
  Serial.println(msg_to_slave);


  // transmission
  Wire.beginTransmission(SLAVE_NUM_ARD);        // transmit to device #8
  Wire.write(msg_to_slave, sizeof(msg_to_slave)); // send msg_to_send, and the amount of bytes
  Wire.endTransmission();                       // stop transmitting
  send_event_number++;

  // request for reply
  Wire.requestFrom(SLAVE_NUM_ARD, 3);           // request 3 bytes from slave device #SLAVE_NUM_ARD
  Serial.println("Requesting from slave.");
  Serial.print("Slave: ");
  while (Wire.available()) {                    // slave may send less than requested
    char c = Wire.read();     // receive a byte as character
    Serial.print(c);         // print the character    
  }
  Serial.println();
  Serial.println();
  
  // frequency
  delay(3000);
}
