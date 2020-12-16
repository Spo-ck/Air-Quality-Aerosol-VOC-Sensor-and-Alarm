# Air Quality & Aerosol (VOC) Sensor and Alarm

## Electronic

### Part List

#### Electronic
* [Buzzer module](https://www.makershop.de/module/audio/passiver-buzzer-summer/)
* [BME280 Barometric Sensor](https://www.makershop.de/sensoren/temperatur/bme280/)
* [GY-SGP30 Gas Sensor (alternative Grove SGP30)](https://www.ebay.de/itm/GY-SGP30-Air-Quality-Sensor-Breakout-VOC-and-eCO2-QITA/353147995118?hash=item523942c3ee:g:GLcAAOSwIZ9fGU-k)
* [M5StickC Microcontroller](https://eckstein-shop.de/M5Stack-M5StickC-PLUS-ESP32-PICO-Mini-IoT-Development-Kit)
* [Grove Sensor Cable](https://eckstein-shop.de/M5Stack-Buckled-Grove-Cable-20cm-5pcs)
* [PCB](https://www.reichelt.de/punkt-streifenrasterplati-hartpapier-160x100mm-h25ps160-p23953.html?&trstct=pol_5&nbc=1)
* [Generic 1x8 Pin Header 2,54mm](https://www.conrad.de/de/p/wws-40-g-sl40g1-pin-header-1x40-rm2-54-straight-au-inhalt-20-stueck-800166396.html)
* Generic solid core wires (PCB)
* Generic Lead Solder

#### Tools
* [Soldering Station](https://www.conrad.de/de/p/basetech-zd-931-loetstation-digital-48-w-150-bis-450-c-1460697.html)
* [Solder Pump](https://www.conrad.de/de/p/toolcraft-lee-192-entloetsaugpumpe-antistatisch-2196503.html)
* [Third Hand](https://www.conrad.de/de/p/inline-dritte-hand-mit-lupe-800404647.html)
* [Soldering Tools](https://www.conrad.de/de/p/toolcraft-lns-151-platinenbesteck-6teilig-2182257.html)

### Pin Mapping
* GND:  Buzzer Module -, BME280 GND, SGP30 GND
* 3.3V: Buzzer Module +, BME280 VIN, SGP30 VIN
* G26:  Buzzer Module S
* G32:  BME280 SDA, SGP30 SDA
* G33:  BME280 SCL, SGP30 SCL

![image](https://github.com/Spo-ck/Air-Quality-Aerosol-VOC-Sensor-and-Alarm/blob/main/Electronic/Schematic.png)

***Electronic Schematic***

### Assembly

The system basically consists out of the M5StickC Microcontroller, a Buzzer Module, a BME280 Barometric Sensor Module (Temperature, Humidity, Pressure) and an SGP30 Gas Sensor Module (eCO2, TVOC, Ethanol, H2). The Buzzer Module is connected to the M5StickCs Pin Header, and the Seonsors are connected to the I2C bus via the Grove Connector.
All 3 Components are soldered together on a single prototyping PCD. To this prototyping PCD is then soldered a 1x8 Pin Header (2.54mm) so that the PCB can be connected to the Front Pin Header of the M5StickC. The Power Supply off all 3 Modules is the 3.3V and the GND Pin of the M5StickCs Pin Header. The Buzzer-Modules signal pin is soldered to Pin 26, because this pin allows output signals. 
Because the Sensor Modules need to be connected to the I2C Bus, which is oly available on the Groov Connector of the M5StickC, a 20cm Grove Sensor cable was utilized. One connected was cutted of, and the 5V and GND Cables were removed. The remaining Yeallow (G32, SDA) and white  (G33, SCL) cables were soldered to the corresponding pins on the PCD, to connect them to the sensor module

![image](https://github.com/Spo-ck/Air-Quality-Aerosol-VOC-Sensor-and-Alarm/blob/main/Photos/IMG_7296.jpg)
***PCB Front***

![image](https://github.com/Spo-ck/Air-Quality-Aerosol-VOC-Sensor-and-Alarm/blob/main/Photos/IMG_7297.jpg)
***PCB Back***

![image](https://github.com/Spo-ck/Air-Quality-Aerosol-VOC-Sensor-and-Alarm/blob/main/Photos/IMG_7298.jpg)
***PCB Cased***
