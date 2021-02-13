/*
Author              : Genady M (GitHub user name: gy-m)
Arduino IDE         : 1.8.12
GitHub Repository:  : https://github.com/gy-m/Arduino_Uno
==============================================================================
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

int rec_event_number = 0;

void setup() {
  ////////////////////// set communication /////////////////
  Serial.begin(9600);           // start serial for output
  
  ////////////////////// set receiver //////////////////////
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event for receiving from master
  Wire.onRequest(requestEvent); // register event for sending to master when he requesting for data

  ////////////////////// set LCDI2C //////////////////////
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  lcd.clear();
  lcd.print("Hello, world!");
}

void loop() {
  // to be on the safe side
  delay(500);
}

// whenever data is received from master execute this function (it is registered as an eventin  setup())
void receiveEvent(int size_bytes) {
  
  Serial.print("receiving event number : ");
  Serial.print(rec_event_number);
  Serial.print(" , number of bytes: ");
  Serial.println(size_bytes);
  Serial.print("msg received: ");
  // start receiving by byte
  
  while (Wire.available()) {      
    char c = Wire.read();         // receive byte as a character
    Serial.print(c);              // print the character
  }
  
  Serial.println();
  Serial.println("End of receiving");

  // indication on LCDI2C - DOES NOT WORK. I SHOULD LEARN HOW TO CONFIG LCDI2C WITH ARDUINO I2C
  /*
  lcd.clear();
  lcd.print("received msg num:");
  lcd.setCursor(0,1);
  lcd.print(rec_event_number);
  */
  
  rec_event_number++;
  return;
}

//  whenever data is requested by master execute this function (it is registered as an eventin  setup())
void requestEvent() {
  // I implemented simple ack, without knowing (or letting master to know) if actually i got the right message
  char msg_to_master[] = {"ACK"};
  // note - the number of bytes we can write to master must be as he expected
  Serial.print("Sending to master: ");
  Serial.println(msg_to_master);
  Serial.println();
  Wire.write(msg_to_master, sizeof(msg_to_master));    // respond with message of 3 bytes
}
