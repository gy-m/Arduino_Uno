/*
Author              : Genady M (GitHub user name: gy-m)
Arduino IDE         : 1.8.12
GitHub Repository:  : https://github.com/gy-m/Arduino_Uno
Credits:
	Stefan Kremser
	Source: github.com/spacehuhn/SimpleCLI
==============================================================================
*/

// Inlcude Library - the path will be 2 folders up
#include <SoftwareSerial.h>
#include <SimpleCLI.h>
#include <IRremote.h>
#include <TinyGPS++.h>
#include <Adafruit_ESP8266.h>


// defines pins numbers
//int RECV_PIN = 12;                    // Declare variables to hold pin number
const int SONAR_TRIG_PIN PROGMEM = 13;
const int SONAR_ECHO_PIN PROGMEM= 12;
const int GPS_TXPin PROGMEM = 2;                // pin of GPS is TX, pin of arduino is 2 (rx = arduino receive)
const int GPS_RXPin PROGMEM = 4;                // pin of GPS is RX, pin of arduino is 3 (tx = arduino transfer) - do not forget resistors! pin 3 going to resistor first! but this pin is not mandatory!
//const int IR_TRAN_PIN = 3;            // could not be defined, but in the source file. Here i only mention it
#define ARD_RX_ESP_TX   10
#define ARD_TX_ESP_RX   11
#define ESP_RST         4


// sonar variables
float sonar_result;                    // data
float sonar_tt;                        // time tag


// ir variables
IRsend irsend;               // Create irsend object


// ir codes
// ac on
unsigned int  rawData_ac_on[139] = {8900,4450, 650,1650, 600,500, 600,450, 650,1650, 550,1700, 600,500, 600,500, 550,550, 550,1650, 650,450, 650,450, 650,1600, 600,500, 650,450, 600,1650, 700,400, 650,1600, 650,1600, 600,1650, 650,1600, 650,450, 650,450, 600,1650, 650,450, 600,500, 600,1650, 650,450, 600,500, 700,400, 650,450, 650,450, 600,500, 600,500, 600,500, 600,500, 600,500, 650,1600, 600,1650, 600,1650, 600,1650, 600,500, 600,500, 600,500, 650,450, 650,450, 600,500, 600,500, 600,500, 650,1600, 600,1650, 600,1650, 600,1650, 600,500, 600,500, 650,450, 600,500, 600,500, 600,500, 600,500, 600,500, 600,500, 600,1650, 600,500, 650,1600, 600,1650, 600,500, 600,1650, 600,500, 550};  // NEC 9892F240
//unsigned int  data_ac_on = 0x9892F240;

// ac off
const unsigned int  rawData_ac_off [139] PROGMEM = {8900,4500, 550,1700, 600,500, 550,550, 550,550, 550,1700, 550,550, 550,550, 550,550, 550,1700, 550,550, 550,550, 550,1700, 550,1700, 550,550, 550,550, 500,600, 550,550, 550,1700, 500,1750, 550,550, 550,550, 550,550, 550,1700, 550,550, 550,550, 550,1700, 500,600, 550,550, 550,500, 550,550, 600,500, 550,600, 500,550, 550,550, 550,550, 550,550, 550,1700, 550,1700, 550,1700, 550,1700, 550,550, 550,600, 500,600, 500,550, 550,600, 500,550, 550,550, 550,550, 550,1700, 550,1700, 550,1700, 500,1750, 500,600, 500,600, 550,550, 550,550, 550,550, 550,550, 500,600, 500,600, 550,550, 550,1700, 550,550, 550,1700, 550,1700, 500,600, 550,1700, 550,550, 550};  // NEC 88986240
//unsigned int  data_ac_off = 0x88986240;
// irobot on and off
const unsigned int  rawData_irobot_tog[] PROGMEM = {8950,4400, 600,500, 550,550, 550,550, 550,550, 550,550, 550,550, 550,1700, 550,550, 550,1700, 550,550, 550,1650, 550,550, 550,1700, 550,550, 500,1700, 550,550, 550,550, 550,550, 550,1700, 550,550, 550,550, 550,550, 500,1700, 550,600, 550,1650, 500,1700, 550,550, 550,1700, 550,1650, 600,1650, 550,550, 500,1700, 550};  // NEC 2AA22DD
//unsigned int  data_irobot_tog = 0x2AA22DD;
// irobot home
const unsigned int  rawData_irobot_home[] PROGMEM = {8950,4450, 500,600, 500,600, 550,550, 600,500, 550,550, 550,550, 550,1700, 550,550, 500,1700, 550,550, 550,1700, 500,600, 500,1700, 550,550, 550,1700, 550,550, 550,1650, 550,550, 550,600, 550,550, 500,1700, 500,600, 500,600, 550,550, 550,550, 550,1700, 500,1700, 550,1700, 550,550, 500,1700, 550,1700, 500,1700, 550};  // NEC 2AA8877
//unsigned int  data_irobot_home = 0x2AA8877;


