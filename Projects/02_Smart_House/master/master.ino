/*
Author              : Genady M (GitHub user name: gy-m)
Arduino IDE         : 1.8.12
GitHub Repository:  : https://github.com/gy-m/Arduino_Uno
==============================================================================
*/

#include <SoftwareSerial.h>
#include <IRremote.h>
#include <Adafruit_ESP8266.h>
#include <Wire.h>
#include <math.h>


// defines pins numbers
int RECV_PIN = 8;                               // Declare variables to hold pin number
//const int IR_TRAN_PIN = 3;                    // could not be defined, but in the source file. Here i only mention it
#define ARD_TX_ESP_RX   5                       // ARD_TX (ARD 5) to ESP_RX
#define ARD_RX_ESP_TX   6                       // ARD_RX (ARD 6) to ESP_TX
#define ESP_RST         4

// ESP defenitions
SoftwareSerial espSerial =  SoftwareSerial(ARD_RX_ESP_TX, ARD_TX_ESP_RX);   // ARD_RX_ESP_TX = TX of ESP,  arduino pin 3 = RX of ESP

// IR receive
IRrecv irrecv(RECV_PIN);      // Create irrecv object
decode_results results_IR;   // declare variable to hold decoded IR receiver result

// ir transmit
IRsend irsend;               // Create irsend object


// ir codes

// tv toggle
unsigned int  rawData_tv_toggle[67] = {4500,4500, 500,1700, 550,1700, 500,1700, 550,600, 500,600, 500,600, 550,600, 500,600, 500,1700, 550,1700, 550,1650, 550,600, 500,600, 550,550, 550,600, 500,600, 500,600, 550,1700, 500,600, 550,550, 550,600, 500,600, 500,600, 550,550, 550,1700, 550,550, 550,1700, 500,1700, 550,1700, 550,1650, 550,1700, 550,1700, 500};  // SAMSUNG E0E040BF

// ac on
//unsigned int  rawData_ac_on[139] = {8900, 4450, 650, 1650, 600, 500, 600, 450, 650, 1650, 550, 1700, 600, 500, 600, 500, 550, 550, 550, 1650, 650, 450, 650, 450, 650, 1600, 600, 500, 650, 450, 600, 1650, 700, 400, 650, 1600, 650, 1600, 600, 1650, 650, 1600, 650, 450, 650, 450, 600, 1650, 650, 450, 600, 500, 600, 1650, 650, 450, 600, 500, 700, 400, 650, 450, 650, 450, 600, 500, 600, 500, 600, 500, 600, 500, 600, 500, 650, 1600, 600, 1650, 600, 1650, 600, 1650, 600, 500, 600, 500, 600, 500, 650, 450, 650, 450, 600, 500, 600, 500, 600, 500, 650, 1600, 600, 1650, 600, 1650, 600, 1650, 600, 500, 600, 500, 650, 450, 600, 500, 600, 500, 600, 500, 600, 500, 600, 500, 600, 500, 600, 1650, 600, 500, 650, 1600, 600, 1650, 600, 500, 600, 1650, 600, 500, 550}; // NEC 9892F240

// ac off
//unsigned int  rawData_ac_off [139] = {8900, 4500, 550, 1700, 600, 500, 550, 550, 550, 550, 550, 1700, 550, 550, 550, 550, 550, 550, 550, 1700, 550, 550, 550, 550, 550, 1700, 550, 1700, 550, 550, 550, 550, 500, 600, 550, 550, 550, 1700, 500, 1750, 550, 550, 550, 550, 550, 550, 550, 1700, 550, 550, 550, 550, 550, 1700, 500, 600, 550, 550, 550, 500, 550, 550, 600, 500, 550, 600, 500, 550, 550, 550, 550, 550, 550, 550, 550, 1700, 550, 1700, 550, 1700, 550, 1700, 550, 550, 550, 600, 500, 600, 500, 550, 550, 600, 500, 550, 550, 550, 550, 550, 550, 1700, 550, 1700, 550, 1700, 500, 1750, 500, 600, 500, 600, 550, 550, 550, 550, 550, 550, 550, 550, 500, 600, 500, 600, 550, 550, 550, 1700, 550, 550, 550, 1700, 550, 1700, 500, 600, 550, 1700, 550, 550, 550}; // NEC 88986240

