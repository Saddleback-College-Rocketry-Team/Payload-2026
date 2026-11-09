#include <Arduino.h>
/***********************************************************************
 * PINS SENSOR ARE CONNECTED TO TEENSY TO
 * do NOT change unless you change what pins they're connected to
 **********************************************************************/
// ACCELOROMETER - ADXL330
const int ACCEL_PIN_X = A0;   // X axis
const int ACCEL_PIN_Y = A1;   // Z axis
const int ACCEL_PIN_Z = A2;   // Y axis

// Varaibles to Read Data
int readX = 0;
int readY = 0;
int readZ = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Time
  Serial.println(millis());

  // Reading and Outputing X axis
  readX = analogRead(ACCEL_PIN_X);
  Serial.print("X axis: ");
  Serial.println(readX);

  // Reading and Outputing Y axis
  readX = analogRead(ACCEL_PIN_Y);
  Serial.print("Y axis: ");
  Serial.println(readY);

  // Reading and Outputing Z axis
  readX = analogRead(ACCEL_PIN_Z);
  Serial.print("Z axis: ");
  Serial.println(readZ);

}