/*
long ir_rec_result;                    // data
float ir_rec_tt;                       // time tag
IRrecv irrecv(IR_RECV_PIN);           // create irrecv object
decode_results results_IR;            // declare variable to hold decoded IR receiver result
*/


// gps variables
long gps_result_lot;                 // data - lot
long gps_result_lng;                 // data - lng
float gps_tt;                         // time tag
const int GPSBaud PROGMEM = 9600;
TinyGPSPlus gps_tinyobj;                                      // Create a TinyGPS++ object
SoftwareSerial gpsSerial(GPS_TXPin, GPS_RXPin);               // Create a software serial port called "gpsSerial"


// Create CLI Object
SimpleCLI cli;


// Commands (empty object)
Command sonar;
//Command ir_rec;
//Command ir_tran;
Command ac;
Command irobot;
Command gps;
Command lcd;


/*
// ESP defenitions
SoftwareSerial softser(ARD_RX_ESP_TX, ARD_TX_ESP_RX);   // Arduino RX = ESP TX, Arduino TX = ESP RX
Adafruit_ESP8266 wifi(&softser, &Serial, ESP_RST);      // Must declare output stream before Adafruit_ESP8266 constructor; can be a SoftwareSerial stream, or Serial/Serial1/etc. for UART.
// Must call begin() on the stream(s) before using Adafruit_ESP8266 object.
#define HOST     "wifitest.adafruit.com"     // Host to contact
#define PAGE     "/testwifi/index.html" // Web page to request
#define PORT     80                     // 80 = HTTP default port
#define LED_PIN  13
// Replace with your network credentials and delete "#include "config.h" above
const char* ESP_SSID = WIFI_SSID;     // Your network name here
const char* ESP_PASS = WIFI_PASSWORD; // Your network pas here
*/


// time variables
int time_current = 0;


