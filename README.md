# Rocketry-Template

[![Build](https://github.com/saddleback-college-rocketry-team/Rocketry-Template/actions/workflows/native.yml/badge.svg?branch=main)](https://github.com/saddleback-college-rocketry-team/Rocketry-Template/actions/workflows/native.yml)
[![Docs](https://img.shields.io/badge/docs-doxygen-blue)](https://saddleback-college-rocketry-team.github.io/Rocketry-Template/)

TO DO:
X = need to do | O = done

--- 

## ABOUT:
Collect data and save to SD card as a csv file
get the data from sensors at 10 Hz:
 - particulates
 - gasses
 - pressure
 - humidity
 - uv
 - gps data by getting all nmea sentences at 100 Hz

Record video
- save to sd

Transmit data to a receiver (live)
 - runs at 1 HZ per second
 - will have a display to show rocket location
 - used to track rocket location


### SENSORS

ADXL330 / ACCELEROMETER: https://www.analog.com/media/en/technical-documentation/data-sheets/ADXL330.pdf
- acceleration, excludes gravity

ArduCAM-M-2MP / Camera: https://www.uctronics.com/download/Amazon/ArduCAM_Mini_2MP_Camera_Shield_DS.pdf

HM3301 / DUST: https://media.digikey.com/pdf/Data%20Sheets/Seeed%20Technology/Grove_Laser_PM2.5_Sensor_HM3301_Web.pdf
- 

GPS: https://www.amazon.com/VFAN-Navigation-Raspberry-Aircraft-Controller/dp/B0D9LFPHQG
- get location (via nmea sentences)

BME280 / HUMIDITY: https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bme280-ds002.pdf
- for humidity

DRF446D20 / RADIO: https://www.dorji.com/docs/data/DRF4463D20.pdf
- transmitting nmea sentences

LTR-390UV-01 / UV: https://optoelectronics.liteon.com/upload/download/DS86-2015-0004/LTR-390UV_Final_%20DS_V1%201.pdf

---

## CONNECTIONS ACTUAL (ON ROCKET)

MICROCONTROLLER
Teensy 4.1: https://www.pjrc.com/store/teensy41.html

TEENSY  |   ACCELEROMETER
  GND   |     GND
  5V    |     5V
  14    |     AX
  15    |     AY
  16    |     AZ

WIRE: Wire2 
TEENSY  |   CAMERA
  GND   |     GND
  5V    |     5V
  25    |     SDA
  24    |     SCL

WIRE: Wire2
TEENSY  |   DUST
  GND   |     GND
  5V    |     5V
  25    |     SDA
  24    |     SCL

SERIAL: Serial2
TEENSY  |   GPS
  GND   |     GND
  5V    |     5V
  7     |     RX
  8     |     TX

WIRE: Wire2 
TEENSY  |   HUMIDITY
  GND   |     GND
  5V    |     5V
  25    |     SDA
  24    |     SCL

SERIAL: Serial8
TEENSY    |   RADIO
  GND     |     GND
  5V      |     5V
  34 (TX) |     RX
  35 (RX) |     TX

WIRE: Wire2
TEENSY  |   UV
  GND   |     GND
  5V    |     5V
  25    |     SDA
  24    |     SCL

```
## Helpful Libraries / Info:
  ### Libraries for Sensors:
BME280: https://github.com/boschsensortec/BME280_SensorAPI
DUST: https://github.com/Seeed-Studio/Seeed_PM2_5_sensor_HM3301
UV: https://github.com/adafruit/Adafruit_LTR390/tree/master

  ### Millis:
https://docs.arduino.cc/language-reference/en/functions/time/millis/
 
  ### Delay:
https://docs.arduino.cc/language-reference/en/functions/time/delay/
 
  ### Serials and baud rates:
BAUD: https://docs.arduino.cc/language-reference/en/functions/communication/serial/begin/

UART: https://docs.arduino.cc/language-reference/en/functions/communication/serial/

I2C:  https://docs.arduino.cc/language-reference/en/functions/communication/wire/
  
  ### Teensy specific serials / pins:
UART: https://www.pjrc.com/teensy/td_uart.html

I2C:  https://www.pjrc.com/teensy/td_libs_Wire.html
 
  ### Transmitting / Recieving:
https://docs.arduino.cc/learn/communication/uart/
 
  ### GPS:
https://www.pjrc.com/teensy/td_libs_TinyGPS.html

  ### SD / file:
https://docs.arduino.cc/libraries/sd/