// irobot on and off
//const unsigned int  rawData_irobot_tog[] PROGMEM = {8950,4400, 600,500, 550,550, 550,550, 550,550, 550,550, 550,550, 550,1700, 550,550, 550,1700, 550,550, 550,1650, 550,550, 550,1700, 550,550, 500,1700, 550,550, 550,550, 550,550, 550,1700, 550,550, 550,550, 550,550, 500,1700, 550,600, 550,1650, 500,1700, 550,550, 550,1700, 550,1650, 600,1650, 550,550, 500,1700, 550};  // NEC 2AA22DD

// irobot home
//const unsigned int  rawData_irobot_home[] PROGMEM = {8950,4450, 500,600, 500,600, 550,550, 600,500, 550,550, 550,550, 550,1700, 550,550, 500,1700, 550,550, 550,1700, 500,600, 500,1700, 550,550, 550,1700, 550,550, 550,1650, 550,550, 550,600, 550,550, 500,1700, 500,600, 500,600, 550,550, 550,550, 550,1700, 500,1700, 550,1700, 550,550, 500,1700, 550,1700, 500,1700, 550};  // NEC 2AA8877

// time variables
int time_current = 0, time_delta = 5000, time_out;
bool DAY = false;
bool HR = false;
bool MIN = false;

// components status
bool TV_PREV = false;   // previous state: true = on, false = off
bool TV_CUR = false;    // current state: true = on, false = off. The reasin why for tv we use 2 booleans of this kind its beacause on and off is the same IR code
bool AC = false;
bool IROBOT = false;
bool next_msg_is_data = false;
bool during_transition = false;

void setup()
{
  uint32_t setup_time = millis();

  /**************************************************** setup serial ****************************************************/
  Serial.println(F("****************** setup serial communication ******************"));
  // Enable Serial connection at given baud rate
//  Serial.begin(9600);
  Serial.begin(115200);   // must be faster than ESP send me the information. So ESP will be defined here (and in esp's code) lower
  // Wait a bit until device is started
  delay(2000);
  // check serial ok
  Serial.println(F("setup serial communication - done"));

  /**************************************************** setup master arduino *******************************************/
  Serial.println(F("****************** setup master arduino ******************"));
  Wire.begin(); // join to i2c bus as a master (address optional for master)
  Serial.println(F("setup master arduino - done"));

  /**************************************************** setup ir rec****************************************************/
  
    Serial.println("Enabling IR Receiver"); // Print text on serial monitor
    irrecv.enableIRIn();                    // Start the IR receiver - note it is an interrupt!
    Serial.println("Enabled IR Receiver"); // Print text on serial monitor
  

  /**************************************************** setup ir trans**************************************************/
 // irsend.enableIROut(38);

  /**************************************************** setup ESP ******************************************************/
  Serial.println(F("****************** setup ESP8266-01  ******************"));
  espSerial.begin(9600);        // must be slower than rate of terminal of the arduino
  //espSerial.begin(115200);
  Serial.println(F("Local ESP is 192.168.1.100"));    // configured in the router to be fixed IP
  Serial.println(F("setup ESP - done"));

  /**********************************************************************************************************************/

  Serial.println("setup time took " + String((millis() - setup_time)) + "ms to start");
  Serial.println();  
  Serial.println(F("Legend:"));
  Serial.println(F("ESP debug message line will begin with: *"));
  Serial.println(F("ESP debud message section will begind and end with: ***"));
  Serial.println(F("ESP message section which will must be analyzed will begin and end with: $$$"));
  Serial.println();  
  Serial.println(F("****************** End of setup ******************\n"));

}

void loop()
{
  int ack_for_esp = 0;   // if true, we must send answer to the server

  // we do not need to send any thing to esp
  /*
    // while serial is available, read the data from the serial and send to esp
    while (Serial.available() && time_current < time_out) {
      char ch = Serial.read();
      espSerial.print(ch);
    }
  */

  // if esp is available eneter here instead of CLI
  if (espSerial.available()) {
    Serial.println(F("\n-------start streaming messages from the ESP server:------- \n"));

    // while esp is available to send data, read this data and print it
    while (espSerial.available()) {

      // get the String from the esp server (one string can be few messages, but it will get to one time untill we get \n
      String esp_server_msg;
      esp_server_msg = espSerial.readStringUntil('\n');      // the first byte, or -1 if no data

      // convert server_msg (String type) to char[] type
      char esp_server_msg_chars[esp_server_msg.length()];
      esp_server_msg.toCharArray(esp_server_msg_chars, esp_server_msg.length());

      // print the message we got from the esp server
      Serial.println(esp_server_msg_chars);

      update_status_elements(esp_server_msg_chars);
    }
    Serial.println(F("\n------------End of streaming from the ESP server------------"));
  }
}


