/*
Author              : Genady M (GitHub user name: gy-m)
Arduino IDE         : 1.8.12
GitHub Repository:  : https://github.com/gy-m/Arduino_Uno
Credits:            : Dejan Nedelkovski
  Ultrasonic Sensor HC-SR04 and Arduino Tutorial
  www.HowToMechatronics.com
==============================================================================
*/

// defines pins numbers - nano
//const int sonar_trigPin = 10;
//const int sonar_echoPin = 11;
// defines pins numbers - uno
const int sonar_trigPin = 8;
const int sonar_echoPin = 9;

// defines variables
long duration;
int distance;

void setup() {
  pinMode(sonar_trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(sonar_echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
}
void loop() {
  // Clears the trigPin
  digitalWrite(sonar_trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(sonar_trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(sonar_trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(sonar_echoPin, HIGH);
  // Calculating the distance
  distance= duration*0.034/2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance cm: ");
  Serial.println(distance);
}
