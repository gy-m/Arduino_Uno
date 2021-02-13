/*
Author              : Genady M (GitHub user name: gy-m)
Arduino IDE         : 1.8.12
GitHub Repository:  : https://github.com/gy-m/Arduino_Uno
==============================================================================
*/

#include <Adafruit_ESP8266.h>

#include <TinyGPS.h>

// Inlcude Library - the path will be 2 folders up
#include <SoftwareSerial.h>
#include <SimpleCLI.h>
//#include <IRremote.h>
#include <TinyGPS++.h>
#include <Adafruit_ESP8266.h>
#include <Wire.h>
#include <Adafruit_GFX.h>    // for TFT - Core graphics library
#include <MCUFRIEND_kbv.h>   // for TFT
#include <math.h>


// object for TFT
MCUFRIEND_kbv tft;


// TFT - Assign human-readable names to some common 16-bit color values:
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

// TFT - Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// Adafruit_TFTLCD tft;
uint16_t g_identifier;


// define slave address (another arduino)
const int SLAVE_NUM_ARD PROGMEM = 8;

// defines pins numbers
//int RECV_PIN = 12;                            // Declare variables to hold pin number
const int SONAR_TRIG_PIN PROGMEM = 12;
const int SONAR_ECHO_PIN PROGMEM = 5;
//const int GPS_TXPin PROGMEM = 2;                // pin of GPS is TX, pin of arduino is 2 (rx = arduino receive)
//const int GPS_RXPin PROGMEM = 4;                // pin of GPS is RX, pin of arduino is 3 (tx = arduino transfer) - do not forget resistors! pin 3 going to resistor first! but this pin is not mandatory!
//const int IR_TRAN_PIN = 3;                    // could not be defined, but in the source file. Here i only mention it
#define ARD_RX_ESP_TX   3
#define ARD_TX_ESP_RX   2
#define ESP_RST         4


// sonar variables
float sonar_result;                    // data
float sonar_tt;                        // time tag


// ir variables
//IRsend irsend;                          // Create irsend object

/*
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
*/

/*
  long ir_rec_result;                    // data
  float ir_rec_tt;                       // time tag
  IRrecv irrecv(IR_RECV_PIN);           // create irrecv object
  decode_results results_IR;            // declare variable to hold decoded IR receiver result
*/

/*
  // gps variables
  long gps_result_lot;                 // data - lot
  long gps_result_lng;                 // data - lng
  float gps_tt;                         // time tag
  const int GPSBaud PROGMEM = 9600;
  TinyGPSPlus gps_tinyobj;                                      // Create a TinyGPS++ object
  SoftwareSerial gpsSerial(GPS_TXPin, GPS_RXPin);               // Create a software serial port called "gpsSerial"
*/

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
Command led;



// ESP defenitions
//SoftwareSerial espSerial =  SoftwareSerial(2,3);                          // arduino 2 = TX of ESP  arduino 3 = RX of ESP
SoftwareSerial espSerial =  SoftwareSerial(ARD_RX_ESP_TX, ARD_TX_ESP_RX);   // arduino ARD_RX_ESP_TX = TX of ESP,  arduino 3 = RX of ESP


// time variables
int time_current = 0, time_delta = 5000, time_out;