void update_status_elements(char* esp_server_msg_chars) {

  // if we recognize new transmition from the esp, we will set the appropriate flags
  if (strcmp(esp_server_msg_chars, "$$$ Start of transmition to Arduino") == 0) {
    //Serial.println(esp_server_msg_chars);
    during_transition = true;
    next_msg_is_data = true;
  }

  // if we got to the end of transmittion, we will diable the appropriate flags
  // then, we can trigger the toggle_elements() func
  else if (strcmp(esp_server_msg_chars, "$$$ End of transmition to Arduino") == 0) {
    //Serial.println(esp_server_msg_chars);
    Serial.println();
    during_transition = false;
    next_msg_is_data = false;   // next_msg_is_data will be false, because we finished getting data
    toggle_elements();          // due to end of transition, we can start toggling the IR elements
  }

  // otherwise, we still during transition and suppose to set the appropriate elements
  else if (during_transition) {
    //Serial.println(esp_server_msg_chars);
    
    if (strcmp(esp_server_msg_chars, "TV: 0") == 0) {
      TV_CUR = false;
    }
    else if (strcmp(esp_server_msg_chars, "TV: 1") == 0) {
      TV_CUR = true;
    }

    if (strcmp(esp_server_msg_chars, "AC: 1") == 0) {
      AC = true;
    }
    else if (strcmp(esp_server_msg_chars, "AC: 0") == 0) {
      AC = false;
    }
    
    if (strcmp(esp_server_msg_chars, "IROBOT: 1") == 0) {
      IROBOT = true;
    }
    else if (strcmp(esp_server_msg_chars, "IROBOT: 0") == 0) {
      IROBOT = false;
    }

    // When we getting the "Boom" it means this is the time and date which is hardcoded in the ESP and we suppose to do something
    if (strcmp(esp_server_msg_chars, "BOOM") == 0) {
      boom();
    }
    
  }
}

void toggle_elements() {
    /*
    Serial.println("Toggling elements: ");
    Serial.println(TV);
    Serial.println(AC);
    Serial.println(IROBOT);    
    */  
    if (TV_PREV != TV_CUR) {
      if (TV_CUR) {
        turn_tv_on();
      }
      else {
        turn_tv_off();
      }
      TV_PREV = TV_CUR;
    }

    if (AC == true) {
      turn_ac_on();
    }
    else if (AC == false) {
      turn_ac_off();
    }
    if (IROBOT == true) {
      send_irobot_tog();
    }
    else if (IROBOT == false) {
      send_irobot_tog();
    }
}

void boom() {
  //turn_ac_on();
  turn_tv_on();  
}

void turn_tv_on() {
  Serial.println(F("Turning on TV"));
  irsend.sendRaw(rawData_tv_toggle, sizeof(rawData_tv_toggle) / sizeof(int), 38 );
}

void turn_tv_off() {
  Serial.println(F("Turning off TV"));
  irsend.sendRaw(rawData_tv_toggle, sizeof(rawData_tv_toggle) / sizeof(int), 38 );
}

void turn_ac_on() {
  //Serial.println(F("Turning on AC"));
  //irsend.sendRaw(rawData_ac_on, sizeof(rawData_ac_on) / sizeof(int), 38 );
  Serial.println(F("Turning on AC - Failed: AC on/off function is disabled (low memory availabe)"));
}

void turn_ac_off() {
  //Serial.println(F("Turning off AC"));
  //irsend.sendRaw(rawData_ac_off, sizeof(rawData_ac_off) / sizeof(int), 38 );
  Serial.println(F("Turning off AC - Failed: AC on/off function is disabled (low memory availabe)"));
}


void send_irobot_tog() {
  //Serial.println(F("Tuggling IROBOT"));
  //irsend.sendRaw(rawData_irobot_tog, sizeof(rawData_irobot_tog) / sizeof(int),38 );
  Serial.println(F("Tuggling IROBOT - Failed: Tuggling IROBOT function is disabled (low memory availabe)"));
}
