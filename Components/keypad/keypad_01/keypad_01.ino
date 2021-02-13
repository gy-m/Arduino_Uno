/* Source || @file CustomKeypad.pde|| @version 1.0|| @author Alexander Brevig|| @contact alexanderbrevig@gmail.coms*/
/* #####  4x4 Membrane Keypad Arduino Interfacing  #####Arduino and Keypad ConnectionKeypad Pin    Arduino Pin   1      =>  Digital Pin 2   2      =>  Digital Pin 3   3      =>  Digital Pin 4   4      =>  Digital Pin 5   5      =>  Digital Pin 6   6      =>  Digital Pin 7   7      =>  Digital Pin 8   8      =>  Digital Pin 9*/

// note: tools - processor - pick the old boot loader for Atmega328P 

#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
// Define the Keymap
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the Rows of the keypad pin 8, 7, 6, 5 respectively
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the Columns of the keypad pin 4, 3, 2, 1 respectively

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  Serial.begin(9600);
}
  
void loop(){
 
  char customKey = customKeypad.getKey();
  
  if (customKey){
    Serial.println(customKey); // Send the pressed key value to the arduino serial monitor
  }
  
}

/*
void setup(){
  Serial.begin(9600);
}

void loop(){
  Serial.println("Hello");
  delay(1000);
}
*/
