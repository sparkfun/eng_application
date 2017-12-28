/*
 * 
 * Storm Alert
 * 
 * A mobile barometer designed for notification of rapidly approaching storms in the mountains.
 * Uses GPS to obtain elevation data to compensate for dropping pressure during altitude gain.
 * Stores two sets of pressure data and looks for a rapid drop in pressure.
 * 
 * NOTE: There is a threshold set for the pressure drop to determine when to alert for a potential storm.
 * This threshold value is experimental and might not be correct. 
 * One should not rely on this device to keep them from dangerous weather.
 * 
 * Hardware:
 * Arduino Nano (or compatible microcontroller)
 * NEO-6M compatible gps sensor. Tx to pin 8, Rx to pin 9
 * BMP180 barometer/thermometer. SCL to pin A5. SDA to pin A4;
 * SSD1306 128X64 compatible SPI OLED screen. Clock/D0 to pin D3, Data/D1 to pin D2, CS to pin D5, DC to pin D4, Reset to pin D13);
 * Active buzzer with PN2222 transister connected to pin D6. PN2222 connections: Emitter to +5V, Base to Arduino D6, Collecter to to active buzzer
 * 
 * For more info on the hardware connections, checkout the wiring diagram.
 * 
 * 
 * Released under the MIT license. Have fun!
 * 
 * V1.0 Halcy Webster - 16DEC2017
 * 
 * TODO list:
 *    Verify accuracy of pressure drop threshold
 *    Low power mode
 *    Monitor battery level
 *    Save records to EEPROM
 *    Button to silence alarm
 *    Be able to view past pressure readings
 *    
 * 
 */


//Let's include our libraries


#include <Wire.h> // For I2C with the BMP180 sensor
#include <SFE_BMP180.h> // For the barometer

#include <AltSoftSerial.h> // To communicate with the GPS
#include <TinyGPS++.h> // To read the GPS data

#include <SPI.h> // For SPI communication with the OLED
#include <U8g2lib.h> // For displaying data to the OLED

// Cool, now we need to create some objects

//Barometer Setup
SFE_BMP180 bmp180; // Whenever we are talking about the barometer sensore, we will call it bmp180

//GPS Setup
AltSoftSerial gpsSerial; // Serial communication on pins D8, D9;
TinyGPSPlus gps; // We can input the GPS NMEA data to this and get more accessible info

// OLED screen Setup
U8G2_SSD1306_128X64_NONAME_1_4W_SW_SPI u8g2(U8G2_R0, /* clock = pin D*/ 3, /* data = pin D*/ 2, /* cs = pin D*/ 5, /* dc = pin D*/ 4, /* reset = pin D*/ 13);


// We are going to use some state machines, so here we define the possible state
enum mainStates { START_TEMP, READ_TEMP, START_PRESSURE, READ_PRESSURE, RECORD_DATA, UPDATE_DISPLAY };
enum displayStates { SHOW_DATA, ALARM };

mainStates state = START_TEMP;
displayStates displayState = SHOW_DATA;


// For our state machines, we want to do things after certain time periods. 
// We will store the time elapsed between state changes in these variables
uint32_t mainStateTime, 
         displayStateTime, 
         recordTimer, 
         buzzerTimer;

// We only want to record new data once per hour, so recordDelay stores how many milliseconds are in an hour
double recordDelay = (double) 1000 * (double) 60 * (double) 60/*millisecond/second * seconds/minute * minutes/hour = 1 hour*/;

unsigned int mainDelayTime = 1000, 
             subDelayTime = 250, 
             buzzerDelay = 500;


// We will use this variable to know if the buzzer is already on or not
bool buzzerState = false;
char buzzer = 6; // The buzzer is connected to pin D6;


// We only want to read the pressure if we have accurate and fresh data from the GPS
bool newGpsData = false;
// For storing the elevation we get from the GPS
double elevation; 


// Our barometer readings will require some variables
char bmpResult;
double temperature, 
       absPressure /*Absolute Pressure*/, 
       pressure /*Relative Pressure*/;


// And finally we need some variables for storing our pressure tracking.
double dataRecord, prevRecord;




void setup() {
  Serial.begin(9600);
  u8g2.begin(); //Start up our OLED
  
  u8g2.setFont(u8g2_font_profont17_tr); // font size 4

  // UPDATE_DISPLAY "Initializing..." on the screen
  u8g2.firstPage();
  do {
    u8g2.setCursor(5, 10);
    u8g2.print(F("Initializing..."));
  } while ( u8g2.nextPage() );

  // Start up the BMP180
  if (bmp180.begin()){
    Serial.println(F("BMP180 init success"));
  }else{
    Serial.println(F("BMP180 init fail\n\n"));
    while(1); // Our program is useless without the barometer sensor, so just stop here if it isn't working
  }
  
  gpsSerial.begin(9600); // Start up our GPS
  
}

