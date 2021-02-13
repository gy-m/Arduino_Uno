#include <SD.h>

//Demo Function: Display graphics, characters
//Arduino IDE: 1.6.5
// Board: Arduino UNO R3, Arduino Mega2560,Arduino Leonardo
// Board:OPEN-SMART UNO R3 5V / 3.3V

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
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
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

   //////////// setup for TFT ////////////
  Serial.begin(9600);
  Serial.println(F("TFT LCD test"));

  #ifdef USE_OPENSMART_SHIELD_PINOUT
    Serial.println(F("Using OPENSMART 3.2\" TFT Arduino Shield Pinout"));
  #else
    Serial.println(F("Using OPENSMART 3.2\" TFT Breakout Board Pinout"));
  #endif

  Serial.print("TFT size is "); Serial.print(tft.width()); Serial.print("x"); Serial.println(tft.height());

  uint32_t when = millis();
//    if (!Serial) delay(5000);           //allow some time for Leonardo
  if (!Serial) delay(10);           //allow some time for Leonardo
  Serial.println("Serial took " + String((millis() - when)) + "ms to start");
  static uint16_t identifier;
  //    tft.reset();                 //we can't read ID on 9341 until begin()
  g_identifier = tft.readID(); //
  Serial.print("ID = 0x");
  Serial.println(g_identifier, HEX);
  tft.begin(0x8357);//to enable RM68140 driver code
  tft.setRotation(3);
  
 /* 
  Serial.println(F("Benchmark                Time (microseconds)"));

  Serial.print(F("Screen fill              "));
  Serial.println(testFillScreen());
//  delay(500);
   delay(10);

  Serial.print(F("Text                     "));
  Serial.println(testText());
//  delay(3000);
   delay(10);

  Serial.print(F("Lines                    "));
  Serial.println(testLines(CYAN));
//  delay(500);
   delay(10);

  Serial.print(F("Horiz/Vert Lines         "));
  Serial.println(testFastLines(RED, BLUE));
//  delay(500);
   delay(10);

  Serial.print(F("Rectangles (outline)     "));
  Serial.println(testRects(GREEN));
//  delay(500);
   delay(10);

  Serial.print(F("Rectangles (filled)      "));
  Serial.println(testFilledRects(YELLOW, MAGENTA));
//  delay(500);
   delay(10);

  Serial.print(F("Circles (filled)         "));
  Serial.println(testFilledCircles(10, MAGENTA));

  Serial.print(F("Circles (outline)        "));
  Serial.println(testCircles(10, WHITE));
//  delay(500);
   delay(10);

  Serial.print(F("Triangles (outline)      "));
  Serial.println(testTriangles());
//  delay(500);
   delay(10);

  Serial.print(F("Triangles (filled)       "));
  Serial.println(testFilledTriangles());
//  delay(500);
   delay(10);

  Serial.print(F("Rounded rects (outline)  "));
  Serial.println(testRoundRects());
//  delay(500);
   delay(10);

  Serial.print(F("Rounded rects (filled)   "));
  Serial.println(testFilledRoundRects());
//  delay(500);
   delay(10);
*/

  Serial.println(F("TFT setup is done"));
  delay(1000);

  ////////////  setup for LiquidCrystal LCD ////////////
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.setCursor(0, 0);
  lcd.print("LCD setup is done!");
  delay(1000);

}

