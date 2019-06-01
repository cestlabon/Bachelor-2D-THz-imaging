#include <Arduino.h>


int analogPin = A0; 
int val = 0;  // variable to store the value read

void setup() {
  Serial.begin(9600);           //  setup serial
}

void loop() {
  val = analogRead(analogPin);  // read the input pin
  float voltage = val * (5.0 / 1024.0);
  Serial.println(voltage);          // debug value
  delay(100);
}