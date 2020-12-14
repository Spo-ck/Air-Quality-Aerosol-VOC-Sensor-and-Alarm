#include <M5StickC.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_SGP30.h"
#include <Adafruit_BME280.h>
#include <EEPROM.h>

// EEPORM
#define EEPROM_size       1
const int Mode_Register       = 0;
const int Activity_Register       = 1;
const int Sound_Register       = 2;

// Modes
#define serial_mode false // if false, no telemetry will be displayed on serial

// Sensors
// Sensor Objects
Adafruit_SGP30    sgp;
Adafruit_BME280   bme;
// Variables
float tvoc              = 0;
float eco2              = 0;
float raw_H2            = 0;
float raw_ethanol       = 0;
float temp              = 0;
float pressure          = 0;
float humidity          = 0;

// Memory
#define num_history 150
int   eco2_history[num_history];
int   history_postion   = 0;
int   history_mean      = 0;
int   history_mean_num  = 0;
unsigned long  history_mean_time    = 10000;  // Time that is used to calculate the mean over for 1 data point
unsigned long  history_mean_millis  = 0;

// Buzzer
int freq                = 2000;
int channel             = 0;
int resolution          = 8;
int buzzer_pin          = 26;

// LED
int LED_r               = 10;
bool LED_State          = false;

// Alarm
bool ALARM              = false;
long last_ALARM         = -60000;
int eCO2_Threshold      = 1000; // ppm
int tvoc_Threshold      = 150;  // ppb
String alarm_mode       = "sound";

// Display
String display_mode           = "diagram";
String display_mode_last      = "diagram";
String display_activity       = "5s_on";
String display_activity_last  = display_activity;
int    display_activity_time  = -1;
uint8_t display_brightness    = 255;

// Buttons
const int Button_A      = 37;
const int Button_B      = 39;

// RTC
// Struct
RTC_TimeTypeDef RTC_TimeStruct;
RTC_DateTypeDef RTC_DateStruct;
// Variables
int time_hh = RTC_TimeStruct.Hours;
int time_mm = RTC_TimeStruct.Minutes;
int time_ss = RTC_TimeStruct.Seconds;
unsigned long time_millis = 0;
unsigned long  loop_interval = 250;

