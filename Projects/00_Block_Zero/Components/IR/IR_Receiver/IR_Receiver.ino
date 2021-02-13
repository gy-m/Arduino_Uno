/*
Author              : Genady M (GitHub user name: gy-m)
Arduino IDE         : 1.8.12
GitHub Repository:  : https://github.com/gy-m/Arduino_Uno
==============================================================================
*/

#include <IRremote.h>

int RECV_PIN = 12;            // Declare variables to hold pin number
IRrecv irrecv(RECV_PIN);      // Create irrecv object
decode_results results_IR;   // declare variable to hold decoded IR receiver result


void setup(void) {
  
  Serial.begin(9600);
  //////////////////////////////////////////////////////
  ////////////  setup for IR reciever /////////////////
  //////////////////////////////////////////////////////
  
  Serial.println("Enabling IR Receiver"); // Print text on serial monitor
  irrecv.enableIRIn();                    // Start the IR receiver
  Serial.println("Enabled IR Receiver"); // Print text on serial monitor

}

void loop(void) {
  int i = 0;

  if (irrecv.decode(&results_IR)) // Checks the code received or not
  {
    Serial.println(results_IR.value, BIN); // Print the decoded value on serial monitor
    irrecv.resume(); // continue to receive the next value
  }

}
