/*
Author              : Genady M (GitHub user name: gy-m)
Arduino IDE         : 1.8.12
GitHub Repository:  : https://github.com/gy-m/Arduino_Uno
==============================================================================
*/

// Inlcude Library - the path will be
#include <SimpleCLI.h>

// defines pins numbers
const int sonar_trigPin = 10;
const int sonar_echoPin = 11;

// sonar variables
#define SONAR_MAX_SAMPLES 3
float sonar_results[SONAR_MAX_SAMPLES][2] = {0};              // col[0] = sample number, col[1] = distance, number of rows = number of samples

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
    pinMode(sonar_trigPin, OUTPUT);   // Sets the trigPin as an Output
    pinMode(sonar_echoPin, INPUT);    // Sets the echoPin as an Input

}

void loop()
{

    sample_sonar(2, SONAR_MAX_SAMPLES);      // sample sonar every 2 seconds, number of samples stored is SONAR_MAX_SAMPLES 
     
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
             if (argVal_results_int > SONAR_MAX_SAMPLES)
                argVal_results_int = SONAR_MAX_SAMPLES;
                
              // print sonar results: number of rows = number of samples, col[0] = time of the sample, col[1] = distance
              Serial.println();
              for(int i=0; i<argVal_results_int; i++) {
                Serial.print("sample numner:");
                Serial.println(i);
                Serial.print("time of the sample:");
                Serial.println(sonar_results[i][0]);
                Serial.print("value of the sample:");
                Serial.println(sonar_results[i][1]);
              }
            
        }

        else if (cmd == ir)
        {
            Argument arg = cmd.getArgument("pop_results");      // get the argument
            String argVal_results = arg.getValue();             // get the argument's value
            int argVal_results_int = argVal_results.toInt();    // convert to int 
            
            for (int i = 0; i < argVal_results_int; i++)
            {
                Serial.print("ir result ");
                Serial.print(i);
                Serial.println(": YYY");
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

void sample_sonar(int sec_rate, int max_samples_stored) {

    int sonar_sample = 0;


    time_current = millis() / 1000;
    time_delta = time_current - time_prev_sample;

    if (time_delta > 2)
    {         
      sonar_results[sonar_sample][0] = time_current;                  // time the sample was taken
      sonar_results[sonar_sample][1] = get_sonar_distance();          // sample value
          
      sonar_sample++;
      if(sonar_sample>=SONAR_MAX_SAMPLES)
        sonar_sample=0;

      time_prev_sample = time_current;
      time_delta = 0;
    }
}


int get_sonar_distance() {
 
  // defines variables
  long duration;
  int distance;
  
  digitalWrite(sonar_trigPin, LOW);            // Clears the trigPin
  delayMicroseconds(2);
  
  digitalWrite(sonar_trigPin, HIGH);          // Sets the trigPin on HIGH state for 10 micro seconds
  delayMicroseconds(10);
  digitalWrite(sonar_trigPin, LOW);
  
  duration = pulseIn(sonar_echoPin, HIGH);  // Reads the echoPin, returns the sound wave travel time in microseconds
  
  distance= duration*0.034/2;               // Calculating the distance in cm
  
  // Prints the distance on the Serial Monitor
  //Serial.print("Distance cm: ");
  //Serial.println(distance);
  return distance;
  
}
