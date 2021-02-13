// Inlcude Library - the path will be
#include <SimpleCLI.h>

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
}

void loop()
{
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

            for (int i = 0; i < argVal_results_int; i++)
            {
                Serial.print("sonar result ");
                Serial.print(i);
                Serial.println(": XXX");
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
