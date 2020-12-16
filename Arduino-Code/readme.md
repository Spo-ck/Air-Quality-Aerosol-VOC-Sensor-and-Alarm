# Arduino Code

## Libraries

* [M5StickC](https://github.com/m5stack/M5StickC)
* Wire.h
* [Adafruit_Sensor.h](https://github.com/adafruit/Adafruit_Sensor)
* [Adafruit_SGP30.h](https://github.com/adafruit/Adafruit_SGP30)
* [Adafruit_BME280.h](https://github.com/adafruit/Adafruit_BME280_Library)
* EEPROM.h

## About the Code

![M5STcikC](https://camo.githubusercontent.com/e300c3852f862f672c0fe0b59992ab82985a9da8ecea672ac21a02845e2c09ae/68747470733a2f2f6d35737461636b2e6f73732d636e2d7368656e7a68656e2e616c6979756e63732e636f6d2f696d6167652f6d352d646f63735f636f6e74656e742f636f72652f6d35737469636b635f30312e706e67)

The Purpose of this Microcontroller Code is to generate Sensor Sate from the SOnsors COnnected to to give an Alarm when the Air Quality is bad. For this reason, the SGP30 Sensor Module measure eCO2 and TVOC in the breathing Air. The BME280 Barometric Sensor is required to calibrate the SGP30, and therefore to enhance its accuracy.
When the eCO2 value is over 1000 ppm, or the TVOC value is over 150 ppb, an Alarm is triggered. The Alarm will result in an alarm sound provided by the buzzer modele, the built in LED enligh in RED and the display turning RED. By the built in settings menue, the Buzzer can also set into silent mode, where only the LED and the Display are tiggered in case of an alarm. The Alarm Sound is only triggered one time, when the eCO2 or TVOC value hits the threshold value, but the LED will be enlighted in RED untill the Sensor Value is below the threshold again. In addition, the buzzer is also disabled for 1 minute. Because of this, you wont hear multible buzzer alarms, when the sensor value is falling below, and hitting the threshold again within one minute.

![image](https://github.com/Spo-ck/Air-Quality-Aerosol-VOC-Sensor-and-Alarm/blob/main/Photos/IMG_7309.jpg)
***System Alarm***

The System has three built in views. The Detailed View only shows the eCO2 and the TVOC values, whereas the Air Quality view shows the Time, Temperature, Humidity and the Battery Status in addition. More advanced is the Chart View, which shows the eCO2 value for the last 20min. If the critical Threshold was hit, the bar shown will red, and if the value is below, the bar will be green. 
To Navigate through the different views, the front Button (Button A) is used. The last view is also Stored in EEPROM, so that the Sensor will start in the last used view when it starts next time.

![image](https://github.com/Spo-ck/Air-Quality-Aerosol-VOC-Sensor-and-Alarm/blob/main/Photos/IMG_7299.jpg)
***System Detailed View***

![image](https://github.com/Spo-ck/Air-Quality-Aerosol-VOC-Sensor-and-Alarm/blob/main/Photos/IMG_7300.jpg)
***System Air Quality View***

![image](https://github.com/Spo-ck/Air-Quality-Aerosol-VOC-Sensor-and-Alarm/blob/main/Photos/IMG_7306.jpg)
***System Chart View***

Nomally, the display is on all time, but it is also possible to disable the display. In this mode, the Front button needs to be pressed one time to turn the display on for 5s. Now, pressing this button again will make the view change.

In Addidion, a settings view is implemented. In the Settings view, one can change the the Display Mode (Display always on or on for 5s) and Alarm Mode (Buzzer turned off or on). This view is opened when the Side Button (Button B) is pressed. In this view, the first option it the Display Mode. When the Front Button is pressed, the Mode will change (Display always on or on for 5s). The Option is confirmed with pressind the Side Button Again. After that, the Settion for the Alarm Mode (Buzzer turned off or on) is opened and can be changed with pressing the Front Button. In general, the active Setting is indicated with a red line on the left side. Pressing the Side Button again will confirm this setting, and you will return to the last Sensor View.

![image](https://github.com/Spo-ck/Air-Quality-Aerosol-VOC-Sensor-and-Alarm/blob/main/Photos/IMG_7301.jpg)
***System Settings***

## How To

In order to flash this code on the M5StickC, the Arduino IDE needs to download and to be installed first. The, the M5StickCs driver and the above mentioned librarieds need to be installed. You can then open (or copy/past) the code into the IDE and flash it.

* [M5StickC Manual and Software](https://m5stack.com/pages/download)
* [Arduino IDE](https://www.arduino.cc/en/software)

## Still tp be implemented

Every Gas Sensor is different, so it also needs to be calibrated. In order to calibrate it, it needs to be turned on for 12h, and this value is then valid for 7 days. To do so, the value needs to be stored in EEPROM, and send to the sensor in the Settings function. Because I never used it for 12h, this is not implemented yet. If not set, the sensor used 400 ppm for eCO2 and 0 ppb for TVOC as calibration value.
Also, it was not possible to set the display off completely. In the current implementation, it will only be black in the 5s-On mode and not completely off.