///////////////////////////////////////////////////
void setup() {
    
  // Init Libraries
  Serial.begin(115200);
  m5.begin();
  Wire.begin();
  Serial.println();
  
  // EEPROM
  // Initialise EEEPROM
  EEPROM.begin(EEPROM_size);
//  // Flash EEPROM
//  EEPROM.write(Mode_Register, 0); // Write 0 to register 0
//  EEPROM.commit();
//  
//  EEPROM.write(Activity_Register, 0); // Write 0 to register 0
//  EEPROM.commit();
//  
//  EEPROM.write(Sound_Register, 1); // Write 0 to register 0
//  EEPROM.commit();
  // Read Out Mode From EEPROM and Change Mode
  int last_mode = EEPROM.read(Mode_Register);
  if (last_mode == 0){    
    display_mode = "air_quality";  
  }
  else  if (last_mode == 1){
    display_mode = "all_readings";
  }
  else  if (last_mode == 2){
    display_mode = "diagram";
  }
  display_mode_last = display_mode;

  last_mode = EEPROM.read(Activity_Register);
  if (last_mode == 0){    
    display_activity = "always_on";
  }
  else  if (last_mode == 5){
    display_activity = "5s_on";
  }
  display_activity_last = display_activity;

  last_mode = EEPROM.read(Sound_Register);
  if (last_mode == 0){    
    alarm_mode == "silent";
  }
  else  if (last_mode == 1){
    alarm_mode == "sound";
  }
  
  // Init Display
  M5.Lcd.setRotation(1);
  M5.Lcd.fillScreen(BLUE);
//  M5.Lcd.setBrightness(display_brightness); // Doesn't work with the Library
  
  // Init Sensors
  // SGP30
  if (! sgp.begin()) {
    if (serial_mode) {
      Serial.println("SGP30 not found");
      
      M5.Lcd.setTextColor(TFT_WHITE);
      M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(10, 5);
      M5.Lcd.print("Can't find Air Quality Sensor ");
    
    }
  }
  else {
    if (serial_mode) {
      Serial.println("SGP30 found");

      Serial.print("Found SGP30 serial #");
      Serial.print(sgp.serialnumber[0], HEX);
      Serial.print(sgp.serialnumber[1], HEX);
      Serial.println(sgp.serialnumber[2], HEX);
    }
    
    // Warm Sensor Up
    sgp.IAQmeasure();
    
    float last_eco2 = sgp.eCO2;
    float last_TVOC = sgp.TVOC;
  
    if (serial_mode) {
      Serial.println("Warming Up SGP30 Sensor");
    }
    
    sgp.IAQmeasure();

    M5.Lcd.setTextColor(TFT_WHITE);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(10, 5);
    M5.Lcd.print("Warming Up");
    M5.Lcd.setCursor(10, 25);
    M5.Lcd.print("Air Quality");
    M5.Lcd.setCursor(10, 45);
    M5.Lcd.print("Sensor ");
    M5.Lcd.setCursor(10, 65);
    
    while (last_eco2 == float(sgp.eCO2) && last_TVOC == sgp.TVOC) {
      sgp.IAQmeasure();

      if (serial_mode) {
        Serial.println("Warming Up SGP: eCO2 " + String(sgp.eCO2) + " ppm" + " TVOC " + String(sgp.TVOC) + " ppb");
      }
     }
  } 
  M5.Lcd.fillScreen(TFT_BLACK);
  
  // BME280
  if (! bme.begin(0x76, &Wire)) {
    if (serial_mode) {
      Serial.println("BMP280 not found");
    }
  }
  else {
    if (serial_mode) {
      Serial.println("BMP280 found");
        
      Serial.print("Temperature " + String(bme.readTemperature()) + "°C\t");
      Serial.print("Pressure " + String(bme.readPressure()) + " Bar\t");
      Serial.println("Humidity" + String(bme.readHumidity()) + "%");
    }
  }

  // FIl History
  for (int i = 0; i < 150; i++) {
    eco2_history[i] = -1;// default value
  }
  
  // Buzzer
  // buzzer
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(buzzer_pin, channel);
  ledcWrite(channel, 0);
  
  // LED
  // ROT, built in
  pinMode (LED_r, OUTPUT);
  digitalWrite(LED_r, HIGH);

  // Button
  pinMode(Button_A, INPUT_PULLUP);
  pinMode(Button_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(Button_A), Function_Button_A, HIGH);
  attachInterrupt(digitalPinToInterrupt(Button_B), Function_Button_B, HIGH);

  // RTC
  // Generate Struct
  RTC_TimeTypeDef TimeStruct;
  RTC_DateTypeDef DateStruct;

  TimeStruct.Hours    = 19;
  TimeStruct.Minutes  = 47;
  TimeStruct.Seconds  = 00;
  DateStruct.WeekDay  = 4;
  DateStruct.Date     = 10;
  DateStruct.Month    = 12;
  DateStruct.Year     = 2020;
  
//  // Set Time and Date
//  M5.Rtc.SetTime(&TimeStruct);
//  M5.Rtc.SetData(&DateStruct);

  // Get Time from RTC
  M5.Rtc.GetTime(&RTC_TimeStruct);
  time_hh     = RTC_TimeStruct.Hours;
  time_mm     = RTC_TimeStruct.Minutes;
  time_ss     = RTC_TimeStruct.Seconds;
}


////////////////////////
// Measurement
////////////////////////

void ALARM_LED(String State) {
  if (State == "Activate") {
    digitalWrite(LED_r, LOW);
    LED_State = true;
  }
  else if (State == "Deactivate") {
    digitalWrite(LED_r, HIGH);
    LED_State = false;
  }
  else if (State == "Change") {
    if (LED_State == false){
      digitalWrite(LED_r, LOW);
      LED_State = true;
    }
    else {
      digitalWrite(LED_r, HIGH);
      LED_State = false;
    }
  }
}

