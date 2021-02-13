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
IRsend irsend;               // Create irsend object

///////////////// ir codes ///////////
//#define Something_HEX 8CE0040A

// ac on
unsigned int  rawData_ac_on[139] = {8900,4450, 650,1650, 600,500, 600,450, 650,1650, 550,1700, 600,500, 600,500, 550,550, 550,1650, 650,450, 650,450, 650,1600, 600,500, 650,450, 600,1650, 700,400, 650,1600, 650,1600, 600,1650, 650,1600, 650,450, 650,450, 600,1650, 650,450, 600,500, 600,1650, 650,450, 600,500, 700,400, 650,450, 650,450, 600,500, 600,500, 600,500, 600,500, 600,500, 650,1600, 600,1650, 600,1650, 600,1650, 600,500, 600,500, 600,500, 650,450, 650,450, 600,500, 600,500, 600,500, 650,1600, 600,1650, 600,1650, 600,1650, 600,500, 600,500, 650,450, 600,500, 600,500, 600,500, 600,500, 600,500, 600,500, 600,1650, 600,500, 650,1600, 600,1650, 600,500, 600,1650, 600,500, 550};  // NEC 9892F240
//unsigned int  data_ac_on = 0x9892F240;

// ac off
unsigned int  rawData_ac_off [139] = {8900,4500, 550,1700, 600,500, 550,550, 550,550, 550,1700, 550,550, 550,550, 550,550, 550,1700, 550,550, 550,550, 550,1700, 550,1700, 550,550, 550,550, 500,600, 550,550, 550,1700, 500,1750, 550,550, 550,550, 550,550, 550,1700, 550,550, 550,550, 550,1700, 500,600, 550,550, 550,500, 550,550, 600,500, 550,600, 500,550, 550,550, 550,550, 550,550, 550,1700, 550,1700, 550,1700, 550,1700, 550,550, 550,600, 500,600, 500,550, 550,600, 500,550, 550,550, 550,550, 550,1700, 550,1700, 550,1700, 500,1750, 500,600, 500,600, 550,550, 550,550, 550,550, 550,550, 500,600, 500,600, 550,550, 550,1700, 550,550, 550,1700, 550,1700, 500,600, 550,1700, 550,550, 550};  // NEC 88986240
//unsigned int  data_ac_off = 0x88986240;


// irobot on and off
unsigned int  rawData_irobot_tog[] = {8950,4400, 600,500, 550,550, 550,550, 550,550, 550,550, 550,550, 550,1700, 550,550, 550,1700, 550,550, 550,1650, 550,550, 550,1700, 550,550, 500,1700, 550,550, 550,550, 550,550, 550,1700, 550,550, 550,550, 550,550, 500,1700, 550,600, 550,1650, 500,1700, 550,550, 550,1700, 550,1650, 600,1650, 550,550, 500,1700, 550};  // NEC 2AA22DD
//unsigned int  data_irobot_tog = 0x2AA22DD;

// irobot home
unsigned int  rawData_irobot_home[] = {8950,4450, 500,600, 500,600, 550,550, 600,500, 550,550, 550,550, 550,1700, 550,550, 500,1700, 550,550, 550,1700, 500,600, 500,1700, 550,550, 550,1700, 550,550, 550,1650, 550,550, 550,600, 550,550, 500,1700, 500,600, 500,600, 550,550, 550,550, 550,1700, 500,1700, 550,1700, 550,550, 500,1700, 550,1700, 500,1700, 550};  // NEC 2AA8877
//unsigned int  data_irobot_home = 0x2AA8877;



void setup(void) {
  
  Serial.begin(9600);
  //////////////////////////////////////////////////////
  ////////////  setup for IR reciever /////////////////
  //////////////////////////////////////////////////////
  
  Serial.println("Enabling IR Receiver"); // Print text on serial monitor
  irrecv.enableIRIn();                    // Start the IR receiver
  Serial.println("Enabled IR Receiver"); // Print text on serial monitor

  //////////////////////////////////////////////////////
  ////////////  setup for IR transmitter ///////////////
  ////////////////////////////////////////////////////// 
//  irsend.enableIROut(38);
  

}

void loop(void) {

    // AC - on and off
    Serial.println("Turning on AC");  
    irsend.sendRaw(rawData_ac_on, sizeof(rawData_ac_on) / sizeof(int),38 ); 
    delay(3000);
    Serial.println("Turning off AC");
    irsend.sendRaw(rawData_ac_off, sizeof(rawData_ac_off) / sizeof(int),38 ); 
    delay(3000);


   // IROBOT - on / off and home - sometimes does not receive the message
   Serial.println("Tuggling IROBOT");
   irsend.sendRaw(rawData_irobot_tog, sizeof(rawData_irobot_tog) / sizeof(int),38 ); 
   delay(3000);
   Serial.println("Sending home IROBOT");
   irsend.sendRaw(rawData_irobot_home, sizeof(rawData_irobot_home) / sizeof(int),38 ); 
   delay(3000);

  // if received a new code
  if (irrecv.decode(&results_IR)) // Checks the code received or not
  {
    Serial.println("Received a new code");   
    
    Serial.print("number of bits: ");   
    Serial.println(results_IR.bits);
    
    Serial.print("code HEX: ");
    Serial.println(results_IR.value, HEX);      // Print the decoded value on serial monitor (hex - 989E5A60) - other: off = 889B8680, on = 9892C680
    Serial.print("code dec: ");
    Serial.println(results_IR.value);           // Print the decoded value on serial monitor (dec - 2559646304)
      
    Serial.print("type: ");
    Serial.println(results_IR.decode_type);

    Serial.println("checking type");
    if (results_IR.decode_type == NEC) {
      Serial.print("NEC: ");      
    } if (results_IR.decode_type == SONY) {
      Serial.print("SONY: ");
    } if (results_IR.decode_type == RC5) {
      Serial.print("RC5: ");
    } if (results_IR.decode_type == RC6) {
      Serial.print("RC6: ");
    } if (results_IR.decode_type == UNKNOWN) {
      Serial.print("UNKNOWN: ");
    }

    irrecv.resume(); // continue to receive the next value
  }
}