void setup()
{

    /**************************************************** setup serial ****************************************************/

    // Enable Serial connection at given baud rate
    Serial.begin(9600);
    // Wait a bit until device is started
    delay(2000);
    // check serial ok
    Serial.println(F("Beginning serial communication"));

    /**************************************************** setup CLI ****************************************************/
    // format: <command name> -<argument> <argument value / default value> <flag>

    /////////////////////// adding commands names to command objects

    // Create the sonar command and initialize sonar variable with it (see declarition above)
    sonar = cli.addCmd("sonar");
    // Create the ir command and initialize ir variable with it (see declarition above)
    //  ir_tran = cli.addCmd("irtran");
    ac = cli.addCmd("ac");
    irobot = cli.addCmd("irobot");
    // Create the gps command and initialize gps variable with it (see declarition above)
    gps = cli.addCmd("gps");
    // create lcd command and initialize gps variable with it (see declarition above)
    //lcd = cli.addSingleArgumentCommand("lcd");    // option 1: single argument command - we cannot add more arguments (we use only the function name)
    lcd = cli.addCmd("lcd");                        // option 2

    /////////////////////// adding arguments and default argument values

    // define arguments for sonar command
    // first elemtn in the brackets - name of the argument (the user must type the argument with "-"")
    // second elemtn in the brackets - argument value default (will be chosen if the user did not write anything). meaning "sensor type X number".
    // if we have only one sensor of this type, so it must be 1
    sonar.addArgument("calc", "1");

    // define arguments for ir_tra command
    //ir_tran.addArgument("ac", "off");
    //ir_tran.addArgument("irobot", "home");
    ac.addArgument("cmd", "off");         // argument = cmd, default argument value = off
    irobot.addArgument("cmd", "home");    // argument = cmd, default argument value = home

    // define arguments for gps command
    gps.addArgument("calc", "1");

    // define arguments for lcd command
    lcd.addArgument("display", "Hey You!");

    ///////////////////////  [Optional] Check if our command was successfully added

    if ((!sonar) || (!ac) || (!irobot) || (!gps) || (!lcd))
          Serial.println(F("Error - Something went wrong during creation of the commands"));
    else {
          Serial.println(F("Success - all commands were added to the CLI!"));
          Serial.println(F("Commmands available: sonar, ir, gps, lcd"));
          Serial.println(F("Format: <command> -<argument> <argument value>"));
          Serial.println(F("note: if defined a default argument value, write only the name of the command"));     
    }

    /**************************************************** setup sonar ****************************************************/
    pinMode(SONAR_TRIG_PIN, OUTPUT);   // Sets the trigPin as an Output
    pinMode(SONAR_ECHO_PIN, INPUT);    // Sets the echoPin as an Input

    /**************************************************** setup ir rec****************************************************/
    /*
    Serial.println("Enabling IR Receiver"); // Print text on serial monitor
    irrecv.enableIRIn();                    // Start the IR receiver - note it is an interrupt!
    Serial.println("Enabled IR Receiver"); // Print text on serial monitor
    */

    /**************************************************** setup ir trans**************************************************/
    //irsend.enableIROut(38);

    /**************************************************** setup GPS ******************************************************/
    // Start the software serial port at the GPS's default baud
    gpsSerial.begin(GPSBaud);

    // If 5000 milliseconds pass and there are no characters coming in
    // over the software serial port, show a "No GPS detected" error
    if (millis() > 5000 && gps_tinyobj.charsProcessed() < 10)
    {
      Serial.println(F("Error occured - No GPS signal (make sure is connected or the antenna is near the window"));
    }

    /**************************************************** setup ESP ******************************************************/
    /*  
    char buffer[50];
  
    // Flash LED on power-up
    pinMode(LED_PIN, OUTPUT);
    for(uint8_t i=0; i<3; i++) {
      digitalWrite(LED_PIN, HIGH); delay(50);
      digitalWrite(LED_PIN, LOW);  delay(100);
    }
  
    // This might work with other firmware versions (no guarantees) by providing a string to ID the tail end of the boot message:
    // comment/replace this if you are using something other than v 0.9.2.4!
    wifi.setBootMarker(F("Version:0.9.2.4]\r\n\r\nready"));
  
    softser.begin(9600); // Soft serial connection to ESP8266
    Serial.begin(57600); while(!Serial); // UART serial debug
  
    Serial.println(F("Adafruit ESP8266 Demo"));
  
    // Test if module is ready
    Serial.print(F("Hard reset..."));
    if(!wifi.hardReset()) {
      Serial.println(F("no response from module."));
      for(;;);
    }
    Serial.println(F("OK."));
  
    Serial.print(F("Soft reset..."));
    if(!wifi.softReset()) {
      Serial.println(F("no response from module."));
      for(;;);
    }
    Serial.println(F("OK."));
  
    Serial.print(F("Checking firmware version..."));
    wifi.println(F("AT+GMR"));
    if(wifi.readLine(buffer, sizeof(buffer))) {
      Serial.println(buffer);
      wifi.find(); // Discard the 'OK' that follows
    } else {
      Serial.println(F("error"));
    }
  
    Serial.print(F("Connecting to WiFi..."));
    if(wifi.connectToAP(F(ESP_SSID), F(ESP_PASS))) {
  
      // IP addr check isn't part of library yet, but
      // we can manually request and place in a string.
      Serial.print(F("OK\nChecking IP addr..."));
      wifi.println(F("AT+CIFSR"));
      if(wifi.readLine(buffer, sizeof(buffer))) {
        Serial.println(buffer);
        wifi.find(); // Discard the 'OK' that follows
  
        Serial.print(F("Connecting to host..."));
        if(wifi.connectTCP(F(HOST), PORT)) {
          Serial.print(F("OK\nRequesting page..."));
          if(wifi.requestURL(F(PAGE))) {
            Serial.println("OK\nSearching for string...");
            // Search for a phrase in the open stream.
            // Must be a flash-resident string (F()).
            if(wifi.find(F("working"), true)) {
              Serial.println(F("found!"));
            } else {
              Serial.println(F("not found."));
            }
          } else { // URL request failed
            Serial.println(F("error"));
          }
          wifi.closeTCP();
        } else { // TCP connect failed
          Serial.println(F("D'oh!"));
        }
      } else { // IP addr check failed
        Serial.println(F("error"));
      }
      wifi.closeAP();
    } else { // WiFi connection failed
      Serial.println(F("FAIL"));
    }

    */
}

