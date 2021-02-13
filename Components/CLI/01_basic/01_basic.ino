/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/SimpleCLI
 */

// Inlcude Library
#include <SimpleCLI.h>

// Create CLI Object
SimpleCLI cli;

// Commands (empty object)
Command ping;
Command pong;

void setup() {
    // Enable Serial connection at given baud rate
    Serial.begin(9600);

    // Wait a bit until device is started
    delay(2000);

    // Print something to let us know that everything is working so far
    Serial.println("Hello World!");

    // Create the ping command and initialize ping variable with it (see declarition above)
    ping = cli.addCmd("ping"); 
    // create pong command, but as a single argument command (for single argument command we cannot add more arguments)
    pong = cli.addSingleArgumentCommand("pong");
    
    // define arguments for ping command, so 
    // the user must type the argument num (-num)
    // if the user did not typed -num, the deafault action will be using this argument and with argument value 1
    ping.addArgument("num", "1");

    // [Optional] Check if our command was successfully added
    if (!ping) {
        Serial.println("Something went wrong :(");
    } else {
        Serial.println("Ping was added to the CLI!");
    }

    // Start the loop
    Serial.println("Type ping to see what happens!");
}

void loop() {
    // Check if user typed something into the serial monitor
    // -> the user types something in
    if (Serial.available()) {
        // Read out string from the serial monitor
        // without using this line, we can not type something in
        String input = Serial.readStringUntil('\n');

        // Echo the user input
        Serial.print("# ");
        Serial.println(input);

        // Parse the user input into the CLI
        cli.parse(input);
    }

    // Check for newly parsed commands
    // -> we have a command in a que
    if (cli.available()) {
        // Get command out of queue
        Command cmd = cli.getCmd();

        // React on our ping command
        // -> we have name of the command, an argument and an argument value
        // note - maybe the user did not enter the argument + argument value, but
        // we defined them as default
        if (cmd == ping) {

            // we will hold the argument and we refer it as "num"
             Argument argN = cmd.getArgument("num");
             // so we expecting after the argument (-num) to get an argument value
             String argVal = argN.getValue();
             int n = argVal.toInt(); 

             for(int i=0; i<n; i++) 
                Serial.println("Pong!");
        }

        // the user used pong command
        else if (cmd == pong) {
            // we defined pong as a command with a single argument,
            // so every thing we write after this command, will be treated as a 
            // single argument if we. if we did not do it, it will try to parse it
            Argument arg = cmd.getArgument();
            Serial.println(arg.getValue());   
        }
    }

    // Check for parsing errors
    if (cli.errored()) {
        // Get error out of queue
        CommandError cmdError = cli.getError();

        // Print the error
        Serial.print("ERROR: ");
        Serial.println(cmdError.toString());

        // Print correct command structure
        if (cmdError.hasCommand()) {
            Serial.print("Did you mean \"");
            Serial.print(cmdError.getCommand().toString());
            Serial.println("\"?");
        }
    }
}
