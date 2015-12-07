
/*Emily Daub 09/30/15
 * 
 * PhotoSmart Running Top Sketch
 * 
 * Project 1 - Physical Computing, Fall 2015           
 * Project 1 - Independant Study, Fall 2015            
 *
 * Two photoresistors controling independ strings of LEDs 
 * See fitzing sketch for full set up 
 * 
 * Ribbon cable colors used in project are denoted when intergers are declared. (color RC) 
 * 
 * Resources: 
 *    Sparkfun Inventor's Kit - Code Library - Circuit 6 - photoresistor 
 *     Arduino Button Tutorial
 *     https://www.arduino.cc/en/Tutorial/StateChangeDetection
 * 
 * Materials used; 
 * 
 * Breadboard set up            Wearable inegration 
 * 2 While LEDs                 12 White LEDs
 * 2 Red LED                    9 Red LEDs 
 * 4 330 ohm resistors 
 * 1 100 ohm resistor 
 * 2 photoresistors             2 Lilypad photosensors 
 * Redboard                     Lilypad
 * 
 * 
 */
  
const int sensorPin1  = 0;      // blue RC             // Controls White LEDs (front of top)   
const int sensorPin2  = 1;      // green RC             // Controls Red LEDs (back of top) 

const int ledWhite1   = 9;      // White                // Two strings of white LEDs because pins couldn't support that much power 
const int ledWhite2   = 10;     // Grey RC              // MUST use PWM pins for LEDs
const int ledRed      = 11;     // Red RC

int lightLevel, high = 0, low = 1023;                 // indicating the extremes of the light levels used 

 
void setup()
{
 
  pinMode(ledWhite1, OUTPUT);   // Declare LED pins outputs
  pinMode(ledWhite2, OUTPUT);
  pinMode(ledRed, OUTPUT);
   
  Serial.begin(9600);           // Initalizes serial monitor 
                                // Make sure number matchs in SM
}
 
void loop()
{
   {
    lightLevel = analogRead(sensorPin1);  
    Serial.print("Sensor 1: ")               // Makes monitor easier to read
    Serial.print(lightLevel);                // Print the light level 
   
     lightTune();
     {
      analogWrite(ledWhite1, 0);
      analogWrite(ledWhite1, lightLevel);
      analogWrite(ledWhite2, 0);
      analogWrite(ledWhite2, lightLevel);
      
      delay(lightLevel);
     }
      delay(50);
   }
   {
    lightLevel = analogRead(sensorPin2);    // Calling maual tune function for sensor 1 behavior 
    Serial.print("     Back Sensor: ")      // Makes SM easier to read
    Serial.println(lightLevel); 

    lightTune();
    {
      analogWrite(ledRed, 0);
      analogWrite(ledRed, lightLevel);
      
      delay(lightLevel);
    }
    delay(50); 
    }
}

void lightTune()      // Use of lightTune function allows us to not hav multiple lightLevel Variables 
{
  lightLevel = map(lightLevel, 900, 1023, 0, 255);  // map the small range given by the light level (0
  lightLevel = constrain(lightLevel, 0, 255);  
}

  
 