void loop()
{

  // time calculation 
  time_current = millis() / 1000;  
     
  // Check if user typed something into the serial monitor (if so, enter the if)
  if (Serial.available())
  {
      // Read out string from the serial monitor (without using this line, we can not type something in)
      String input = Serial.readStringUntil('\n');

      // Echo the user input
      Serial.print("# ");
      Serial.println(input);

      // Parse the user input into the CLI
      cli.parse(input);
  }

  // Check for newly parsed commands were entered by the user
  // if true - we have a valid command in a que
  if (cli.available())
  {
      // Get command out of queue
      Command cmd = cli.getCmd();
      Serial.println();
      
      if (cmd == sonar)
      {
          sample_sonar();     //results will be stored in sonar_result and sonar_tt
          Argument arg_calc = cmd.getArgument("calc");          // get the argument
          String argVal_calc = arg_calc.getValue();             // get the argument's value (represents id of the sonar, in case we have more than one sonar)
          int argVal_calc_int = argVal_calc.toInt();      // convert to int

           // if the user wants to print more samples than the size of the array, he will get all the array
           if (argVal_calc_int == 1) {
            Serial.print(F("time of the sample:"));
            Serial.println(sonar_tt);
            Serial.print(F("distance:"));
            Serial.println(sonar_result);                      
           }
           else {
            Serial.println(F("There is only 1 sensor from this type, so your provided argument's value must be 1"));
           }
      }
      
      else if (cmd == ac)
      {       
          Argument arg_cmd = cmd.getArgument("cmd");            // get the argument
          String argVal_cmd = arg_cmd.getValue();               // get the argument's value

          if(argVal_cmd == "on") {
            ir_turn_on_ac();
          }
          else if (argVal_cmd == "off") {
            ir_turn_off_ac();
          }
      }
      
      else if (cmd == irobot)
      {
        Argument arg_cmd = cmd.getArgument("cmd");    // get the argument
        String argVal_cmd = arg_cmd.getValue();       // get the argument's value
        
        if (argVal_cmd == "tog") {
          ir_irobot_tog();
        }          
        else if (argVal_cmd == "home") {
          ir_irobot_home();
        }        
      }
      
      else if (cmd == gps)
      {
        sample_gps();                                         //results will be stored in gps_result_lat, gps_result_lng and gps_tt
        Argument arg_calc = cmd.getArgument("calc");          // get the argument
        String argVal_calc = arg_calc.getValue();             // get the argument's value (represents id of the sonar, in case we have more than one sonar)
        int argVal_calc_int = argVal_calc.toInt();            // convert to int

       // if the user wants to print more samples than the size of the array, he will get all the array
       if (argVal_calc_int == 1) {         
        Serial.print(F("time of the sample:"));
        Serial.println(gps_tt);
        gpsdisplayInfo();
       }
       else {
        Serial.println(F("There is only 1 sensor from this type, so your provided argument's value must be 1"));
       }
      }
      
      // todo: finish this section
      else if (cmd == lcd)
      {
          Argument arg = cmd.getArgument("display");      // get the argument
          String argVal_user_input = arg.getValue();          // get the argument's value

          Serial.print(F("I should print: "));
          Serial.println(argVal_user_input);
      }
  }


    // Check for parsing errors
    if (cli.errored())
    {
        // Get error out of queue
        CommandError cmdError = cli.getError();

        // Print the error
        Serial.print("ERROR: ");
        Serial.println(cmdError.toString());

        // Print correct command structure
        if (cmdError.hasCommand())
        {
            Serial.print("Did you mean \"");
            Serial.print(cmdError.getCommand().toString());
            Serial.println("\"?");
        }
    }
}


void sample_sonar() {
 
  // get results
  sonar_result = get_sonar_distance();    // sample sonar
  sonar_tt = time_current;                // time the sample was taken
}


