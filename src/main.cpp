/**
 * @file main.cpp
 * @details Main code, what will be uploaded to microcontroller. This is
 *          assuming the component is a Teensy
 */
#ifndef MAIN_CPP
#define MAIN_CPP
/***********************************************************************
 * LIBRARIES
 **********************************************************************/
#include <Arduino.h>  // To make it compatible for Platform.IO
                      // delete if you're using Arduino IDE

// OUR CUSTOM LIBRARY 
#include <CSV.h>      // all realted to saving to SD files

#endif
// SENSOR LIBRARIES

/***********************************************************************
 * PINS SENSOR ARE CONNECTED TO TEENSY TO
 * do NOT change unless you change what pins they're connected to
 **********************************************************************/
// ACCELOROMETER - ADXL330
const int ACCEL_PIN_X = A0;   // X axis
const int ACCEL_PIN_Y = A1;   // Z axis
const int ACCEL_PIN_Z = A2;   // Y axis


/***********************************************************************
 * VARAIBLES TO CHANGE : rates, serials, etc.
 * Things that we might need to change
 **********************************************************************/
// SIZES: size = send rate (second) * read rate (Hz)
const uint8_t SIZE_BUFFER_RAM = 10;
const uint8_t SIZE_BUFFER_NMEA = 10;
const uint16_t SIZE_NMEA_SENT = 120;   // (max) size of nmea senteces

// RATES: rate (millis) = 1000 millis /  times to run per second
const uint8_t RATE_BUFFER_RAM = 100;   // 10Hz, read sensor data
const uint8_t RATE_BUFFER_NMEA = 100;  // 10Hz, get gps
const uint32_t  RATE_SD = 1000;          // 1Hz, save to sd

/***********************************************************************
 * CONTANTS AND VARIABLES
 **********************************************************************/
// CSV FILES
CSV fileAccel("accel.csv");

// SENSOR OBJECTS

// COUNTER
uint8_t counter_ram = 0;    // index for ram buffer
uint8_t counter_nmea = 0;   // index for nmea buffer
uint8_t indexSave = 0;      // index for saving to sd

// VARIABLES FOR KEEPING TRACK OF TIME
unsigned long currentTime;    // current time, from when sensor was turned on
unsigned long lastBufferRam;  // last time sensor was read
unsigned long lastSDSave;     // last time data was saved to sd

// SENSOR RELATED

/***********************************************************************
 * PACKETS
 **********************************************************************/
// Packet for RAM buffer - general sensors
struct packetRam {
  unsigned long ramTime;   // time read data

  // ACCELEROMETER ADXL330  DATA
  int accelX;   // x axis
  int accelY;   // y axis
  int accelZ;   // z axis
}; // END packetRam

/***********************************************************************
 * BUFFERS
 **********************************************************************/
// Buffer for all sensors
packetRam BufferRam[SIZE_BUFFER_RAM];

/***********************************************************************
 * CODE THAT RUNS ONCE
 **********************************************************************/
void setup() {

  // DEBUGGING: for computer
  Serial.begin(115200);

  // CONFIGS FOR TRANSMITTING OR RECEIVING

  /*********************************************************************
   * CHECK IF SENSORS ARE CONNECTED HERE
   ********************************************************************/
  
  /*********************************************************************
   * CREATE CSV FILES
   ********************************************************************/
  fileAccel.createCSV("time,x axis,y axis,z axis");

  // DELAY, milli seconds
  delay(300);

} // END setup() -------------------------------------------------------



/***********************************************************************
 * CODE THAT WILL RUN CONTINUOUSLY
 * Make faster tasks first and slower tasks last
 **********************************************************************/
void loop() {
  currentTime = millis();
  
  /*********************************************************************
   * RAM BUFFER: READING SENSOR DATA
   * -------------------------------------------------------------------
   * Condition:
   *  if its time to read AND if our buffer isnt full
   * 
   * For sensors / data:
   *  - 
   ********************************************************************/
  if (currentTime - lastBufferRam >= RATE_BUFFER_RAM) {
    // UPDATE LAST TIME READ
    lastBufferRam = currentTime;

    // SAVE TIME
    BufferRam[counter_ram].ramTime = currentTime;

    /*******************************************************************
     * READ ACCELEROMETER DATA: x, y, z axis
     ******************************************************************/
    BufferRam[counter_ram].accelX = analogRead(ACCEL_PIN_X);
    BufferRam[counter_ram].accelY = analogRead(ACCEL_PIN_Y);
    BufferRam[counter_ram].accelZ = analogRead(ACCEL_PIN_Z);
    
    // INCREMENT INDEX FOR NEXT READ
    counter_ram++;
  } // END if

  /*********************************************************************
   * SAVING TO SD
   * -------------------------------------------------------------------
   * Condition:
   *  if its time to save OR if our buffer is full
   * 
   * For sensors / data:
   *  -
   ********************************************************************/
  if (currentTime - lastSDSave >= RATE_SD) {
    // UPDATE LAST TIME READ
    lastSDSave = currentTime;

    while (indexSave < SIZE_BUFFER_RAM) {

      // WRITE TO FILE
      fileAccel.writeToFile(BufferRam[indexSave].ramTime,
                            BufferRam[indexSave].accelX,
                            BufferRam[indexSave].accelY,
                            BufferRam[indexSave].accelZ);

      // INCREMENT COUNTER
      indexSave++;

    } // END while
    
    // REST WRITING COUNTERS
    counter_ram = 0;
    indexSave = 0;

  } // END if

} // END loop() --------------------------------------------------------

/***********************************************************************
 * FUNCTION DEFINITIONS
 **********************************************************************/