void setup()
{
  uint32_t setup_time = millis();

  /**************************************************** setup serial ****************************************************/
  Serial.println(F("****************** setup serial communication ******************"));
  // Enable Serial connection at given baud rate
  Serial.begin(9600);
  // Wait a bit until device is started
  delay(2000);
  // check serial ok
  Serial.println(F("setup serial communication - done"));

  /**************************************************** setup master arduino *******************************************/
  Serial.println(F("****************** setup master arduino ******************"));
  Wire.begin(); // join to i2c bus as a master (address optional for master)
  Serial.println(F("setup master arduino - done"));

  /**************************************************** setup TFT ****************************************************/
  Serial.println(F("****************** setup TFT display ******************"));

  if (!Serial)
    delay(10);                        //allow some time for Leonardo
  static uint16_t identifier;
  //    tft.reset();                  //we can't read ID on 9341 until begin()
  g_identifier = tft.readID();
  Serial.print(F("ID = 0x"));
  Serial.println(g_identifier, HEX);
  tft.begin(0x8357);                  //to enable RM68140 driver code
  tft.setRotation(3);

  // final setups
  tft.fillScreen(BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(GREEN);
  tft.setTextSize(5);

  delay(250);
  Serial.println(F("setup TFT display - done"));
  /**************************************************** setup CLI ****************************************************/
  Serial.println(F("****************** setup CLI ******************"));

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

  // Create the led command and initialize led variable with it (see declarition above)
  led = cli.addCmd("led");

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

  // define arguments for led command
  led.addArgument("cmd", "off");

  ///////////////////////  [Optional] Check if our command was successfully added

  if ((!sonar) || (!ac) || (!irobot) || (!gps) || (!lcd) || (!led))
    Serial.println(F("Error - Something went wrong during creation of the commands"));
  else {
    Serial.println(F("all commands were added to the CLI"));
    Serial.println(F("Commmands available: sonar, ir, gps, lcd, led"));
    Serial.println(F("Format: <command> -<argument> <argument value>"));
    Serial.println(F("note: if defined a default argument value, write only the name of the command"));
  }

  Serial.println(F("setup CLI - done"));
  /**************************************************** setup sonar ****************************************************/
  Serial.println(F("****************** setup sonar  ******************"));

  pinMode(SONAR_TRIG_PIN, OUTPUT);   // Sets the trigPin as an Output
  pinMode(SONAR_ECHO_PIN, INPUT);    // Sets the echoPin as an Input

  Serial.println(F("setup sonar - done"));
  /**************************************************** setup ir rec****************************************************/
  /*
    Serial.println("Enabling IR Receiver"); // Print text on serial monitor
    irrecv.enableIRIn();                    // Start the IR receiver - note it is an interrupt!
    Serial.println("Enabled IR Receiver"); // Print text on serial monitor
  */

  /**************************************************** setup ir trans**************************************************/
  //irsend.enableIROut(38);

  /**************************************************** setup GPS ******************************************************/
  /*
    // Start the software serial port at the GPS's default baud
    gpsSerial.begin(GPSBaud);

    // If 5000 milliseconds pass and there are no characters coming in
    // over the software serial port, show a "No GPS detected" error
    if (millis() > 5000 && gps_tinyobj.charsProcessed() < 10)
    {
    Serial.println(F("Error occured - No GPS signal (make sure is connected or the antenna is near the window"));
    }
  */
  /**************************************************** setup ESP ******************************************************/
  Serial.println(F("****************** setup ESP8266-01  ******************"));
  espSerial.begin(9600);
  Serial.println(F("Local ESP is 192.168.43.240"));
  Serial.println(F("setup ESP - done"));
  //delay(1500);      // can delete it if i do not use ESP, but with ESP I need to take alittle spare

  /**********************************************************************************************************************/

  Serial.println("setup time took " + String((millis() - setup_time)) + "ms to start");
  Serial.println(F("****************** End of setup ******************\n"));

}

void loop()
{

  // we do not need to send any thing to esp

  /*
    // while serial is available, read the data from the serial and send to esp
    while (Serial.available() && time_current < time_out) {
      char ch = Serial.read();
      espSerial.print(ch);
    }
  */

  int ack_for_esp = 0;   // if true, we must send answer to the server

  // if esp is available eneter here instead of CLI
  if (espSerial.available()) {
    Serial.println("\n-------start streaming messages from the ESP server:------- \n");

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

      // analytizng the lines
      char line_prefix[esp_server_msg.length()] = {0};
      char line_suffix[esp_server_msg.length()] = {0};
      int prefix_size = 15;
      strncpy(line_prefix, esp_server_msg_chars, prefix_size);
      strcpy(line_suffix, esp_server_msg_chars + prefix_size);

      //      Serial.print("line prefix: ");
      //      Serial.println(line_prefix);
      //      Serial.print("line suffix: ");
      //      Serial.println(line_suffix);

      if (strcmp("line number 2: ", line_prefix) == 0) {
        cli.parse(line_suffix);
        ack_for_esp = 1;
      }
 
    }
    Serial.println("\n------------End of streaming from the ESP server------------");
  }



  // Check if user typed something into the serial monitor (if so, enter the if)
  else if (Serial.available())
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

        // send the info to display
        char char_arr_dist[4] = "";
        itoa(sonar_result, char_arr_dist, 10);      // convert distance from int to string
        strcat(char_arr_dist, "cm\n");
        char user[] = "Sonar";
        simple_text_display(user, char_arr_dist);

        // send message to ESP
        if (ack_for_esp) {
          espSerial.print(char_arr_dist);
          ack_for_esp = 0;
        }
      }
      else {
        Serial.println(F("There is only 1 sensor from this type, so your provided argument's value must be 1"));
      }
    }

    //      else if (cmd == ac || server_msg == "ac on" || server_msg == "ac off")
    else if (cmd == ac)
    {

      Argument arg_cmd = cmd.getArgument("cmd");            // get the argument
      String argVal_cmd = arg_cmd.getValue();               // get the argument's value

      //        if (argVal_cmd == "on" || server_msg == "ac on") {
      if (argVal_cmd == "on") {
        //slave_ir_turn_on_ac();
        char msg_to_slave[] = "ac_on";
        if (send_msg_slave(msg_to_slave, sizeof(msg_to_slave)) == 1) {
          Serial.println(F("The slave got the message. AC is ON"));        // note - we cannot be sure if AC is on because maybe we got ACK for a partial message

          // send the info to display
          // convert the desired text from String to char array
          char user[] = "AC";
          char msg_to_tft[] = "The slave got the message. AC is ON";
          simple_text_display(user, msg_to_tft);

          // send message to ESP
          if (ack_for_esp) {
            espSerial.print(msg_to_tft);
            ack_for_esp = 0;
          }
        }
        else {
          Serial.println(F("Something went wrong. Try to send again the message"));
          // send message to ESP
          if (ack_for_esp) {
            espSerial.print("Something went wrong. Try to send again the message");
            ack_for_esp = 0;
          }
        }
      }
      //        else if (argVal_cmd == "off" || server_msg == "ac off") {
      else if (argVal_cmd == "off") {
        //slave_ir_turn_off_ac();
        char msg_to_slave[] = "ac_off";
        if (send_msg_slave(msg_to_slave, sizeof(msg_to_slave))) {
          Serial.println(F("The slave got the message. AC is OFF"));        // note - we cannot be sure if AC is on because maybe we got ACK for a partial message

          // send the info to display
          // convert the desired text from String to char array
          char user[] = "AC";
          char msg_to_tft[] = "The slave got the message. AC is OFF";
          simple_text_display(user, msg_to_tft);

          // send message to ESP
          if (ack_for_esp) {
            espSerial.print(msg_to_tft);
            ack_for_esp = 0;
          }
        }
        else {
          Serial.println(F("Something went wrong. Try to send again the message"));
          if (ack_for_esp) {
            espSerial.print("Something went wrong. Try to send again the message\n");
            ack_for_esp = 0;
          }
        }
      }
    }

    /*
      else if (cmd == irobot)
      {
      Argument arg_cmd = cmd.getArgument("cmd");    // get the argument
      String argVal_cmd = arg_cmd.getValue();       // get the argument's value

      if (argVal_cmd == "tog") {
        //ir_irobot_tog();
        char msg_to_slave[] = "irobot_tog";
        if (send_msg_slave(msg_to_slave, sizeof(msg_to_slave))) {
          Serial.println("The slave got the message. IROBOT toggled");        // note - we cannot be sure if AC is on because maybe we got ACK for a partial message
        }
        else {
          Serial.println("Something went wrong. Try to send again the message");
        }
      }
      else if (argVal_cmd == "home") {
        //ir_irobot_home();
        char msg_to_slave[] = "irobot_home";
        if (send_msg_slave(msg_to_slave, sizeof(msg_to_slave))) {
          Serial.println("The slave got the message. IROBOT was sent home");        // note - we cannot be sure if AC is on because maybe we got ACK for a partial message
        }
        else {
          Serial.println("Something went wrong. Try to send again the message");
        }
      }
      }
    */
    /*
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
    */
    else if (cmd == lcd)
    {

      Argument arg = cmd.getArgument("display");          // get the argument
      String argVal_user_input = arg.getValue();          // get the argument's value

      Serial.print(F("message was sent to display: "));
      Serial.println(argVal_user_input);

      // convert the desired text from String to char array
      char user[] = "User";
      char char_arr[argVal_user_input.length() + 1];
      argVal_user_input.toCharArray(char_arr, argVal_user_input.length() + 1);
      simple_text_display(user, char_arr);


      if (ack_for_esp) {
        espSerial.print(char_arr);
        ack_for_esp = 0;
      }
    }
    
    else if (cmd == led)
    {

      Argument arg_cmd = cmd.getArgument("cmd");            // get the argument
      String argVal_cmd = arg_cmd.getValue();               // get the argument's value

      if (argVal_cmd == "on") {
        char msg_to_slave[] = "led_on";
        if (send_msg_slave(msg_to_slave, sizeof(msg_to_slave)) == 1) {
          Serial.println(F("The slave got the message. LED is ON"));        // note - we cannot be sure if AC is on because maybe we got ACK for a partial message

          // send the info to display
          // convert the desired text from String to char array
          char user[] = "LED";
          char msg_to_tft[] = "The slave got the message. LED is ON";
          simple_text_display(user, msg_to_tft);

          // send message to ESP
          if (ack_for_esp) {
            espSerial.print(msg_to_tft);
            ack_for_esp = 0;
          }
        }
        else {
          Serial.println(F("Something went wrong. Try to send again the message"));
          // send message to ESP
          if (ack_for_esp) {
            espSerial.print("Something went wrong. Try to send again the message");
            ack_for_esp = 0;
          }
        }
      }
      else if (argVal_cmd == "off") {
        char msg_to_slave[] = "led_off";
        if (send_msg_slave(msg_to_slave, sizeof(msg_to_slave))) {
          Serial.println(F("The slave got the message. LED is OFF"));        // note - we cannot be sure if AC is on because maybe we got ACK for a partial message

          // send the info to display
          // convert the desired text from String to char array
          char user[] = "LED";
          char msg_to_tft[] = "The slave got the message. LED is OFF";
          simple_text_display(user, msg_to_tft);

          // send message to ESP
          if (ack_for_esp) {
            espSerial.print(msg_to_tft);
            ack_for_esp = 0;
          }
        }
        else {
          Serial.println(F("Something went wrong. Try to send again the message"));
          if (ack_for_esp) {
            espSerial.print("Something went wrong. Try to send again the message\n");
            ack_for_esp = 0;
          }
        }
      }
    }

    //delay(1500);      // can delete it if i do not use ESP, but with ESP I need to take alittle spare
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

    if (ack_for_esp) {
      espSerial.print("Error - check the validity of your command");
      ack_for_esp = 0;
    }

  }
}


