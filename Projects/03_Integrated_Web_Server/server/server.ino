/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com
*********/

// Load Wi-Fi library
#include "ESP8266WiFi.h"
#include "config.h"                 // additional configurations: Wifi SSID and PASSWORD


// Replace with your network credentials and delete "#include "config.h" above
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

// Set web server port number to 80
//WiFiServer server(80);
WiFiServer server(1025);       // i prefer to change it to be accesible from the internet, so i do not think 80 is a good choice


// Variable to store the HTTP request
String header;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {

  /*********************************** setup serial **********************************/
  Serial.println(F("****************** setup serial communication ******************"));
  //  Serial.begin(115200);
  Serial.begin(9600);

  /*********************************** setup ESP ************************************/
  Serial.println(F("****************** setup ESP ***********************************"));

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address (of myself - ESP) and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

  Serial.println(F("The setup is done"));
  Serial.println(F("*****************************************************************"));

}


void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients
  int row_num = 0;
  if (client) {                             // If a new client connects,

    Serial.println("New Client");          // print a message out in the serial port
    String incoming_row = "";              // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;

    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();

      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        //Serial.write(c);                    // print it out the serial monitor (meaning sending to arduino the request the ESP got)

        if (c == '\n') {                      // if the byte is a newline character
          if (incoming_row.length() == 0)    // // if the incoming_row is blank line (and before it was newline) -> we finish getting the headers of the client's request
          {
            // The Response to the client - Headers
            //client.println("HTTP/1.1 200 OK");
            //            client.println("Content-type: text");
            client.println();

            // The Response to the client - content

            // The HTML of ESP

            // Break out of the while loop
            break;
          }

          // if you got a newline, then init (clear) incoming_row (prepare to get more data for the next row)
          else
          {

            // send rows to arduino
            Serial.print("line number ");
            Serial.print(row_num);
            Serial.print(": ");
            Serial.println(incoming_row);

            /////////////////// section 1 - if i want only send all the messages to arduino, just comment section 1

            // check password - will be named as p
            if (row_num == 1) {
              char incoming_row_chars[incoming_row.length() + 1];
              incoming_row.toCharArray(incoming_row_chars, incoming_row.length() + 1);

//              if (strcmp("password: abc", incoming_row_chars) != 0) {
              if (strcmp("p: abc", incoming_row_chars) != 0) {
  
                Serial.print("line number ");
                Serial.print(row_num);
                Serial.print(": ");
                Serial.println("Invalid HTTP request sent to the ESP");
                delay(500); // !
                row_num++;
                break;
              }
            }

            // row 2 is the command row - so expect an answer
            if (row_num == 2) {
              delay(500);
              while (! (Serial.available() > 0)) { }
              String response = Serial.readStringUntil('\n');
              char response_chars[response.length() + 1];
              response.toCharArray(response_chars, response.length() + 1);

              Serial.print("Esp got the msg from arduino: ");
              Serial.println(response_chars);

              // send response from arduino to the outside client (django)
//              client.write(response_chars);
                client.println(response_chars);
//                client.println();
            }
            /////////////////// end of section 1


            // prepare for the next row
            incoming_row = "";

            row_num++;
          }
        }

        // if you got anything else but a carriage return character, add it to the end of the incoming_row (meaning we did not finish getting all the chars from the client)
        else if (c != '\r')
        {
          incoming_row += c;
        }
      }

    }

    // Clear the header variable
    header = "";

    // Close the connection
    /*
      Serial.print("line number ");
      Serial.print(row_num);
      Serial.print(": ");
    */
    Serial.println("Client disconnected.");
    Serial.println("");
    row_num = 0;

    client.stop();
    delay(500);
  }
}