void loop() {
  
  while(gpsSerial.available() > 0){ // Is there GPS data ready for us? Let's read it then
    if (gps.encode(gpsSerial.read())){
      if (gps.location.isValid()){ // But we only want to use the data if it is valid
        
        elevation = gps.altitude.meters(); // Saving the elevation for later
        newGpsData = true; // And marking that we got some new GPS data
        
      }else{
        Serial.println(F("INVALID LOCATION"));
      }
    }
  }

  
  /*
   * The main state machine
   * 
   * This state machine is pretty important and there is a reason we are doing only one of these states per loop.
   * The GPS is sending new data over the serial communications at about 1Hz. 
   * We can't really handle that data while we are doing other things.
   * But it works out because there is a buffer that saves it up while we do our business.
   * But there is also a problem. If the buffer is too full, it will fail and mess up our data.
   * And during the time it takes to read the temperatue/pressure, update the display, and etc, the buffer will overfill
   * So we only have time to do parts of those tasks between repeating the loop so we can clear the GPS serial buffer.
   * 
   * The rhythm of this state machine is: 
   *    Start the temperature reading
   *    Read the temperature data
   *    Start the pressure reading
   *    Read the pressure data
   *    Check to see if it is time to store a new pressure record
   *    Update the display
   *    And do it all over again
   */
  
  switch (state) {
    case START_TEMP:
      if (millis() - mainStateTime >= mainDelayTime) { 
        // This will be repeated for each state. We only want to move on if it is time to move on
        
        
        Serial.println(F("START_TEMP"));
        
        // Starting the temperature reading will return how much time it needs to complete the reading
        mainDelayTime = bmp180.startTemperature(); 
        mainStateTime = millis();
        state = READ_TEMP;
      }
      break;
    case READ_TEMP:
      if (millis() - mainStateTime >= mainDelayTime) {
        Serial.println(F("READ_TEMP"));
        
        // Store the temperatue in the variable "temperature"
        // bmpResult will tell us if we actually got a temperature reading or not. 1 = success and 0 = failure
        bmpResult = bmp180.getTemperature(temperature);
        if (bmpResult != 0){
          Serial.print(F("temperature: "));
          Serial.print(temperature,2); // We get the temperature in C, but we can convert it to F
          Serial.print(F(" deg C, "));
          Serial.print((9.0/5.0)*temperature+32.0,2); // Converting to F
          Serial.println(F(" deg F"));
          
        }else Serial.println(F("error retrieving temperature measurement\n"));
        mainStateTime = millis();
        state = START_PRESSURE;
      }
      break;
    case START_PRESSURE:
      if (millis() - mainStateTime >= 1000) {
        Serial.println(F("START_PRESSURE"));

        // Starting the pressure reading will return how much time it needs to complete the reading
        // The parameter for "startPressure" tells it how much oversampling it should do. 
        // 0 is the lowest (quick but less accurate) and 3 is the highest (slow and precise)
        mainDelayTime = bmp180.startPressure(3);
        mainStateTime = millis();
        state = READ_PRESSURE;
      }
      break;
    case READ_PRESSURE:
      if (millis() - mainStateTime >= mainDelayTime) {
        Serial.println(F("READ_PRESSURE"));
        
        
        // Time to get the pressure reading.
        // The absolute pressure (not adjusted for elevation) will be stored in the variable "absPressure"
        // But we need to tell it the temperature, so we use the temperature reading we got a little bit ago
        // bmpResult will tell us if we actually got a pressure reading or not. 1 = success and 0 = failure
        bmpResult = bmp180.getPressure(absPressure, temperature);
        
        if (bmpResult != 0){
          
          Serial.print(F("absolute pressure: "));
          Serial.print(absPressure,2);
          Serial.print(F(" mb, ")); // the BMP180 returns the pressure in mb, but we can convert it to inHg.
          Serial.print(absPressure*0.0295333727,2);
          Serial.println(F(" inHg"));

          
          // But since this is designed for mountain use, we need to adjust for changes in pressure from changes in elevation
          // So in the parameters for "bmp180.sealevel" we will tell it the absolute pressure and our elevation (which we got from the GPS)
          // And it will return the air pressure relative to sea level
          pressure = bmp180.sealevel(absPressure, elevation);
          Serial.print(F("relative (sea-level) pressure: "));
          Serial.print(pressure,2);
          Serial.print(F(" mb, "));
          Serial.print(pressure*0.0295333727,2);
          Serial.println(F(" inHg"));

        }else Serial.println(F("error retrieving pressure measurement\n"));

        mainStateTime = millis();
        state     = RECORD_DATA;
        mainDelayTime  = 100;
      }
      break;
    case RECORD_DATA:
      if (millis() - mainStateTime >= mainDelayTime ) {

        if(millis() - recordTimer >= recordDelay || !prevRecord){
          // We don't want to record new data every time we read the barometer. So let's just do it once per hour.
          // And we will add the "|| !prevRecord" to also make a record if we don't have enough data stored yet.
          
          Serial.println(F("RECORD_DATA"));

          // Since we want to compare the pressure between now and an hour ago. So let's save our last pressure record
          prevRecord = dataRecord; 
          
          dataRecord = pressure*0.0295333727; // Saving the pressure in inHg
          
          
          Serial.print(F("Previous Pressure: "));
          Serial.println(prevRecord);
          Serial.print(F("Current Pressure: "));
          Serial.println(dataRecord);

          recordTimer = millis(); // And reset the timer so our next record is in an hour

          // Storms generally only come in low pressure systems
          // so if the pressure is above 29.8inHg it is not a low pressure system
          // And we only want to check for a drop if we have two points of data
          // And if the pressure drops by .15 inHg in an hour, that is a sign of a storm on the way
          // NOTE: the rate of .15 inHg/hr is currently under evaluation. This number might need tweaking
          if(dataRecord <= 29.8 && prevRecord > 1 && prevRecord - dataRecord >= .15){
            Serial.println(F("Potential storm incoming"));
            displayState = ALARM; // Instead of just showing the regular data, go into alarm mode
            subDelayTime = 20000; // for 20 seconds
          }
        }
        
        mainStateTime = millis();
        state = UPDATE_DISPLAY;
        mainDelayTime = 100;
      }
      break;
    case UPDATE_DISPLAY:

      // We have a sub-state machine for showing either the regular sensor data
      // Or the alarm if a pressure drop was detected
      
      switch(displayState){
        case SHOW_DATA:
          u8g2.firstPage();
          do {
            // Show the relative pressure. 
            // This is very useful because someone might want to take their own pressure readings more often
            u8g2.setCursor(5, 10);
            u8g2.print(F("P: "));
            u8g2.print(pressure*0.0295333727,2);
            u8g2.print(" inHg");

            // Show the temperature in deg F
            u8g2.setCursor(5, 30);
            u8g2.print(F("T: "));
            u8g2.print((9.0/5.0)*temperature+32.0,2);
            u8g2.print(F(" deg F"));

            // Show the elevation in meters
            u8g2.setCursor(5, 60);
            u8g2.print(F("E: "));
            u8g2.print(elevation);
            u8g2.print(F(" m"));
            
            
          } while ( u8g2.nextPage() );
          break;
        case ALARM:

          // A potential storm was detected. Alert the user.
          u8g2.firstPage();
          do {
            u8g2.setCursor(5, 15);
            u8g2.print(F("Storm Alert"));

            // Showing the pressure difference can help them determine the strength and speed of the storm
            u8g2.setCursor(5, 30);
            u8g2.print(F("P1:"));
            u8g2.print(prevRecord);
            u8g2.print(F(" inHg"));
            
            u8g2.setCursor(5, 60);
            u8g2.print(F("P2:"));
            u8g2.print(dataRecord);
            u8g2.print(F(" inHg"));

  
            // Turn the buzzer on and off
            // Remember, we don't want to use delay() because it could cause the GPS serial buffer to overfill
            if(!buzzerState && millis() - buzzerTimer >= buzzerDelay){
              digitalWrite(buzzer, HIGH);
              buzzerTimer = millis();
              buzzerState = true;
            }
            if(buzzerState && millis() - buzzerTimer >= buzzerDelay){
              digitalWrite(buzzer, LOW);
              buzzerTimer = millis();
              buzzerState = false;
            }

            
            if (millis() - displayStateTime >= subDelayTime ) {
              // Ok, the alarm has been on long enough. Let's not be annoying
              digitalWrite(buzzer, LOW);
              buzzerState = false;
              displayStateTime = millis();
              displayState     = SHOW_DATA;
              subDelayTime = 2000;
            }
          } while ( u8g2.nextPage() );
          break;
      }
      if (millis() - mainStateTime >= mainDelayTime ) {
        
        // It is time to start the state machine back at the beginning. 
        // Let's make sure we get fresh GPS data
        newGpsData = false;
        mainStateTime = millis();
        state     = START_TEMP;
        mainDelayTime = 1000;
      }
      break;
  }

}