void simple_text_display(char* user, char* text) {
  tft.fillScreen(BLACK);
  tft.setTextColor(GREEN);
  tft.setCursor(0, 0);
  //   tft.print(F("user: "));
  tft.print(user);
  tft.print(":");
  tft.setTextColor(WHITE);
  tft.println(text);
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

  distance = duration * 0.034 / 2;          // Calculating the distance in cm

  return distance;
}

/*
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
*/

int send_msg_slave(char* msg_to_slave, int sz) {

  char msg_ack_from_slave[] = "ACK";    // a msg we suppose to get from the slave. otherwise, we did not get ack

  // information for the terminal
  Serial.print(F("number of bytes to send: "));
  Serial.println(sz);
  Serial.print(F("msg to send: "));
  Serial.println(msg_to_slave);

  // transmission
  Wire.beginTransmission(SLAVE_NUM_ARD);        // transmit to device #8
  Wire.write(msg_to_slave, sz); // send msg_to_send, and the amount of bytes
  Wire.endTransmission();                       // stop transmitting

  // request for reply
  Wire.requestFrom(SLAVE_NUM_ARD, 3);           // request 3 bytes from slave device #SLAVE_NUM_ARD
  Serial.println(F("Requesting from slave."));
  Serial.print(F("Slave: "));
  int i = 0;
  while (Wire.available()) {                    // slave may send less than requested
    char c = Wire.read();     // receive a byte as character
    Serial.print(c);         // print the character

    if (strcmp(c, msg_ack_from_slave[i]) == 0) {
      i++;
    }
    else {
      return 0;     // we did not get "ACK"
    }

  }

  Serial.println();

  if (i == sizeof(msg_ack_from_slave) - 1)
    return 1;        // we got the ack
  else {
    return 0;
  }
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

/*
  // if we do not use another arduino (but we must because the of the memory)
  void slave_ir_turn_on_ac() {

    // AC - on
    Serial.println(F("Turning on AC"));
    irsend.sendRaw(rawData_ac_on, sizeof(rawData_ac_on) / sizeof(int),38 );
    delay(1500);

  }
*/

/*
  // if we do not use another arduino (but we must because the of the memory)
  void slave_ir_turn_off_ac() {
    // AC - off
    Serial.println(F("Turning off AC"));
    irsend.sendRaw(rawData_ac_off, sizeof(rawData_ac_off) / sizeof(int),38 );
    delay(1500);

  }
*/


/*
  // if we do not use another arduino (but we must because the of the memory)
  void ir_irobot_tog() {
   // IROBOT - on / off  - sometimes does not receive the message
   Serial.println(F("Tuggling IROBOT"));
   irsend.sendRaw(rawData_irobot_tog, sizeof(rawData_irobot_tog) / sizeof(int),38 );
   delay(1500);
  }
*/


/*
  // if we do not use another arduino (but we must because the of the memory)
  void ir_irobot_home() {
   // IROBOT - home - sometimes does not receive the message
   Serial.println(F("Sending home IROBOT"));
  //  irsend.sendRaw(rawData_irobot_home, sizeof(rawData_irobot_home) / sizeof(int),38 );
   delay(3000);
  }
*/
