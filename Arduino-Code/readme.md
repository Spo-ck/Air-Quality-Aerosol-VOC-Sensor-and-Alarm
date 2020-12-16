# Arduino Code

## Libraries

* [M5StickC](https://github.com/m5stack/M5StickC)
* Wire.h
* [Adafruit_Sensor.h](https://github.com/adafruit/Adafruit_Sensor)
* [Adafruit_SGP30.h](https://github.com/adafruit/Adafruit_SGP30)
* [Adafruit_BME280.h](https://github.com/adafruit/Adafruit_BME280_Library)
* EEPROM.h

## About the Code

The Purpose of this Microcontroller Code is to generate Sensor Sate from the SOnsors COnnected to to give an Alarm when the Air Quality is bad. For this reason, the SGP30 Sensor Module measure eCO2 and TVOC in the breathing Air. The BME280 Barometric Sensor is required to calibrate the SGP30, and therefore to enhance its accuracy.
When the eCO2 value is over 1000 ppm, or the TVOC value is over 150 ppb, an Alarm is triggered. The Alarm will result in an alarm sound provided by the buzzer modele, the built in LED enligh in RED and the display turning RED. By the built in settings menue, the Buzzer can also set into silent mode, where only the LED and the Display are tiggered in case of an alarm. The Alarm Sound is only triggered one time, when the eCO2 or TVOC value hits the threshold value, but the LED will be enlighted in RED untill the Sensor Value is below the threshold again. In addition, the buzzer is also disabled for 1 minute. Because of this, you wont hear multible buzzer alarms, when the sensor value is falling below, and hitting the threshold again within one minute.

![image](https://github.com/Spo-ck/Air-Quality-Aerosol-VOC-Sensor-and-Alarm/blob/main/Photos/IMG_7309.jpg)
***System Alarm***

## How To
