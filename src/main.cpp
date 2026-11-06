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
#include <SD.h>       // Teensy SD config
#include <Wire.h>     // Teensy for wire configs

// OUR CUSTOM LIBRARY 
#include <CSV.h>      // CSV files

// SENSOR LIBRARIES
#include <Seeed_HM330X.h>       // dust: https://github.com/Seeed-Studio/Seeed_PM2_5_sensor_HM3301

#endif

/***********************************************************************
 * VARAIBLES TO CHANGE : rates, serials, etc.
 * Things that we might need to change
 **********************************************************************/
// SIZES: size = send rate (second) * read rate (Hz)
const uint8_t SIZE_BUFFER_RAM = 10;

// RATES: rate (millis) = 1000 millis /  times to run per second
const uint8_t RATE_BUFFER_RAM = 100;   // 10Hz, read sensor data
const uint8_t RATE_SD = 1000;          // 1Hz, save to sd

// SENSOR RELATED
const float SEA_LEVEL = 1013.25;       // sealevel pressure in hPa

/***********************************************************************
 * CONTANTS AND VARIABLES
 **********************************************************************/
// CSV FILES
CSV fileDust("dust.csv");

// SENSOR OBJECTS
HM330X sensorDust;          // dust particles

// COUNTER
uint8_t counter_ram = 0;    // index for ram buffer
uint8_t indexSave;          // index for saving to sd

// VARIABLES FOR KEEPING TRACK OF TIME
float currentTime;    // current time, from when sensor was turned on
float lastBufferRam;  // last time sensor was read
float lastSDSave;     // last time data was saved to sd

/***********************************************************************
 * PACKETS
 **********************************************************************/
// Packet for RAM buffer - general sensors
struct packetRam {
  float ramTime;   // time read data

  // dust sensor
}; // END packetRam

// DUST SENSOR DATA
struct dustData {

}; // END dustData

/***********************************************************************
 * BUFFERS
 **********************************************************************/
// Buffer for all sensors
packetRam BufferRam[SIZE_BUFFER_RAM];

/***********************************************************************
 * CODE THAT RUNS ONCE
 **********************************************************************/
void setup() {
  // I2C CONNECTIONS
  Wire.begin();     // Default

  // UART CONNECTIONS
  Serial.begin(115600);

  // CONFIGS FOR TRANSMITTING OR RECEIVING

  /*********************************************************************
   * CHECK IF SENSORS ARE CONNECTED HERE
   ********************************************************************/
  // CHECK IF BME: humidity, temp, and pressure
   if (!sensorDust.init()) {
    Serial.println("Error! Cannot find sensor: dust");
  }
  // sensor code

  /*********************************************************************
   * CREATE CSV FILES
   ********************************************************************/
  fileDust.createCSV("time,dust");
  
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

    // SAVE TIME
    BufferRam[counter_ram].ramTime = currentTime;

    /*******************************************************************
     * READ DUST DATA: temp, pressure, humidity
     ******************************************************************/

    // INPUT CODE TO READ SENSORS
    
    // INCREMENT INDEX FOR NEXT READ
    counter_ram++;
  } // END if

  /*********************************************************************
   * SAVING TO SD
   * -------------------------------------------------------------------
   * Note: code is assuming transmitting and sd saving rate are the same
   * 
   * Condition:
   *  if its time to save AND if our buffer is full
   * 
   * For sensors / data:
   *  -
   ********************************************************************/
  if (currentTime - lastSDSave >= RATE_SD) {
    uint8_t indexSave = 0;

    while (indexSave < SIZE_BUFFER_RAM) {

      // WRITE TO FILE

      // INCREMENT COUNTER
      indexSave++;

    } // END while
    
    // REST WRITING COUNTERS
    counter_ram = 0;
  } // END if

} // END loop() --------------------------------------------------------

/***********************************************************************
 * FUNCTION DEFINITIONS
 **********************************************************************/