```c
/*
 * Title:   Sensing Digital Recorder
 * Author:  Elizabeth S. Robert
 * Created: October 15, 2015
 * 
 * Description: Utilizes Arduino Duemilanove and environmental triggers (temperature, proximity, and EMF) 
 *              to start the digital recorder for the purpose to capture EVP or Electronic Voice 
 *              Phenomenon. The digital recorder has three modes of operation: Sensing, Recorder Only 
 *              (or Manual Mode), and No Mode. This mode is selectable via a slide switch on the device.
 *              Indicator lights let the user know what mode the device is currently in, whether or not 
 *              the device is recording, and also which sensor was triggered when in Sensing Mode.
 *              Recording of ISD1932 allows for 1 message at 20 seconds long which also applies to
 *              playback length. Temperature is triggered with a +/- 3 degree change. 
 *              
 *              Design Note: EMF detection was programmed, but did not make it to the initial design 
 *              of the sensing recorder. For this reason it was commented out of the code.
 * 
 * Design Image: https://www.dropbox.com/s/oi4s3sw3rabju4y/IMG_6741.JPG?dl=0
 * Design Video: https://www.dropbox.com/s/nb2yvg29ypgz4ho/Sensing%20Digital%20Recorder.mov?dl=0
 * 
 * Major Circuit Components:
 *    - Arduino Duemilanove
 *    - DHT22 Sensor
 *    - PING))) Ultrasonic Sensor
 *    - SparkFun Voice Recorder Breakout - ISD1932
 *    - 8 Ohm Speaker
 *    - Indicator LEDs, Momentary Push Buttons (3), and Slide Switch
 * 
 * Libraries Used:
 *    - DHT.h (DHT Sensor Library)
 */

//DHT Library Sensor Includes and Defines=====
#include <DHT.h>
#define DHTPIN 5
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

//Variable Declarations=======================
double tempCheckA = 0;
double tempCheckB = 0;
int tval = 0;
int emfPin = 1;
int emfVal = 0;
int recordingCaptured = 0;
const int pingPin = 7;
const int resetButton = 9;
float f = 0;
//LED Sensor Association
int ledSensor = 0;

//SETUP=======================================
void setup() {
  //ISD1932 Pin Modes
  pinMode(8,OUTPUT);  //P/R
  pinMode(6,OUTPUT);  //M1
  digitalWrite(8,HIGH); //Initially set to playback
  digitalWrite(6,HIGH); //Initially set to inactive

  //Indicator Lights Pin Modes
  pinMode(2,OUTPUT); //RED LED Recording Status Black Wire
  pinMode(3,OUTPUT); //BLUE LED SENSOR MODE ACTIVATED Purple Wire
  pinMode(4,OUTPUT); //GREEN LED RECORDER MODE ACTIVATED Gray Wire
  digitalWrite(2,LOW);
  digitalWrite(3,LOW);
  digitalWrite(4,LOW);

  //External Buttons Pin Modes
  pinMode(resetButton,INPUT);   //RESET
  pinMode(10,INPUT);  //Sensor Trigger Mode
  pinMode(11,INPUT);  //Recorder Only Mode
  pinMode(12,INPUT);  //Record Button White Wire
  pinMode(13,INPUT);  //Playback Button Orange Wire
  
  // Uncomment for Debugging Purposes
  //Serial.begin(9600);
  //dht.begin();

  // Uncomment to take initial temperature readings
  //Read temperature as Fahrenheit (isFahrenheit = true)
  //float f = dht.readTemperature(true);

  // Throw this initial value into tempCheck
  f = dht.readTemperature(true);
  Serial.println("New Temp Acquired");
}

//LOOP========================================
void loop() {

  //As long as a recording has not been captured yet,
  //you are still able to select a different mode of operation
  if(recordingCaptured == 0){
    //Pin 10 is the slide switch position for Sensing Mode
    if(digitalRead(10)==1){
      Serial.println("Sensor Trigger Mode");
      delay(5000);  //Prepare to step away from box
      sensorMode(); //Send to sensor mode code
    }
    //Pin 11 is the slide switch position for Sensing Mode
    if(digitalRead(11)==1){
      Serial.println("Recorder Only Mode");
      recorderMode(); //Send to recorder mode code
    }
    //Slide switch position is in the center indicating
    //that No Mode has been selected yet
    if(digitalRead(10)==0 && digitalRead(11)==0){
      Serial.println("No Mode Selected");
      noMode(); //Send to no mode state with pretty flashing lights!
    }
  }

  //If recording has been captured device will
  //wait until reviewed by the user
  if(recordingCaptured == 1){
    //Hold circuit here until playBack
    messageWaiting();
  }
}

//SENSOR=MODE=================================
//Utilizes the temperature, proximity, and EMF sensors
void sensorMode(){
  //While in sensor mode Blue indicating LED will stay on
  digitalWrite(3,HIGH); //BLUE LED ON
  digitalWrite(4,LOW);  //GREEN LED OFF
  Serial.println("SENSOR MODE ACTIVATED");

  //Digital Recorder Initial Settings
  digitalWrite(8,HIGH); //Initially set to playback
  digitalWrite(6,HIGH); //Initially set to inactive

  // Uncomment to enable operation of EMF sensor
  //EMF SENSOR
  //delay(50);
  //Read EMF Pin
  //analogReference(INTERNAL);
  //emfVal = analogRead(emfPin);
  //Serial.print("EMF PIN: ");
  //Serial.println(emfVal);
  //delay(50);
  //EMF Recording
  //if (emfVal > 4 && emfVal < 100){
    //ledSensor = 1;
    //startRecording();
  //}

  //ULTRASONIC SENSOR
  // Variables for duration of the ping
  // and the distance result in inches and centimeters
  long duration, inches, cm;
  
  // PING))) is triggered by a HIGH pulse of 2 or more microseconds
  // Send a short LOW pulse to ensure a clean HIGH pulse:
  pinMode(pingPin,OUTPUT);
  digitalWrite(pingPin,LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin,HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin,LOW);
  
  // Read the signal from PING)))
  // High pulse time (in uS) from sending ping to echo from object
  pinMode(pingPin,INPUT);
  duration = pulseIn(pingPin,HIGH);
  
  // Convert the time into a distance
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);

  // Information for Debugging and Setting desired sensor distance
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();

  //Ultrasonic Recording Triggered if less than 12 inches away
  if(inches < 12){
    //This is let the user know which sensor was set off
    //This number indicates the number of times the LED code
    //Will loop in messageWaiting()
    ledSensor = 1;
    startRecording(); //Device will start recording
  }
  
  //TEMPERATURE AND HUMIDITY
  // READ DATA
  tempCheckA = f; //For comparison of value
  
  // Note: Reading takes approx. 250 mS and readings up to 2 S old
  // **Replace this sensor for a faster one**
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  //Throw value into tempCheck for monitoring of changing temperature
  tempCheckB = f;

  // Serial out of temperature readings for debugging
  Serial.println(tempCheckA-3);
  Serial.println(tempCheckA+3);
  Serial.println(tempCheckB);
  
  //If a temperature increase of +3 degrees occurs start recording
  if (tempCheckA > 0){
    if (tempCheckB > (tempCheckA+3)){
        Serial.println("TEMP INCREASE");
        //This is let the user know which sensor was set off
        //This number indicates the number of times the LED code
        //Will loop in messageWaiting()
        ledSensor = 2;
        startRecording();
    }
  }

  //If a temperature increase of -3 degrees occurs start recording
  if (tempCheckA > 0){
    if (tempCheckB < (tempCheckA-3)){
        Serial.println("TEMP DECREASE");
        //This is let the user know which sensor was set off
        //This number indicates the number of times the LED code
        //Will loop in messageWaiting()
        ledSensor = 3;
        startRecording();
    }
  }
  
  //DHT Sensor: Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  //Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  //Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  //Output information to Serial
  Serial.print("Temperature: ");
  Serial.println(f);

  delay(1000);

  //Playback audio if button pressed
  if (digitalRead(13)==0){
    playBack();
  }
}

//RECORDER=MODE===============================
//Also considered manual mode. Utilizes momentary push
//buttons for operation
void recorderMode(){
  // Uncomment for debugging purposes
  //Slide switch was acting up in some cases. 
  //Might need to find a new part.
  //Serial.print("Digital Pin Mode:");
  //Serial.print(digitalRead(10));
  //Serial.print("& ");
  //Serial.println(digitalRead(11));

  //Indicator LED Green LED light ON letting user know
  //we are now in Recording mode. 
  digitalWrite(3,LOW); //BLUE LED OFF
  digitalWrite(4,HIGH);  //GREEN LED ON
  Serial.println("RECORDER MODE ACTIVATED");
  delay(1000);

  //Pin 12 is the recording button
  if(digitalRead(12) == 0){
    recordingCaptured = 1; //Code flag
    digitalWrite(2,HIGH); //LED RED ON
    //Digital Recorder set to record
    digitalWrite(8,LOW);  
    digitalWrite(6,LOW);  
    //Digital Recorder now recording for 20 seconds
    Serial.println("Recording for 20 seconds");
    delay(20000);
    //Digital Recorder done recording
    Serial.println("Done recording");
    digitalWrite(2,LOW); //LED RED OFF
    //Digital Recorder set to intial state
    digitalWrite(8,HIGH); //Initially set to playback
    digitalWrite(6,HIGH); //Initially set to inactive
  }

  //This is let the user know which sensor was set off
  //This number indicates the number of times the LED code
  //Will loop in messageWaiting()
  ledSensor = 3;

  //Send back to loop
  loop();

  //Ability to Reset Digital Recorder to capture new recording
  if (digitalRead(resetButton)==0){
      recordingCaptured = 0;
      ledSensor = 0;
      loop();
  }
  
  //Playback recording
  //if (digitalRead(13)==0){
    //playBack();
  //}
}

//START=RECORDING=============================
void startRecording(){
  //Tell serial we are recording
  Serial.println("Device is now recording");
  recordingCaptured = 1;  //Code check
  tempCheckA = 0;   //Code check
  //Turn RED LED on to signify we are recording
  digitalWrite(2,HIGH); //LED RED ON
  delay(500);
  //Tell digital recorder to start recording
  digitalWrite(8,LOW);
  digitalWrite(6,LOW);
  //Record for 20 seconds
  Serial.println("Recording for 20 seconds");
  delay(20000);
  Serial.println("Done recording");
  digitalWrite(2,LOW); //LED RED OFF
  //Tell digital recorder to stop recording
  digitalWrite(8,HIGH); //Initially set to playback
  digitalWrite(6,HIGH); //Initially set to inactive
  //Send back through loop
  loop();
}

//PLAYBACK=RECORDING=========================
void playBack(){
    //Playback Pin is Pressed
    delay(1000);
    //Tell digital recorder to playback message
    digitalWrite(8,HIGH);
    digitalWrite(6,LOW);
    Serial.println("Playback for 20 seconds");
    delay(20000);
    Serial.println("Done with plackback");
    //Set digital recorder to original state
    digitalWrite(8,HIGH); //Initially set to playback
    digitalWrite(6,HIGH); //Initially set to inactive

    //The ability to reset the digital recorder 
    //in playback mode
    if(digitalRead(resetButton)==0){
      recordingCaptured = 0;
      ledSensor = 0;
      loop();
    }
    
    messageWaiting();
}

//MESSAGE=WAITING=========================
void messageWaiting(){
    //blink LEDs if a message is capturedcaptured
    Serial.println("MESSAGE WAITING");
    delay(2000);

    //ledSensor value is determined throughout the code
    //based on which sensor was triggered or mode we are in
    //This lets the user know how the device was triggered
    for (int loopTimes = 0; loopTimes < ledSensor; loopTimes++){
      
      digitalWrite(4,LOW);  //GREEN LED OFF
      delay(500);
      digitalWrite(4,HIGH);  //GREEN LED ON
      delay(500);
      digitalWrite(4,LOW);  //GREEN LED OFF
      delay(500);
      digitalWrite(4,HIGH);  //GREEN LED ON
      delay(500);
      digitalWrite(4,LOW);  //GREEN LED OFF
    }

    //Playback recording
    if (digitalRead(13)==0){
      playBack();
    }
    //Give the user the ability to reset the device
    //and capture a new recording
    if (digitalRead(resetButton)==0){
      recordingCaptured = 0;
      ledSensor = 0;
      loop();
    }
}

//NO=MODE=OPERATION=========================
void noMode(){
  //Device will just flash all of the LEDs
  //to let user know that no mode has been 
  //selected yet. 
  digitalWrite(3,LOW);  //BLUE LED OFF
  digitalWrite(4,LOW);  //GREEN LED OFF
  digitalWrite(2,HIGH);  //RED LED ON
  delay(500);
  digitalWrite(3,HIGH);  //BLUE LED ON
  digitalWrite(4,HIGH);  //GREEN LED ON
  digitalWrite(2,LOW);  //RED LED OFF
  delay(500);
  digitalWrite(3,LOW);  //BLUE LED OFF
  digitalWrite(4,LOW);  //GREEN LED OFF
  digitalWrite(2,HIGH);  //RED LED ON
  delay(500);
  digitalWrite(3,HIGH);  //BLUE LED ON
  digitalWrite(4,HIGH);  //GREEN LED ON
  digitalWrite(2,LOW);  //RED LED OFF
  delay(500);
}

long microsecondsToInches(long microseconds){
  // According to datasheet for PING))) 
  // 73.746 microseconds per inch
  // Divide by 2 to get the distance of the obstacle.
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds){
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // Distance of the object is half of the distance travelled.
  return microseconds / 29 / 2;
}

```
