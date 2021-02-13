#include <NTPClient.h>              // for calculating time
#include <WiFiUdp.h>                // for calculating UDP time
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include "config.h"                 // additional configurations: Wifi SSID and PASSWORD

// Replace with your network credentials and delete "#include "config.h" above
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

// Set web server port number to 80
WiFiServer server(80);

// Variables for getting UDP and local time
WiFiUDP ntpUDP;                           // Define NTP Client to get time
const long utcOffsetInSeconds = 7200;     // GMT + 2 (3600 * 2)
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup() {
  
  Serial.println(F("****************** setup serial communication ******************"));
  //Serial.begin(115200);
  Serial.begin(9600);

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
  WiFiClient client = server.available();           // Listen for incoming clients
  String client_http_respond;                       // Variable to store the HTTP from the client (when he clicked submit)
  
  if (client) {                                     // If a new client connects  
    client_http_respond = get_client_data(client);  // if there was a timeout, it will get only a message "ERROR - Timeout."
    respond_to_client(client);
    close_connection(client);
    send_data_to_arduino(client_http_respond);
  }

  // "BOOM" message will be sent every day on 7:30 except Saturday (if min are less than 10, they suppose to be with one digit)
  send_boom("Saturday", 19, 4);
}


String get_client_data(WiFiClient client) {

    unsigned long currentTime = millis();
    unsigned long previousTime = currentTime;
    const long timeoutTime = 2000;              //  Define timeout time in milliseconds (example: 2000ms = 2s)
    String client_http_req_header;              // Variable to store the HTTP request
    String client_http_data;                    // Variable to store the HTTP request
    String currentLine = "";                    // make a String to hold incoming data from the client
    bool finish_headers = false;

    // printing headline to the terminal with time tag
    Serial.print("#################### ");
    Serial.print("New Client: ");               // print a message out in the serial port
    timeClient.update();
    Serial.print(daysOfTheWeek[timeClient.getDay()]);
    Serial.print(", ");
    Serial.print(timeClient.getHours());
    Serial.print(":");
    Serial.print(timeClient.getMinutes());
    Serial.println(" ####################");
    Serial.println();
    
    // loop while the client's connected - Read from the cleint and respond to the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) { 
      currentTime = millis();

      // if there's bytes to read from the client
      if (client.available()) {                       
        char client_byte = client.read();             // read a byte from the client
        client_http_req_header += client_byte;        // add the byte to the client's header
        //Serial.write(client_byte);                  // print it out the serial monitor, meaning sending to arduino the request the ESP got (raw, so we use Serial.write

        // new char - if we did not get a new line / carriage return character, add it to the end of the currentLine
        if (client_byte != '\n' && client_byte != '\r') {  
          currentLine += client_byte;      
        }

        // new row - if we did get a new line but after it we have more data (we did not finish getting the headers of the client's request)
        // so we need to init currentLine for getting a new row the nexe byte
        else if (client_byte == '\n' && currentLine.length() != 0) {
          currentLine = "";
        }
        
        // end of the header - if the byte is a newline character and the current line is empty -> we finish getting the headers of the client's request
        else if (client_byte == '\n' && currentLine.length() == 0) {
           Serial.println("*** Client's Headers:");
           Serial.println();
           Serial.println(client_http_req_header);
           Serial.println("*** End of Client's Headers.");
           Serial.println();               
           finish_headers = true;
        }
        if (finish_headers) {
          // if we got here, meaning the client have data also after the headers
          client_http_data += client_byte;
        }
      }
    }

    if (currentTime - previousTime <= timeoutTime) {
        // will get here after timeoutTime
        Serial.println("ERROR - Timeout.");
        return "ERROR - Timeout.";  
    }
    
    else {
        Serial.println();
        Serial.println("*** Client's Data:");
        Serial.println(client_http_data);
        Serial.println();
        Serial.println("*** End of Client's Data.");
        Serial.println();
        return client_http_data;
    }
      return client_http_data;
}


void close_connection(WiFiClient client) {
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
}

void respond_to_client(WiFiClient client) {
  Serial.println("Sending response to the client.");

  // The Response to the client - Headers
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type: text");
  client.println("Connection: close");
  client.println();
  
  // The Response to the client - content (The HTML of ESP - Display the HTML web page)
  send_to_client_html(client);
}