void ALARM_Buzzer() {
  
  M5.Lcd.fillScreen(RED);
  
  // Play Buzzer Pattern
  ledcWrite(channel, 100);  // Buzzer On
  wait_for(500);            // Wait 500ms
  ledcWrite(channel, 0);    // Buzzer Off
  wait_for(500);            // Wait 500ms
  ledcWrite(channel, 100);  // Buzzer On
  wait_for(500);            // Wait 500ms
  ledcWrite(channel, 0);    // Buzzer Off
  wait_for(500);            // Wait 500ms
  ledcWrite(channel, 100);  // Buzzer On
  wait_for(750);            // Wait 750ms
  ledcWrite(channel, 0);    // Buzzer Off

  M5.Lcd.fillScreen(TFT_BLACK);
}

void wait_for(int milliseconds){
  unsigned long alarm_time = millis();
  
  while(millis() - alarm_time < milliseconds){
    // Measurement
    sensor_readings();
    store_history();    // Save Readings in History
    display_readings();
  }
}

////////////////////////
// Measurement
////////////////////////
void sensor_readings() {
  // Measure
  // Sensor Reading
  if (! sgp.IAQmeasure()) {
    Serial.println("Measurement failed");
    return;
  }
  else {
    // BME280
    temp        = bme.readTemperature();
    pressure    = bme.readPressure();
    humidity    = bme.readHumidity();

    // SGP30
    sgp.setHumidity(getAbsoluteHumidity(temp, humidity));

    sgp.IAQmeasure();
    tvoc        = sgp.TVOC;
    eco2        = sgp.eCO2;

    sgp.IAQmeasureRaw();
    raw_H2      = sgp.rawH2;
    raw_ethanol = sgp.rawEthanol;
  }
}

uint32_t getAbsoluteHumidity(float temperature, float humidity) {
  // approximation formula from Sensirion SGP30 Driver Integration chapter 3.15
  const float absoluteHumidity = 216.7f * ((humidity / 100.0f) * 6.112f * exp((17.62f * temperature) / (243.12f + temperature)) / (273.15f + temperature)); // [g/m^3]
  const uint32_t absoluteHumidityScaled = static_cast<uint32_t>(1000.0f * absoluteHumidity); // [mg/m^3]
  return absoluteHumidityScaled;
}

void store_history() {


  if (history_mean_num  == 0) {
    history_mean_millis = millis();
  }

  if (history_mean_time > (millis() - history_mean_millis)) {
    
    if (history_mean_num == 0) {
       history_mean = eco2;
    }
    else {
      // Calculate Mean
      history_mean = history_mean * history_mean_num;
      history_mean = history_mean + eco2;
    }

    history_mean_num++;
    history_mean = history_mean / history_mean_num;
  }
  else {
    // Save History
    eco2_history[history_postion] = int(history_mean);
    
    history_mean_num = 0;
    
    history_postion++;

    eco2_history[history_postion] = -1;
    
    if (history_postion >= num_history) {
      history_postion = 0;
    } 
  }
}


////////////////////////
// Buttons
////////////////////////
void Function_Button_A() {

  if (display_activity != "always_on" && display_activity_time == -1) {
    // if the display is not always on, the time when the putton is pressed needs to be stored
    display_activity_time = millis();
 //   M5.Lcd.setBrightness(display_brightness); ; // Doesn't work with the Library
  }
  else if (display_activity != "always_on" && display_activity_time != -1) {
    change_mode();
  }
  else {
    change_mode();
  }
  

//  // Activate new Mode
//  display_readings();
// This line was written to update the display right after clicking, now it will just take some milliseconds to update the display<.
// otherwise it is likely, that the history array is open for writing at the same time when the interrupt comes
// in this case, the array will be read out, which is not possible when the controller is in writing mode,
// what would then trigger a reboot
}

