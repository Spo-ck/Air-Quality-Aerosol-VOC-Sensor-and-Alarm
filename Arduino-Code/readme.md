## Arduino Code

### Libraries

* [M5StickC](https://github.com/m5stack/M5StickC)
* Wire.h
* [Adafruit_Sensor.h](https://github.com/adafruit/Adafruit_Sensor)
* [Adafruit_SGP30.h](https://github.com/adafruit/Adafruit_SGP30)
* [Adafruit_BME280.h](https://github.com/adafruit/Adafruit_BME280_Library)
* EEPROM.h

### About the Code

![M5STcikC](https://camo.githubusercontent.com/e300c3852f862f672c0fe0b59992ab82985a9da8ecea672ac21a02845e2c09ae/68747470733a2f2f6d35737461636b2e6f73732d636e2d7368656e7a68656e2e616c6979756e63732e636f6d2f696d6167652f6d352d646f63735f636f6e74656e742f636f72652f6d35737469636b635f30312e706e67)

The purpose of this Microcontroller Code is to generate sensor data from the sensors connected to trigger an Alarm when the Air Quality is bad. For this reason, the SGP30 Sensor Module measures eCO2 and TVOC in the breathing Air. The BME280 Barometric Sensor is required to calibrate the SGP30, and therefore to enhance its accuracy.
When the eCO2 value is over 1000 ppm, or the TVOC value is over 150 ppb, an alarm is triggered. The alarm will result in an alarm sound provided by the buzzer modele, the built in LED enlight in RED and the display turning RED. By the built in settings options, the Buzzer can also set into silent mode, where only the LED and the Display are tiggered in case of an alarm. The alarm sound is only triggered once in 60s, when the eCO2 or TVOC value hits the threshold value, but the LED will be enlighted in RED untill the Sensor Value is below the threshold again. In addition, the buzzer then disabled for 1 minute. Because of this, you will not hear multible buzzer alarms, when the sensor value is falling below, and hitting the threshold again within one minute.

![image](https://github.com/Spo-ck/Air-Quality-Aerosol-VOC-Sensor-and-Alarm/blob/main/Photos/IMG_7309.jpg?raw=true)
***System Alarm***

The System has three built in views. The Detailed View shows only the eCO2 and the TVOC values, whereas the Air Quality view shows the Time, Temperature, Humidity and the Battery Status in addition. More advanced is the Chart View, which shows the eCO2 value for the last 20min. If the critical threshold was hit, the bar shown will turn red, and if the value is below, the bar will be green. 
To navigate through the different views, the front button (Button A) is used. The last view is also stored in EEPROM, so that the sensor will start in the last used view when it starts next time.

![image](https://github.com/Spo-ck/Air-Quality-Aerosol-VOC-Sensor-and-Alarm/blob/main/Photos/IMG_7299.jpg?raw=true)
***System Detailed View***

![image](https://github.com/Spo-ck/Air-Quality-Aerosol-VOC-Sensor-and-Alarm/blob/main/Photos/IMG_7300.jpg?raw=true)
***System Air Quality View***

![image](https://github.com/Spo-ck/Air-Quality-Aerosol-VOC-Sensor-and-Alarm/blob/main/Photos/IMG_7306.jpg?raw=true)
***System Chart View***

Nomally, the display is On all time, but it is also possible to disable the display. In this mode, the front button needs to be pressed one time to turn the display on for 5s. After that, pressing this button again will make the view change as usual.

In addidion, a settings view is implemented. In the settings view, one can change the Display Mode (Display always on or on for 5s) and the Alarm Mode (Buzzer turned off or on). This view is opened when the side button (Button B) is pressed. In this view, the first option it the Display Mode. When the Front Button is pressed, the Mode will change (Display always on or on for 5s). The option is confirmed with pressind the side button again. After that, the settings for the Alarm Mode (Buzzer turned off or on) is opened and can be changed with pressing the front button. In general, the active settings option is indicated with a red line on the left side. Pressing the side button a third time will confirm the settings, and open the last Sensor View again.

![image](https://github.com/Spo-ck/Air-Quality-Aerosol-VOC-Sensor-and-Alarm/blob/main/Photos/IMG_7301.jpg)
***System Settings***

### How To

In order to flash this code on the M5StickC, the Arduino IDE needs to download and to be installed first. Then, the M5StickCs driver and the above mentioned libraries need to be installed. You can then open (or copy/past) the code into the IDE and flash it.

* [M5StickC Manual and Software](https://m5stack.com/pages/download)
* [Arduino IDE](https://www.arduino.cc/en/software)