void send_data_to_arduino(String client_http_respond) {
  int tv = 0;
  int ac = 0;
  int irobot = 0;
  int semi_index = client_http_respond.indexOf(":", 0);

  if (client_http_respond.substring(semi_index+1, semi_index+2) == "f") {
    tv = 0;
  }
  else {
    tv = 1;
  }

  semi_index = client_http_respond.indexOf(":", semi_index+1);
  if (client_http_respond.substring(semi_index+1, semi_index+2) == "f") {
    ac = 0;
  }
  else {
    ac = 1;
  }

  semi_index = client_http_respond.indexOf(":", semi_index+1);
  if (client_http_respond.substring(semi_index+1, semi_index+2) == "f") {
    irobot = 0;
  }
  else {
    irobot = 1;
  }

  Serial.println("$$$ Start of transmition to Arduino");
  timeClient.update();
  Serial.print("DAY: ");
  Serial.println(daysOfTheWeek[timeClient.getDay()]);
  Serial.print("HR: ");
  Serial.println(timeClient.getHours());
  Serial.print("MIN: ");
  Serial.println(timeClient.getMinutes());
  
  Serial.print("TV: ");
  Serial.println(tv);
  Serial.print("AC: ");
  Serial.println(ac);
  Serial.print("IROBOT: ");
  Serial.println(irobot);
  Serial.println("$$$ End of transmition to Arduino");

  /*
    timeClient.update();
    Serial.println("Start of transmition to Arduino");
    Serial.println("TOD:");
    Serial.print(daysOfTheWeek[timeClient.getDay()]);
    Serial.print(", ");
    Serial.print(timeClient.getHours());
    Serial.print(":");
    Serial.println(timeClient.getMinutes());
    Serial.println("End of TOD");
    Serial.println("Start Status:");
    Serial.println(client_http_respond);                  // print it out the serial monitor, meaning sending to arduino (raw)
    Serial.println("");
    Serial.println("End Status");
    Serial.println();
    Serial.println("End of transmition to Arduino");
    */
}

void send_boom(char* un_required_day, int required_hr, int required_min) {
  timeClient.update();

  // converting String to chars for using cmp func
  String week_day = daysOfTheWeek[timeClient.getDay()];
  char week_day_chars[week_day.length()];
  week_day.toCharArray(week_day_chars, week_day.length()+1);

  if (strcmp(week_day_chars, un_required_day) != 0 && timeClient.getHours() == required_hr && timeClient.getMinutes() == required_min) {
    // send the magic word "BOOM" and go to sleep for 60 sec
    Serial.println("$$$ Start of transmition to Arduino");
    Serial.println("BOOM");
    Serial.println("$$$ End of transmition to Arduino");
    delay(60000);           
  }
  
}

void send_to_client_style(WiFiClient client) {

/*
 <style>
    html {
        font-family: Helvetica;
        color: #335ada;
        display: inline-block;
        margin: 0px auto;
        text-align: center;
    }

    button {
        background-color: grey;
        border: none;
        color: white;
        padding: 15px 32px;
        text-align: center;
        text-decoration: none;
        display: inline-block;
        font-size: 14px;
        margin: 4px 2px;
        cursor: pointer;
        width: 200px;
        font-family: Helvetica;
        font-size: 18px
    }

    label_submit {
        font-family: Helvetica;
        font-size: 20px
    }

    submit {
        background-color: #af4c9c;
        width: 300px;
        text-decoration: none;
        font-size: 30px;
        margin: 2px;
        cursor: pointer;
    }

    p.p_response {
        text-align: left;
        color: black;
        text-decoration: underline;
    }
</style>
 */

 
  client.println("<style>");
  client.println("html {font-family: Helvetica;color: #335ada;display: inline-block;margin: 0px auto;text-align: center;}");
  client.println("button {background-color: grey;border: none;color: white;padding: 15px 32px;text-align: center;text-decoration: none;display: inline-block;font-size: 14px;margin: 4px 2px;cursor: pointer;width: 200px;font-family: Helvetica;font-size: 18px}");
  client.println("label_submit {font-family: Helvetica;font-size: 20px}");
  client.println("submit {background-color: #af4c9c;width: 300px;text-decoration: none;font-size: 30px;margin: 2px;cursor: pointer;}");
  client.println("p.p_response {text-align: left;color: black;text-decoration: underline;");
  client.println("</style>");
}