void Function_Button_B() {
  M5.Lcd.fillScreen(TFT_BLACK);
  
  display_activity_last = display_activity;
  display_activity = "always_on";
  
  display_mode = "settings";

  detachInterrupt(digitalPinToInterrupt(Button_A));
  detachInterrupt(digitalPinToInterrupt(Button_B));
  attachInterrupt(digitalPinToInterrupt(Button_A), Function_Button_A1, HIGH);
  attachInterrupt(digitalPinToInterrupt(Button_B), Function_Button_B1, HIGH);

  M5.Lcd.drawLine(3, 0, 3, 25, RED);
}


void Function_Button_A1() {
  if (display_activity_last == "always_on") {
    display_activity_last = "5s_on";
  }
  else if (display_activity_last == "5s_on") {
    display_activity_last = "always_on";
  }
}

void Function_Button_B1() {
  detachInterrupt(digitalPinToInterrupt(Button_A));
  detachInterrupt(digitalPinToInterrupt(Button_B));
  pinMode(Button_A, INPUT_PULLUP);
  pinMode(Button_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(Button_A), Function_Button_A2, HIGH);
  attachInterrupt(digitalPinToInterrupt(Button_B), Function_Button_B2, HIGH);

  M5.Lcd.drawLine(3, 0, 3, 25, TFT_BLACK);
  M5.Lcd.drawLine(3, 33, 3, 55, RED);
}


void Function_Button_A2() {
  
  if (alarm_mode == "sound") {
    alarm_mode = "silent";
  }
  else if (alarm_mode = "silent") {
    alarm_mode = "sound";
  }
}

void Function_Button_B2() {
  display_mode = display_mode_last;
  display_activity = display_activity_last;

  M5.Lcd.fillScreen(TFT_BLACK);
    
  detachInterrupt(digitalPinToInterrupt(Button_A));
  detachInterrupt(digitalPinToInterrupt(Button_B));
  pinMode(Button_A, INPUT_PULLUP);
  pinMode(Button_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(Button_A), Function_Button_A, HIGH);
  attachInterrupt(digitalPinToInterrupt(Button_B), Function_Button_B, HIGH);
}

void change_mode() {
  M5.Lcd.fillScreen(TFT_BLACK);
  
  // Choose new Display Mode
  if (display_mode == "air_quality"){
    display_mode = "all_readings";
  }
  else if (display_mode == "all_readings"){
    display_mode = "diagram";
  }
  else if (display_mode == "diagram"){
    display_mode = "air_quality";    
  }
}


void Save_Mode_To_EEPROM() {
  if (display_mode == "air_quality"){
    EEPROM.write(Mode_Register, 0); // Write 1 to register 0
    EEPROM.commit();
  }
  else if (display_mode == "all_readings"){
    EEPROM.write(Mode_Register, 1); // Write 2 to register 0
    EEPROM.commit();
  }
  else if (display_mode == "diagram"){   
    EEPROM.write(Mode_Register, 2); // Write 0 to register 0
    EEPROM.commit();
  }

  display_mode_last = display_mode;
}

