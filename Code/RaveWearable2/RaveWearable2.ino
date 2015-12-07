/*
Alex Klinger

Sound reactive top

This is code for a simple sound reactive wearable I made in collaboration with Sabina Hayutin. 

We worked together on the top which can be seen here: http://alexklinger.tumblr.com/post/121792518619/sound-reactive-top-collaboration-with-sabina
Physical construction of the top was collaborative, while the code was written entirely by me. 

I started with the basic analog input tutorial from arduino and built from there https://www.arduino.cc/en/Tutorial/AnalogInput

 */
 
 
/*

The top has seven rows of LEDs in a "V" formation along the neckline. Each row has two LEDs, one on both the left and right sides. 
Each row is attached to a differnt pin, which allows the LEDs to react like a Volume meter.
As higher values are read by the microphone, more rows are of LEDs are lit up. 

*/

// Everything attached to the pins on a LilyPad arduino is listed below:
int micPin = A5; // a SparkFun Electret Microphone is attached to pin A5 on the LilyPad Arduino. 
int ledPin = 5; // the two LEDs at the bottom of the top are attached here.
int ledPin2 = 6; // the next row of LEDs from the bottom are attached here. 
int ledPin3 = 9; // the next row of LEDs are attached here. 
int ledPin4 = 10; // the next row of LEDs are attached here. 
int ledPin5 = 11; // the next row of LEDs are attached here. 
int ledPin6 = A2; // the next row of LEDs are attached here. 
int ledPin7 = A3; // the top row of LEDs are connected here.

int micValue = 0; // this variable is used to store values from the microphone

void setup() {
  //setting different pins to OUTPUT:
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
  pinMode(ledPin5, OUTPUT);
  pinMode(ledPin6, OUTPUT);
  pinMode(ledPin7, OUTPUT);
   
  Serial.begin(9600);  
}

void loop() {
  
  // reads the value from the microphone:
  micValue = analogRead(micPin);
  
  // sets a low threshold for the microphone to activate only the bottom row of LEDs
  if (sensorValue > 500){
  digitalWrite(ledPin, HIGH);
  }
  
  // a slighting higher threshold activates both the first row and the next row of LEDs
  if (sensorValue > 570){
   digitalWrite(ledPin, HIGH);
  digitalWrite(ledPin2, HIGH);
  }
  
  // activates the third row of LEDs at the next higher threshold
  if (sensorValue > 680){
   digitalWrite(ledPin, HIGH);
  digitalWrite(ledPin2, HIGH);
  digitalWrite(ledPin3, HIGH);  
  }
  
  // activates the fourth row of LEDs at the next higher threshold 
  if (sensorValue > 750){
   digitalWrite(ledPin, HIGH);
  digitalWrite(ledPin2, HIGH);
  digitalWrite(ledPin3, HIGH);  
  digitalWrite(ledPin4, HIGH);
  }
  
  // activates the fifth row of LEDs at the next higher threshold
  if (sensorValue > 880){
   digitalWrite(ledPin, HIGH);
  digitalWrite(ledPin2, HIGH);
  digitalWrite(ledPin3, HIGH);  
  digitalWrite(ledPin4, HIGH);
  digitalWrite(ledPin5, HIGH);
  }
  
  // activates the sixth row of LEDs at the next higher threshold
  if (sensorValue > 950){
    digitalWrite(ledPin, HIGH);
    digitalWrite(ledPin2, HIGH);
    digitalWrite(ledPin3, HIGH);  
    digitalWrite(ledPin4, HIGH);
    digitalWrite(ledPin5, HIGH);
    digitalWrite(ledPin6, HIGH);
  }
  
  // activates the seventh and final row of LEDs at the next higher threshold
  if (sensorValue > 1000){
  
    digitalWrite(ledPin, HIGH);
    digitalWrite(ledPin2, HIGH);
    digitalWrite(ledPin3, HIGH);  
    digitalWrite(ledPin4, HIGH);
    digitalWrite(ledPin5, HIGH);
    digitalWrite(ledPin6, HIGH);
    digitalWrite(ledPin7, HIGH);
  }
  
  // keeps the LEDs off for low values read into the microphone, keeps ambient noise from activating LEDs.
  else if (sensorValue < 500){
    digitalWrite(ledPin, LOW);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, LOW);
    digitalWrite(ledPin4, LOW);
    digitalWrite(ledPin5, LOW);
    digitalWrite(ledPin6, LOW);
    digitalWrite(ledPin7, LOW); 
  }


  // printing the microphone value to the serial monitor for testing
  Serial.println(micValue);  
}
