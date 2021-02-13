#include <IRremote.h>

int RECV_PIN = 10;            // Declare variables to hold pin number
IRrecv irrecv(RECV_PIN);      // Create irrecv object
decode_results results_IR;   // declare variable to hold decoded IR receiver result



// include - For TFT
#include <Adafruit_GFX.h>    // Core graphics library
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;

// include - For LiqidCrystal
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//***********************************************//
// If you use OPEN-SMART TFT breakout board                 
// Reconmmend you to add 5V to 3.3V level converting circuit.
// or you can use OPEN-SMART UNO Black version with 5V/3.3V power switch,
// you just need switch to 3.3V. (This is what I did)
// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
//----------------------------------------|
// TFT Breakout  -- Arduino UNO / Mega2560 / OPEN-SMART UNO Black
// GND                -- GND
// 3V3                -- 3.3V
// CS                 -- A3
// RS                 -- A2
// WR                 -- A1
// RD                 -- A0
// RST                -- RESET
// LED                -- GND
// DB0                -- 8
// DB1                -- 9
// DB2                -- 10
// DB3                -- 11
// DB4                -- 4
// DB5                -- 13
// DB6                -- 6
// DB7                -- 7

// Assign human-readable names to some common 16-bit color values:
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

//Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// Adafruit_TFTLCD tft;
uint16_t g_identifier;        // ???

void setup(void) {

   ///////////////////////////////////////
   //////////// setup for TFT ////////////
   //////////////////////////////////////
  Serial.begin(9600);
  Serial.println(F("TFT LCD test"));


  #ifdef USE_OPENSMART_SHIELD_PINOUT
    Serial.println(F("Using OPENSMART 3.2\" TFT Arduino Shield Pinout"));
  #else
    Serial.println(F("Using OPENSMART 3.2\" TFT Breakout Board Pinout"));
  #endif

  Serial.print("TFT size is "); Serial.print(tft.width()); Serial.print("x"); Serial.println(tft.height());

  uint32_t when = millis();
  if (!Serial) delay(10);             //allow some time for Leonardo
  Serial.println("Serial took " + String((millis() - when)) + "ms to start");
  static uint16_t identifier;
  //    tft.reset();                  //we can't read ID on 9341 until begin()
  g_identifier = tft.readID(); //
  Serial.print("ID = 0x");
  Serial.println(g_identifier, HEX);
  tft.begin(0x8357);//to enable RM68140 driver code
  tft.setRotation(3);

  // final setups
  tft.fillScreen(BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(WHITE);
  tft.setTextSize(5);
    
  Serial.println(F("TFT setup is done"));
  tft.print("Hello SHANY!");
  delay(1000);

  //////////////////////////////////////////////////////
  ////////////  setup for LiquidCrystal LCD ////////////
  //////////////////////////////////////////////////////

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.setCursor(0, 0);
  Serial.println(F("LCD setup is done!"));
  lcd.print("Hello Crys 1");
  delay(1000);

  //////////////////////////////////////////////////////
  ////////////  setup for IR reciever /////////////////
  //////////////////////////////////////////////////////

  Serial.println("Enabling IR Receiver"); // Print text on serial monitor
  irrecv.enableIRIn();                    // Start the IR receiver
  Serial.println("Enabled IR Receiver"); // Print text on serial monitor

}
/*
void check_IR() {
  if (irrecv.decode(&results_IR)) // Checks the code received or not
  {
    //Serial.print("Decoded Signal = "); //Print text on serial monitor
    //Serial.println(results_IR.value); // Print the decoded value on serial monitor
    Serial.println(results_IR.value, BIN); // Print the decoded value on serial monitor
    irrecv.resume(); // continue to receive the next value
  }
}

void test_operation() {
  int i = 0;
  
  for(i = 0; i<5; i++) {
    Serial.print(i, ":");
    check_IR(); 
  }
}
*/

void loop(void) {
  int i = 0;

  if (irrecv.decode(&results_IR)) // Checks the code received or not
  {
  
//      Serial.println("Trim left test:");
//      test_operation();
//      Serial.println("Trim right test:");
//      test_operation();
//      Serial.println("full thrust test:");
//      test_operation();
//      Serial.println("full thrust + left turn test:");
//      test_operation();
//      Serial.println("full thrust + right turn test:");
//      test_operation();


    Serial.println(results_IR.value, BIN); // Print the decoded value on serial monitor
    irrecv.resume(); // continue to receive the next value

      
//      delay(250);

  }

}
