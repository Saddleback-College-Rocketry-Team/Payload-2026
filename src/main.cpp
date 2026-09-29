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
#include <CSV.h>      // all realted to saving to SD files
#include <NMEAGPS.h>  // Getting nmea sentences

// SENSOR LIBRARIES
#include <Adafruit_BME280.h>    // humidity: https://github.com/boschsensortec/BME280_SensorAPI
#include <Seeed_HM330X.h>       // dust: https://github.com/Seeed-Studio/Seeed_PM2_5_sensor_HM3301
#include <Adafruit_LTR390.h>    // uv: https://github.com/adafruit/Adafruit_LTR390/tree/master
#endif

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
const uint32_t  RATE_TRANS = 1000;       // 1HZ, transmitting

// SENSOR RELATED
const float SEA_LEVEL = 1013.25;       // sealevel pressure in hPa

// TRANSMITTER / RADIO
HardwareSerial &RADIO = Serial8;      // Serial we will be transmitting on

/***********************************************************************
 * CONTANTS AND VARIABLES
 **********************************************************************/
// CSV FILES
CSV fileHumid("humidity.csv");
File fileNmea = SD.open("nmea.csv", FILE_WRITE);

// SENSOR OBJECTS
Adafruit_BME280 sensorHumid; // humidity, temp, pressure
NMEAGPS gps(Serial2);        // gps / nmea sentences

// COUNTER
uint8_t counter_ram = 0;    // index for ram buffer
uint8_t counter_nmea = 0;   // index for nmea buffer
uint8_t indexSave = 0;      // index for saving to sd
uint8_t indexTrans = 0;     // index for transmitting nmea sentences

// VARIABLES FOR KEEPING TRACK OF TIME
unsigned long currentTime;    // current time, from when sensor was turned on
unsigned long lastBufferRam;  // last time sensor was read
unsigned long lastBufferNmea; // last time gps was read
unsigned long lastSDSave;     // last time data was saved to sd
unsigned long lastTrans;      // last time transmitted

/***********************************************************************
 * PACKETS
 **********************************************************************/
// Packet for RAM buffer - general sensors
struct packetRam {
  unsigned long ramTime;   // time read data

  // bme sensor
  float bmeAltitude;
  float bmeHumidity;
  float bmePressure; 
  float bmeTemp;
}; // END packetRam

/***********************************************************************
 * BUFFERS
 **********************************************************************/
// Buffer for all sensors
packetRam BufferRam[SIZE_BUFFER_RAM];

// Nmea sentences collected
char BufferNmea[SIZE_BUFFER_NMEA][SIZE_NMEA_SENT];

/***********************************************************************
 * CODE THAT RUNS ONCE
 **********************************************************************/
void setup() {
  // I2C CONNECTIONS
  Wire.begin();     // Default

  // UART CONNECTIONS
  RADIO.begin(9600);    // radio
  gps.begin(9600);      // gps

  // DEBUGGING: for computer
  Serial.begin(115200);

  // CONFIGS FOR TRANSMITTING OR RECEIVING

  /*********************************************************************
   * CHECK IF SENSORS ARE CONNECTED HERE
   ********************************************************************/
  // CHECK IF BME: humidity, temp, and pressure
   if (!sensorHumid.begin()) {
    Serial.println("Error! Cannot find sensor: BME");
  } // END if (!sensorHumid.begin())

  // CHECK RADIO
  RADIO.println("Testing radio!");

  /*********************************************************************
   * CREATE CSV FILES
   ********************************************************************/
  fileHumid.createCSV("time,altitude,humidity,pressure,temperature");

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
   * NMEA BUFFER: GET GPS
   * -------------------------------------------------------------------
   * Condition:
   *  if its time to read AND if our buffer isnt full
   * 
   * For sensors / data:
   *  - 
   ********************************************************************/
  // READ GPS
  gps.update();

  // CHECK IF RECIEVIED SENTENCE FROM GPS
  if (gps.available()) {

    // SAVE FULLY FORMED SENTENCE TO BUFFER
    gps.read(BufferNmea[counter_nmea]);
    
    // INDEX BUFFER
    counter_nmea++;
  } // END if
  
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
     * READ BME DATA: temp, pressure, humidity
     ******************************************************************/
    BufferRam[counter_ram].bmeAltitude = sensorHumid.readAltitude(SEA_LEVEL);
    BufferRam[counter_ram].bmeHumidity = sensorHumid.readHumidity();
    BufferRam[counter_ram].bmePressure = sensorHumid.readPressure();
    BufferRam[counter_ram].bmeTemp = sensorHumid.readTemperature();

    // INPUT CODE TO READ SENSORS
    
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
      fileHumid.writeToFile(BufferRam[indexSave].ramTime,
                            BufferRam[indexSave].bmeAltitude,
                            BufferRam[indexSave].bmeHumidity,
                            BufferRam[indexSave].bmePressure,
                            BufferRam[indexSave].bmeTemp);

      // INCREMENT COUNTER
      indexSave++;

    } // END while
    
    // REST WRITING COUNTERS
    counter_ram = 0;
    indexSave = 0;

  } // END if

  /*********************************************************************
   * TRANSMITTING TO RECIEVER
   * -------------------------------------------------------------------
   * Condition:
   *  if its time to save OR if our buffer is full
   * 
   * For sensors / data:
   *  - gps (all nmea sentences)
   ********************************************************************/
  if (currentTime - lastTrans >= RATE_TRANS) {
    // UPDATE LAST TRANSMISSION TIME
    lastTrans = currentTime;

    /*******************************************************************
     * ITERATE THROUGH BUFFER TO TRANSMIT
     ******************************************************************/
    while (indexTrans < counter_nmea) {
      // TRANSMITTING
      RADIO.print(BufferNmea[indexSave]);

      // INCREMENT TO NEXT SENTENCE
      indexTrans++;
    } // END while while (indexSave <= counter_nmea)

    // RESET
    indexTrans= 0;
    counter_nmea = 0;
  } // END if (currentTime - lastTrans >= RATE_TRANS) {

} // END loop() --------------------------------------------------------

/***********************************************************************
 * FUNCTION DEFINITIONS
 **********************************************************************/