////////////////////////
// Communication
////////////////////////
void display_readings(){
  
  if (display_mode == "air_quality"){
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Lcd.setTextSize(2);
    
    M5.Lcd.setCursor(10, 5);
    M5.Lcd.print("eCO2:");
    M5.Lcd.setCursor(10, 25);
    M5.Lcd.print(eco2);
    M5.Lcd.println(" ppm   ");
  
    M5.Lcd.setCursor(10, 45);
    M5.Lcd.print("TVOC:");
    M5.Lcd.setCursor(10, 65);
    M5.Lcd.print(tvoc);
    M5.Lcd.println(" ppb   ");
  }
  else if (display_mode == "all_readings"){

    // Get Time from RTC
    M5.Rtc.GetTime(&RTC_TimeStruct);
    time_hh     = RTC_TimeStruct.Hours;
    time_mm     = RTC_TimeStruct.Minutes;
    time_ss     = RTC_TimeStruct.Seconds;
  
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Lcd.setTextSize(1);

    M5.Lcd.setCursor(10, 5);
    M5.Lcd.print("Time: ");
    M5.Lcd.setCursor(50, 5);
    M5.Lcd.print(time_hh);
    M5.Lcd.print(" : ");
    M5.Lcd.print(time_mm);
    M5.Lcd.print(" : ");
    M5.Lcd.print(time_ss);
  
    M5.Lcd.setCursor(10, 20);
    M5.Lcd.print("eCO2: ");
    M5.Lcd.setCursor(50, 20);
    M5.Lcd.print(eco2);
    M5.Lcd.println(" ppm   ");
  
    M5.Lcd.setCursor(10, 35);
    M5.Lcd.print("TVOC: ");
    M5.Lcd.setCursor(50, 35);
    M5.Lcd.print(tvoc);
    M5.Lcd.println(" ppb   ");
    
    M5.Lcd.setCursor(10, 50);
    M5.Lcd.print("Temp: ");
    M5.Lcd.setCursor(50, 50);
    M5.Lcd.print(temp);
    M5.Lcd.println(" C   ");

    M5.Lcd.setCursor(10, 65);
    M5.Lcd.print("Hum: ");
    M5.Lcd.setCursor(50, 65);
    M5.Lcd.print(humidity);
    M5.Lcd.println(" %   ");

    M5.Lcd.setCursor(115, 50);
    M5.Lcd.print("Bat: ");
    M5.Lcd.setCursor(115, 65);
    if (M5.Axp.GetWarningLevel()) {
      M5.Lcd.setTextColor(TFT_WHITE, RED);
    }
    float bat_level = ((M5.Axp.GetBatVoltage() - 3.00) / 0.9) * 100;
    if (bat_level > 100) {
      M5.Lcd.print(100);
    }
    else {      
      M5.Lcd.print(bat_level);
    }
    
    M5.Lcd.println(" %");
  }
  else if (display_mode == "diagram"){
    // Display Axis
    M5.Lcd.drawLine(5, 76, 160, 76, TFT_WHITE);  // X-Axis
    M5.Lcd.drawLine(5, 76, 5, 14, TFT_WHITE);    // Y-Axis

    // Display Text-Value
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Lcd.setTextSize(1);
    
    M5.Lcd.setCursor(5, 4);
    M5.Lcd.print("eCO2: ");
    M5.Lcd.setCursor(40, 4);
    M5.Lcd.print(eco2);
    M5.Lcd.println(" ppm   ");
    
    // Print Bars
    int actual_pos = history_postion;

    for (int i = num_history - 1; i >= 0 ; i = i - 1) {
      // Check every time if the mode is still diagram or if this changed
      // Otherwise there will be fragments on the next visualisation page on the display
      if (display_mode == "diagram"){        
         // CHeck if there is something in the memory to display (default is -1)
         if (eco2_history[actual_pos] >= 0.00) {
            
            if (eco2_history[actual_pos] < eCO2_Threshold) {   // Values below Threshold will be green
              M5.Lcd.drawLine(i + 8, ((75 - (eco2_history[actual_pos] / (1.5 * eCO2_Threshold)) * 62) - 1), i + 8, 14, BLACK);
              M5.Lcd.drawLine(i + 8, 75, i + 8, (75 - (eco2_history[actual_pos] / (1.5 * eCO2_Threshold)) * 62), GREEN);
            }
            else if (eco2_history[actual_pos] >= eCO2_Threshold && eco2_history[actual_pos] < (1.5 * eCO2_Threshold)){   // Values above Threshold will be red
              M5.Lcd.drawLine(i + 8, ((75 - (eco2_history[actual_pos] / (1.5 * eCO2_Threshold)) * 62) - 1), i + 8, 14, BLACK);
              M5.Lcd.drawLine(i + 8, 75, i + 8, (75 - ((eco2_history[actual_pos] / (1.5 * eCO2_Threshold)) * 62)), RED);
            }
            else {  // if the value is greater 1.5*eCO2_Treshold, there will only be one red bar displayed
              M5.Lcd.drawLine(i + 8, 75, i + 8, 15, RED); 
            }
          }
          
          actual_pos++;
          
          if (actual_pos >= num_history) {
            actual_pos = 0;
          } 
      }
    }
  }
  else if (display_mode == "settings"){
    
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Lcd.setTextSize(1);

    M5.Lcd.setCursor(10, 5);
    M5.Lcd.print("Display Activity: ");
    M5.Lcd.setCursor(10, 15);
    M5.Lcd.print(String(display_activity_last) + "     ");
  
    M5.Lcd.setCursor(10, 35);
    M5.Lcd.print("Alarm Sound:   ");
    M5.Lcd.setCursor(10, 45);
    M5.Lcd.print(String(alarm_mode) + "     ");    
  }
}

