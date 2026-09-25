/**
 * @file test_main.cpp
 * @brief file that will be to test things
 * @date 2026-03-07
 * 
 */
#ifndef MAIN_CPP
#define MAIN_CPP
/***********************************************************************
 * LIBRARIES
 **********************************************************************/
#include <Arduino.h>  // To make it compatible for Platform.IO
                      // delete if you're using Arduino IDE
#include <SD.h>       // Teensy SD config

#include <SoftwareSerial.h>   // for recieving
#include <Wire.h>     // Teensy for wire configs

// OUR CUSTOM LIBRARY 
#include <CSV.h>      // all realted to saving to SD files
#include <NMEAGPS.h>  // Getting nmea sentences

// SENSOR LIBRARIES
#include <Adafruit_BME280.h>    // humidity: https://github.com/boschsensortec/BME280_SensorAPI
#include <Seeed_HM330X.h>       // dust: https://github.com/Seeed-Studio/Seeed_PM2_5_sensor_HM3301
#include <Adafruit_LTR390.h>    // uv: https://github.com/adafruit/Adafruit_LTR390/tree/master


#define RADIO_RX 4    // nano D-pin connected to radio RX pin
#define RADIO_TX 5    // nano D-pin connected to radio TX pin
#endif

SoftwareSerial Radio(RADIO_RX, RADIO_TX);   // radio 

/**
 * @brief Arduino-style test runner.
 */
void setup() {
    
    // BAUD RATES
  Serial.begin(115200);     // computer / testing
  Radio.begin(9600);        // radio

  // Check sensor
}

void loop() {
    // Testing radio
    Radio.println("Hello from transmitter!");
    Serial.println("Message sent");

    // Print sensor
}