void loop(void) {
  
//  for(uint8_t rotation=0; rotation<4; rotation++) {
//  	tft.fillScreen(BLACK);
//    tft.setRotation(rotation);
//	tft.setCursor(0, 0);
//  tft.setTextColor(WHITE);
//  tft.setTextSize(1);
//  tft.print(rotation);
//  testText();
//    delay(2000);
//  }

    tft.fillScreen(BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor(WHITE);
    tft.setTextSize(5);
    tft.print("Hello TFT!");
//    lcd.setCursor(0, 0);
//    lcd.print("Hello, Crys!");

    // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.setCursor(0, 0);
  lcd.print("Hello, Crys!");

    delay(1500);

}

// Not mandatory
unsigned long testFillScreen() {
  unsigned long start = micros();
  tft.fillScreen(BLACK);
//  delay(1000);
  delay(10);
  tft.fillScreen(RED);
//  delay(1000);
  delay(10);
  tft.fillScreen(GREEN);
//  delay(1000);
  delay(10);
  tft.fillScreen(BLUE);
//  delay(1000);
  delay(10);
  tft.fillScreen(BLACK);
  return micros() - start;
}

// Not mandatory

unsigned long testText() {
//  tft.fillScreen(BLACK);
  unsigned long start = micros();
//  tft.setCursor(0, 0);
//  tft.setTextColor(WHITE);
  tft.setTextSize(1);
//  tft.println("Hello World!");
  /*  
    tft.setTextColor(YELLOW); 
    tft.setTextSize(2);
  tft.println(1234.56);
  tft.setTextColor(RED);    tft.setTextSize(3);
  tft.println(0xDEADBEEF, HEX);
  tft.println();
  */
  tft.setCursor(0, 50);
  tft.setTextColor(WHITE);
  tft.setTextSize(4);
  tft.println("OPEN-SMART");
  tft.setTextColor(GREEN);
  tft.setTextSize(2);
  tft.println("3.5 inch TFT LCD");
/*  tft.setTextSize(1);
  tft.println("my foonting turlingdromes.");
  tft.println("And hooptiously drangle me");
  tft.println("with crinkly bindlewurdles,");
  tft.println("Or I will rend thee");
  tft.println("in the gobberwarts");
  tft.println("with my blurglecruncheon,");
  tft.println("see if I don't!");
  */
  return micros() - start;
}



// Not mandatory
/*
unsigned long testLines(uint16_t color) {
  unsigned long start, t;
  int           x1, y1, x2, y2,
                w = tft.width(),
                h = tft.height();

  tft.fillScreen(BLACK);

  x1 = y1 = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t     = micros() - start; // fillScreen doesn't count against timing

  tft.fillScreen(BLACK);

  x1    = w - 1;
  y1    = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  tft.fillScreen(BLACK);

  x1    = 0;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  tft.fillScreen(BLACK);

  x1    = w - 1;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);

  return micros() - start;
}

unsigned long testFastLines(uint16_t color1, uint16_t color2) {
  unsigned long start;
  int           x, y, w = tft.width(), h = tft.height();

  tft.fillScreen(BLACK);
  start = micros();
  for(y=0; y<h; y+=5) tft.drawFastHLine(0, y, w, color1);
  for(x=0; x<w; x+=5) tft.drawFastVLine(x, 0, h, color2);

  return micros() - start;
}
*/

// Not mandatory
/*
unsigned long testRects(uint16_t color) {
  unsigned long start;
  int           n, i, i2,
                cx = tft.width()  / 2,
                cy = tft.height() / 2;

  tft.fillScreen(BLACK);
  n     = min(tft.width(), tft.height());
  start = micros();
  for(i=2; i<n; i+=6) {
    i2 = i / 2;
    tft.drawRect(cx-i2, cy-i2, i, i, color);
  }

  return micros() - start;
}

unsigned long testFilledRects(uint16_t color1, uint16_t color2) {
  unsigned long start, t = 0;
  int           n, i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(BLACK);
  n = min(tft.width(), tft.height());
  for(i=n; i>0; i-=6) {
    i2    = i / 2;
    start = micros();
    tft.fillRect(cx-i2, cy-i2, i, i, color1);
    t    += micros() - start;
    // Outlines are not included in timing results
    tft.drawRect(cx-i2, cy-i2, i, i, color2);
  }

  return t;
}
*/

// Not mandatory
/*
unsigned long testFilledCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int x, y, w = tft.width(), h = tft.height(), r2 = radius * 2;

  tft.fillScreen(BLACK);
  start = micros();
  for(x=radius; x<w; x+=r2) {
    for(y=radius; y<h; y+=r2) {
      tft.fillCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}
*/

// Not mandatory
/*
unsigned long testCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int           x, y, r2 = radius * 2,
                w = tft.width()  + radius,
                h = tft.height() + radius;

  // Screen is not cleared for this one -- this is
  // intentional and does not affect the reported time.
  start = micros();
  for(x=0; x<w; x+=r2) {
    for(y=0; y<h; y+=r2) {
      tft.drawCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}
*/

// Not mandatory
/*
unsigned long testTriangles() {
  unsigned long start;
  int           n, i, cx = tft.width()  / 2 - 1,
                      cy = tft.height() / 2 - 1;

  tft.fillScreen(BLACK);
  n     = min(cx, cy);
  start = micros();
  for(i=0; i<n; i+=5) {
    tft.drawTriangle(
      cx    , cy - i, // peak
      cx - i, cy + i, // bottom left
      cx + i, cy + i, // bottom right
      tft.color565(0, 0, i));
  }

  return micros() - start;
}
*/


// Not mandatory
/*
unsigned long testFilledTriangles() {
  unsigned long start, t = 0;
  int           i, cx = tft.width()  / 2 - 1,
                   cy = tft.height() / 2 - 1;

  tft.fillScreen(BLACK);
  start = micros();
  for(i=min(cx,cy); i>10; i-=5) {
    start = micros();
    tft.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      tft.color565(0, i, i));
    t += micros() - start;
    tft.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      tft.color565(i, i, 0));
  }

  return t;
}
*/

// Not mandatory
/*
unsigned long testRoundRects() {
  unsigned long start;
  int           w, i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(BLACK);
  w     = min(tft.width(), tft.height());
  start = micros();
  for(i=0; i<w; i+=6) {
    i2 = i / 2;
    tft.drawRoundRect(cx-i2, cy-i2, i, i, i/8, tft.color565(i, 0, 0));
  }

  return micros() - start;
}
*/

// Not mandatory
/*
unsigned long testFilledRoundRects() {
  unsigned long start;
  int           i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(BLACK);
  start = micros();
  for(i=min(tft.width(), tft.height()); i>20; i-=6) {
    i2 = i / 2;
    tft.fillRoundRect(cx-i2, cy-i2, i, i, i/8, tft.color565(0, i, 0));
  }

  return micros() - start;
}
*/