int get_sonar_distance() {
 
  // defines variables
  long duration;
  int distance;
  
  digitalWrite(SONAR_TRIG_PIN, LOW);            // Clears the trigPin
  delayMicroseconds(2);
  
  digitalWrite(SONAR_TRIG_PIN, HIGH);          // Sets the trigPin on HIGH state for 10 micro seconds
  delayMicroseconds(10);
  digitalWrite(SONAR_TRIG_PIN, LOW);
  
  duration = pulseIn(SONAR_ECHO_PIN, HIGH);  // Reads the echoPin, returns the sound wave travel time in microseconds
  
  distance= duration*0.034/2;               // Calculating the distance in cm
  
  return distance;
}


void sample_gps() {

  TinyGPSPlus gps_obj = get_gps();

  // get results
  gps_result_lot = gps_obj.location.lat();    // gps lat
  gps_result_lng = gps_obj.location.lng();    // gps lng 
  gps_tt = time_current;            
}


TinyGPSPlus get_gps() {
    
  // This sketch displays information every time a new sentence is correctly encoded.
  while (gpsSerial.available() > 0)
  {
     if (gps_tinyobj.encode(gpsSerial.read())) {
//        Serial.println("TMP: ", gps_tinyobj.location);
      return gps_tinyobj;
     }
  }    
}


void gpsdisplayInfo()
{
  if (gps_tinyobj.location.isValid())
  {
    Serial.print("Latitude: ");
    Serial.println(gps_tinyobj.location.lat(), 6);
    Serial.print("Longitude: ");
    Serial.println(gps_tinyobj.location.lng(), 6);
    Serial.print("Altitude: ");
    Serial.println(gps_tinyobj.altitude.meters());
  }
  else
  {
    Serial.println("Location: Not Available");
  }
  
  Serial.print("Date: ");
  if (gps_tinyobj.date.isValid())
  {
    Serial.print(gps_tinyobj.date.month());
    Serial.print("/");
    Serial.print(gps_tinyobj.date.day());
    Serial.print("/");
    Serial.println(gps_tinyobj.date.year());
  }
  else
  {
    Serial.println("Not Available");
  }

  Serial.print("Time: ");
  if (gps_tinyobj.time.isValid())
  {
    if (gps_tinyobj.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps_tinyobj.time.hour());
    Serial.print(":");
    if (gps_tinyobj.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps_tinyobj.time.minute());
    Serial.print(":");
    if (gps_tinyobj.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps_tinyobj.time.second());
    Serial.print(".");
    if (gps_tinyobj.time.centisecond() < 10) Serial.print(F("0"));
    Serial.println(gps_tinyobj.time.centisecond());
  }
  else
  {
    Serial.print(F("Not Available "));
    Serial.println(F("- Please check if the antenna is near an open windows and the sky are clear"));
  }

  Serial.println();
  Serial.println();
  delay(1000);
}


/*
// if there is a new code, return it. if not, return 0
//(https://forum.arduino.cc/index.php?topic=40814.0)
long get_ir_code() {

  int i = 0;
  long code = 0;
  if (irrecv.decode(&results_IR)) // Checks the code received or not
  {
    code = results_IR.value;
    //Serial.println(results_IR.value, BIN); // Print the decoded value on serial monitor
//    Serial.println(code, BIN); // Print the decoded value on serial monitor
//    Serial.println(code, HEX); // Print the decoded value on serial monitor
//    Serial.println(code); // Print the decoded value on serial monitor
    irrecv.resume(); // continue to receive the next value
    return code;
  }

  return 0;
//  Serial.println("No IR code was detected");
}
*/


void ir_turn_on_ac() {
    // AC - on
    Serial.println(F("Turning on AC"));  
    irsend.sendRaw(rawData_ac_on, sizeof(rawData_ac_on) / sizeof(int),38 ); 
    delay(1500);
}

void ir_turn_off_ac() {
    // AC - off
    Serial.println(F("Turning off AC"));  
    irsend.sendRaw(rawData_ac_off, sizeof(rawData_ac_off) / sizeof(int),38 ); 
    delay(1500);
}


void ir_irobot_tog() {
   // IROBOT - on / off  - sometimes does not receive the message
   Serial.println(F("Tuggling IROBOT"));
   irsend.sendRaw(rawData_irobot_tog, sizeof(rawData_irobot_tog) / sizeof(int),38 ); 
   delay(1500);
}


void ir_irobot_home() {
   // IROBOT - home - sometimes does not receive the message
   Serial.println(F("Sending home IROBOT"));
 //  irsend.sendRaw(rawData_irobot_home, sizeof(rawData_irobot_home) / sizeof(int),38 ); 
   delay(3000);
}
