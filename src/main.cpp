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
#include <Adafruit_LTR390.h>    // uv: https://github.com/adafruit/Adafruit_LTR390/tree/master

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
CSV fileUV("uv.csv");

// SENSOR OBJECTS
Adafruit_LTR390 sensorUV = Adafruit_LTR390 (); // uv

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

  // uv sensor
  uint32_t uvParticulates;  // uv light particulates
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
  // I2C CONNECTIONS
  Wire.begin();     // Default

  // UART CONNECTIONS
  Serial.begin(115600);

  // CONFIGS FOR TRANSMITTING OR RECEIVING

  /*********************************************************************
   * CHECK IF SENSORS ARE CONNECTED HERE
   ********************************************************************/
  // CHECK UV
   if (!sensorUV.begin()) {
    Serial.println("Error! Cannot find sensor: UV");
  }
  // SET UV MODES
  sensorUV.setMode(LTR390_MODE_UVS);                // if getting uv or av data
  sensorUV.enable(true);                            // enable sensor
  sensorUV.setGain(LTR390_GAIN_1);                  // how many data per reading
  sensorUV.setResolution(LTR390_RESOLUTION_13BIT);  // how many bits per reading
  
  // sensor code

  /*********************************************************************
   * CREATE CSV FILES
   ********************************************************************/
  fileUV.createCSV("time,particulates");
  
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
     * READ UV / LTR390 DATA: particulates
     ******************************************************************/
    BufferRam[counter_ram].uvParticulates = sensorUV.readUVS();

    // INPUT CODE TO READ SENSORS

    // DEBUGGING: OUTPUTTING TO SERIAL PORT
    Serial.print("Counter: ");
    Serial.println(counter_ram);
    Serial.print("Time: ");
    Serial.println(BufferRam[counter_ram].ramTime);
    Serial.print("Particulates: ");
    Serial.println(BufferRam[counter_ram].uvParticulates);
    
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
      fileUV.writeToFile(BufferRam[indexSave].ramTime,
                         BufferRam[indexSave].uvParticulates);

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