void send_to_client_html(WiFiClient client) {
  
//    String myVar = "My Var";

/*
  <!DOCTYPE html>
  <html>
  
  <head>
      <meta name="viewport" content="width=device-width, initial-scale=1">
  </head>
 */
    
    client.println("<!DOCTYPE html><html>");
    client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head>");
    send_to_client_style(client);

/*
   <body>
      <h1>My Smart House</h1>
      <p style="color: black;">Based on ESP8266 Web Server</p><br>
      <form style="text-align: left;">
          <button type="button" class="button" id = "TV_button" onClick="updateUnit('TV_status', 'TV_button')">TV <a id="TV_status"> NULL</a></button><br />
          <button type="button" class="button" id = "AC_button" onClick="updateUnit('AC_status', 'AC_button')">AC <a id="AC_status"> NULL</a></button><br />
          <button type="button" class="button" id = "IROBOT_button" onClick="updateUnit('IROBOT_status', 'IROBOT_button')">IROBOT <a id="IROBOT_status"> NULL</a></button><br />
          <br>
          <input type="button" class="button button_submit" id = "submmit_button" value="Submit" onclick="sendState()"/>
      </form>
      <br>
      <p class="p_response" >Response from the server: </p>
      <p id="response" style="text-align: left;">N/A</p>
  </body>
   */
   
    client.println("<body>");
    client.println("<h1>My Smart House</h1>");
    client.println("<p style=\"color: black;\">Based on ESP8266 Web Server</p><br>");
    client.println("<form method=\"post\" style=\"text-align: left;\"onSubmit=\"JavaScript:SendMyData()\">");
    client.println("<button method=\"post\" type=\"button\" class=\"button\" id = \"TV_button\" onClick=\"updateUnit('TV_status', 'TV_button')\">TV <a id=\"TV_status\"> NULL</a></button><br />");
    client.println("<button type=\"button\" class=\"button\" id = \"AC_button\" onClick=\"updateUnit('AC_status', 'AC_button')\">AC <a id=\"AC_status\"> NULL</a></button><br />");
    client.println("<button type=\"button\" class=\"button\" id = \"IROBOT_button\" onClick=\"updateUnit('IROBOT_status', 'IROBOT_button')\">IROBOT <a id=\"IROBOT_status\"> NULL</a></button><br />");
    client.println("<br>");
    client.println("<input type=\"button\" class=\"button button_submit\" id = \"submmit_button\" value=\"Submit\" onclick=\"sendState()\"/>");
    client.println("</form>");
    client.println("<br>");
    client.println("<p class=\"p_response\" >Message status to the server: </p>");
    client.println("<p id=\"response\" style=\"text-align: left;\">N/A</p>");
    client.println("</body>");
   
 /*
    
  <script>
      var units = { "TV_status": false, "AC_status": false, "IROBOT_status": false };
      function updateUnit(unit, unit_button) {
          units[unit] = !units[unit];
          if (units[unit] == false) {
              document.getElementById(unit_button).style.backgroundColor = "grey";
              document.getElementById(unit).innerHTML = "OFF";
          }
          else {
              document.getElementById(unit_button).style.backgroundColor = "green";
              document.getElementById(unit).innerHTML = "ON";
          }
      }
      function sendState() {
        var xmlHttp = new XMLHttpRequest();
        xmlHttp.onreadystatechange = function() { 
            if (this.readyState == 4 && this.status == 200)
              document.getElementById("response").innerHTML = "Message was sent successfully";
            else
              document.getElementById("response").innerHTML = "Sending message...";
            }
        xmlHttp.open("POST", "https://httpbin.org/post", true); // true for asynchronous 
        xmlHttp.send(JSON.stringify(units)); 
    }
  </script>
  
  </html>
*/

  client.println("<script>");
  client.println("var units = { \"TV_status\": false, \"AC_status\": false, \"IROBOT_status\": false };");
  client.println("function SendMyData() {");
  client.println("alert(\"HELLO\");");
  //client.println("var xhttp = new XMLHttpRequest();xhttp.onreadystatechange = function() {if (this.readyState == 4 && this.status == 200) {document.getElementById(\"demo\").innerHTML = this.responseText;}};xhttp.open(\"POST\", units[\"TV\"], true);xhttp.send(\"HELLO\");");
  //client.println("var url = \"http://192.168.1.100/\";var method = \"POST\";var postData = \"Some data\";var request = new XMLHttpRequest();request.onload = function () {var status = request.status; var data = request.responseText; }request.open(method, url, true);request.setRequestHeader(\"Content-Type\", \"application/json;charset=UTF-8\");request.send("Hello");");
  client.println("}");
  client.println("function updateUnit(unit, unit_button) {");
  client.println("units[unit] = !units[unit];");
  client.println("if (units[unit] == false) {document.getElementById(unit_button).style.backgroundColor = \"grey\";document.getElementById(unit).innerHTML = \"OFF\";}");
  client.println("else {document.getElementById(unit_button).style.backgroundColor = \"green\";document.getElementById(unit).innerHTML = \"ON\";}");
  client.println("}");
  client.println("function sendState() {");
  client.println("var xmlHttp = new XMLHttpRequest();");
  client.println("xmlHttp.onreadystatechange = function() { ");
  client.println("if (this.readyState == 4 && this.status == 200)");
  client.println("document.getElementById(\"response\").innerHTML = \"Message was sent successfully\";");
  client.println("else");
  client.println("document.getElementById(\"response\").innerHTML = \"Sending message...\";");
  client.println("}");
  client.println("xmlHttp.open(\"POST\", \"http://192.168.1.102\", true); // true for asynchronous");    // sending data to my self, because I want to esp get the state of the components from the post request (there is no other way to get it from the java script that runns in the client). Then show it in the serial Monitor
  client.println("xmlHttp.send(JSON.stringify(units)); ");
  client.println("}");
  client.println("");
  client.println("</script>");
  client.println("</html>");
}