void print_readings() {
  // Print
  Serial.print("TVOC: " + String(tvoc) + " ppb\t");
  Serial.print(" | eCO2 " + String(eco2) + " ppm\t");
  Serial.print(" | Raw H2: " + String(raw_H2) + "\t");
  Serial.print(" | Raw Ethanol: " + String(raw_ethanol) + "\t");
  Serial.print(" | Temperature: " + String(temp) + "°C\t");
  Serial.print(" | Pressure: " + String(pressure) + " Bar\t");
  Serial.println(" | Humidity: " + String(humidity) + "%");
}

////////////////////////
// Main Loop
////////////////////////
void loop() {
  
  time_millis = millis();
  
  // Measurement
  sensor_readings();  // Sensor Reading
  store_history();    // Save Readings in History

  // Sensor Display
  // Serial
  if (serial_mode == true) {
    print_readings();
  }
  // Display
  if (display_activity == "always_on") {
    display_readings();
  }
  else if (display_activity == "5s_on") {
    
    if (millis() - display_activity_time < 5000 && display_activity_time != -1) {
      display_readings();
    }
    else if (millis() - display_activity_time >= 5000 && display_activity_time != -1) {
      M5.Lcd.fillScreen(TFT_BLACK);
//      M5.Lcd.setBrightness(0); ; // Doesn't work with the Library
      display_activity_time = -1;
    }    
    else if (eco2 > (eCO2_Threshold * 0.8) || tvoc > (tvoc_Threshold * 0.8)) {
      if (display_activity_time == -1) {
        display_activity_time = millis();
      }
    }
  }

  // Check for Alarms
  // Visual Alarm everytime the eCO2 is over Threshold
  // Audio Alarm one time in 60s
  if (ALARM == false) {                                   // Check if an Alarm was triggered Previously
    if (eco2 > eCO2_Threshold || tvoc > tvoc_Threshold) { // Check if measurement exceeds threshold
      ALARM = true;
      ALARM_LED("Activate");
  
      if (millis() - last_ALARM > 60000 && alarm_mode == "sound") {
        ALARM_Buzzer();
        last_ALARM = millis();
      }
    }
  }
  else if (eco2 <= eCO2_Threshold && tvoc <= tvoc_Threshold && ALARM == true) {
    ALARM_LED("Deactivate");
    
    ALARM = false;
  }
  
  // if the mode has changed for the display, store this in eeprom
  if (display_mode_last != display_mode && display_mode != "settings") {
    Save_Mode_To_EEPROM();
  }

  Serial.print(M5.Axp.GetBatVoltage());
  Serial.print(" - ");
  Serial.print(M5.Axp.GetAPSVoltage());
  Serial.print(" - ");
  Serial.println(M5.Axp.GetWarningLevel());
  
  // Just run the loop 4 times per second
  if (millis() - time_millis < loop_interval) {
//    Serial.println(millis() - time_millis);
    delay(loop_interval - (millis() - time_millis));
  }
}
