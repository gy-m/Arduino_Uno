// Inlcude Library - the path will be 2 folders up
#include <SimpleCLI.h>
#include <IRremote.h>


// defines pins numbers
const int SONAR_TRIG_PIN = 10;
const int SONAR_ECHO_PIN = 11;
const int IR_RECV_PIN = 12;            // Declare variables to hold pin number

// defining max samples for all sensors (except ir)
#define MAX_SAMPLES 3

// sonar variables
//#define SONAR_MAX_SAMPLES 3
float sonar_results[MAX_SAMPLES][2] = {0};              // col[0] = sample number, col[1] = distance, number of rows = number of samples
int sample_sonar_index = 0;


// ir variables
//#define IR_MAX_SAMPLES 3
long ir_results[MAX_SAMPLES][2] = {0};              // col[0] = sample number, col[1] = data, number of rows = number of samples
int sample_ir_index = 0;
IRrecv irrecv(IR_RECV_PIN);                          // create irrecv object
decode_results results_IR;                          // declare variable to hold decoded IR receiver result


// time variables
int time_delta = 0, time_current = 0, time_prev_sample = 0;

// Create CLI Object
SimpleCLI cli;

// Commands (empty object)
Command sonar;
Command ir;
Command gps;
Command lcd;

void setup()
{

    /**************************************************** setup serial ****************************************************/

    // Enable Serial connection at given baud rate
    Serial.begin(9600);
    // Wait a bit until device is started
    delay(2000);
    // check serial ok
    Serial.println("Beginning serial communication");

    /**************************************************** setup CLI ****************************************************/
    // format: <command name> -<argument> <argument value / default value> <flag>

    /////////////////////// adding commands names to command objects

    // Create the sonar command and initialize sonar variable with it (see declarition above)
    sonar = cli.addCmd("sonar");
    // Create the ir command and initialize ir variable with it (see declarition above)
    ir = cli.addCmd("ir");
    // Create the gps command and initialize gps variable with it (see declarition above)
    gps = cli.addCmd("gps");
    // create lcd command and initialize gps variable with it (see declarition above)
    //lcd = cli.addSingleArgumentCommand("lcd");    // option 1: single argument command - we cannot add more arguments (we use only the function name)
    lcd = cli.addCmd("lcd"); // option 2

    /////////////////////// adding arguments and default argument values

    // define arguments for sonar command
    // first elemtn in the brackets - name of the argument (the user must type the argument with "-"")
    // second elemtn in the brackets - argument value default (will be chosen if theuser did not write anything)
    sonar.addArgument("pop_results", "1");

    // define arguments for ir command
    ir.addArgument("pop_results", "1");

    // define arguments for gps command
    gps.addArgument("pop_results", "1");

    // define arguments for lcd command
    lcd.addArgument("display", "Hey You!");

    ///////////////////////  [Optional] Check if our command was successfully added

    if ((!sonar) || (!ir) || (!gps) || (!lcd))
          Serial.println("Error - Something went wrong during creation of the commands");
    else {
          Serial.println("Success - all commands were added to the CLI!");
          Serial.println("Commmands available: sonar, ir, gps, lcd");
          Serial.println("Format: <command> -<argument> <argument value>");
          Serial.println("note: if defined a default argument value, write only the name of the command");     
    }

    /**************************************************** setup sonar ****************************************************/
    pinMode(SONAR_TRIG_PIN, OUTPUT);   // Sets the trigPin as an Output
    pinMode(SONAR_ECHO_PIN, INPUT);    // Sets the echoPin as an Input

    /**************************************************** setup ir rec****************************************************/

    irrecv.enableIRIn();                    // Start the IR receiver - note it is an interrupt!

}

void loop()
{

      int sample_rate = 2;    // sec

      // samples semsors: (no ir sensor because ir will be sampled using interrupt routine)
      sample_sensors(sample_rate, MAX_SAMPLES);

     
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

        if (cmd == sonar)
        {
            Argument arg = cmd.getArgument("pop_results");      // get the argument
            String argVal_results = arg.getValue();             // get the argument's value
            int argVal_results_int = argVal_results.toInt();    // convert to int

             // if the user wants to print more samples than the size of the array, he will get all the array
             if (argVal_results_int > MAX_SAMPLES)
                argVal_results_int = MAX_SAMPLES;
                
              // print sonar results: number of rows = number of samples, col[0] = time of the sample, col[1] = distance
              Serial.println();
              for(int i=0; i<argVal_results_int; i++) {
                Serial.print("sample numner:");
                Serial.println(i);
                Serial.print("time of the sample:");
                Serial.println(sonar_results[i][0]);
                Serial.print("distance:");
                Serial.println(sonar_results[i][1]);
              }
        }
 
        else if (cmd == ir)
        {
            Argument arg = cmd.getArgument("pop_results");      // get the argument
            String argVal_results = arg.getValue();             // get the argument's value
            int argVal_results_int = argVal_results.toInt();    // convert to int 

              // print sonar results: number of rows = number of samples, col[0] = time of the sample, col[1] = distance
              Serial.println();
              for(int i=0; i<argVal_results_int; i++) {
                Serial.print("sample numner:");
                Serial.println(i);
                Serial.print("time of the sample:");
                Serial.println(ir_results[i][0]);
                Serial.print("ir code:");
                Serial.println(ir_results[i][1]);
              }
        }
        
        

        else if (cmd == gps)
        {
            Argument arg = cmd.getArgument("pop_results");      // get the argument
            String argVal_results = arg.getValue();             // get the argument's value
            int argVal_results_int = argVal_results.toInt();    // convert to int 

            for (int i = 0; i < argVal_results_int; i++)
            {
                Serial.print("gps result ");
                Serial.print(i);
                Serial.println(": ZZZ");
            }
        }

        else if (cmd == lcd)
        {

            Argument arg = cmd.getArgument("display");      // get the argument
            String argVal_user_input = arg.getValue();          // get the argument's value

            Serial.print("I should print: ");
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

void sample_sensors(int sec_rate, int max_samples_stored) {
   
    // time calculation 
    time_current = millis() / 1000;
    time_delta = time_current - time_prev_sample;

    // sensor will be sampled according to sec_rate
    if (time_delta > 2)
    {         
      sonar_results[sample_sonar_index][0] = time_current;            // time the sample was taken
      sonar_results[sample_sonar_index][1] = get_sonar_distance();    // sample sonar
      ir_results[sample_ir_index][0] = time_current;                  // time the sample was taken
      ir_results[sample_ir_index][1] = get_ir_code();                 // sample ir - if there was not new sample, we will get 0

      // making sure we have enough space in the array
      sample_sonar_index++;
      if(sample_sonar_index>=max_samples_stored)
        sample_sonar_index=0;

      // making sure we have enough space in the array and add one only if there was a new sample
      if(ir_results[sample_ir_index][1] != 0) {
        sample_ir_index++;
        if(sample_ir_index>=max_samples_stored)
          sample_ir_index=0;
      }
      
      time_prev_sample = time_current;
      time_delta = 0;
    }
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
  
  // Prints the distance on the Serial Monitor
  //Serial.print("Distance cm: ");
  //Serial.println(distance);
  return distance;
  
}

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
