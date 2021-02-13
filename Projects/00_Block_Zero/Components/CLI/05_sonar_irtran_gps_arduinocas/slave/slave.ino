/*
Author              : Genady M (GitHub user name: gy-m)
Arduino IDE         : 1.8.12
GitHub Repository:  : https://github.com/gy-m/Arduino_Uno
==============================================================================
*/

#include <IRremote.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// lcdi2c
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
#define SLAVE_NUM_ARD 8

char msg_from_master[128] = {};
int msg_from_master_size = 0;

// IR receive
int RECV_PIN = 12;            // Declare variables to hold pin number
IRrecv irrecv(RECV_PIN);      // Create irrecv object
decode_results results_IR;   // declare variable to hold decoded IR receiver result

// IR transmit
IRsend irsend;               // Create irsend object


// ir codes 

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
  /**************************************************** set communication *******************************************/
  Serial.begin(9600);

  /**************************************************** setup for IR reciever *******************************************/
  Serial.println("Enabling IR Receiver"); // Print text on serial monitor
  irrecv.enableIRIn();                    // Start the IR receiver
  Serial.println("Enabled IR Receiver"); // Print text on serial monitor

  /****************************************************  setup for IR transmitter *******************************************/
//  irsend.enableIROut(38);

  /**************************************************** set LCDI2C *******************************************/
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  lcd.clear();
  lcd.print("Hello, world!");
  
  /**************************************************** setup slave arduino *******************************************/
  Wire.begin(SLAVE_NUM_ARD);    // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event for receiving from master
  Wire.onRequest(requestEvent); // register event for sending to master when he requesting for data
}


void loop(void) {
  
  if(!strcmp(msg_from_master, "ac_on")) {
    turn_ac_on();
    init_msg_arr();
  }
  else if(!strcmp(msg_from_master, "ac_off")) {
    turn_ac_off();
    init_msg_arr();
  }
  
  else if(!strcmp(msg_from_master, "irobot_tog")) {
    send_irobot_tog();
    init_msg_arr();
  }
  else if(!strcmp(msg_from_master, "irobot_home")) {
    send_irobot_home();
    init_msg_arr();
  }
  
   delay(1000);
}


// whenever data is received from master execute this function (it is registered as an eventin  setup())
void receiveEvent(int size_bytes) {
 
  Serial.print(F("number of bytes: "));
  Serial.println(size_bytes);
  Serial.print(F("msg received: "));
  // start receiving by byte
  int i=0;
  while (Wire.available()) {      
    char c = Wire.read();         // receive byte as a character
    Serial.print(c);              // print the character
    msg_from_master[i] = c;
    i++;
    msg_from_master_size++;
  }
  
  Serial.println();
  Serial.println(F("End of receiving"));
    
  return;
}

//  whenever data is requested by master execute this function (it is registered as an eventin  setup())
void requestEvent() {
  // I implemented simple ack, without knowing (or letting master to know) if actually i got the right message
  char msg_to_master[] = {"ACK"};
  // note - the number of bytes we can write to master must be as he expected
  Serial.print(F("Sending to master: "));
  Serial.println(msg_to_master);
  Serial.println();
  Wire.write(msg_to_master, sizeof(msg_to_master));    // respond with message of 3 bytes
}


void init_msg_arr() {
//  Serial.print("Before init: ");
//  Serial.println(msg_from_master);

  for (int i=0; i<msg_from_master_size; i++) {
    msg_from_master[i] = '\0';
  }
}


void turn_ac_on() {
  Serial.println(F("Turning on AC"));  
  irsend.sendRaw(rawData_ac_on, sizeof(rawData_ac_on) / sizeof(int),38 ); 
}


void turn_ac_off() {
  Serial.println(F("Turning off AC"));  
  irsend.sendRaw(rawData_ac_off, sizeof(rawData_ac_off) / sizeof(int),38 ); 
}



void send_irobot_tog() {
   Serial.println(F("Tuggling IROBOT"));
   irsend.sendRaw(rawData_irobot_tog, sizeof(rawData_irobot_tog) / sizeof(int),38 ); 
}


void send_irobot_home() {
   Serial.println(F("Sending home IROBOT"));
   irsend.sendRaw(rawData_irobot_home, sizeof(rawData_irobot_home) / sizeof(int),38 ); 
}
