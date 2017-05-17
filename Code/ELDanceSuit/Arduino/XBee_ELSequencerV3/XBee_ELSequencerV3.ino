/**********************************************************************
XBee_ELSequencerV3.ino
Modified by Ho Yun Bobby Chan @ SparkFun Electronics May 12th, 2017

 * Taken from the SparkFun XBee EL Sequencer Demo Sketch
 * Ho Yun Bobby Chan @ SparkFun Electronics June 20, 2014
 * Updated by Toni Klopfenstein @ SparkFun Electronics April, 2015
 * https://github.com/sparkfun/EL_Sequencer
 *
 * Description:
 * This is a modified sketch for the EL Sequencer with a wireless controller.
 * Code used to read the slave XBee's received character and sequence each dancer's hoodie or pants.
 *
 * The wireless controller consists of a RedBoard Programmed with Arduino,
 * XBee Shield, XBee Series 1 transceiver, diffused RGB Common Cathode LED,
 * Blue Clear LED, 330Ohm current limiting resistors, 3x buttons, 
 * a 9V battery, and a 9V adapter.
 *
 * Each of the 7x EL dance suits use an EL Sequencer, 2x EL Wires, a
 * 12V EL inverter, XBee female sockets soldered, a 9V battery, 9V adapter,
 * and a XBee Series 1 transceiver.  Each EL Sequencer will use this same code.
 * An XBee Series 2 can be used but the throughput of the Series 1 is 
 * much higher. To reduce latency, I recommend using the XBee
 * Series 1. The basic configuration of the XBee module with point-to-point configuratin is
 * based on Digi's Example tutorial => https://www.digi.com/blog/xbee/basic-xbee-802-15-4-chat/.
 * Page 5 of the tutorial shows you how to broadcast with
 * point-to-multipoint configuration so that multiple EL Sequencers
 * can be controlled.
 *
 * By pushing the button, a character is sent from a remote microcontroller.
 * The corresponding EL Sequencer will receive the character and control
 * the EL component on a channel that is associated with that character.
 *
 * EL Sequencer uses the hardware UART of the Atmega328 for communication:
 * pin 0 = Rx
 * pin 1 = Tx
 *
 * Note: Make sure to remove the XBee Series 1 on the EL Sequencer when
 * uploading a new sketch file. Otherwise it will brick the XBee. You can
 * always use the next generation XCTU software to unbrick and recover
 * the transceiver.
 *
 * Development environment specifics:
 * Arduino 1.6.5
 *
 * This code is beerware; if you see me (or any other SparkFun employee) at the local,
 * and you've found our code helpful, please buy us a round!
 * Distributed as-is; no warranty is given.
 *
 ***********************************************************************/

char val; //Declare character 'val' when Slave XBee receives a character
char temp_delete; //used to delete buffer and prevent false triggers when Master XBee sends character more than once

//LED to check if the board is initialized.
const int status_LED = 13;

int counter = 0; //adding counter to prevent false triggers for a small period of time
boolean XBee_sent = false; //flag to see if we have received any characters after a certain period of time

/*******************Setup Loop***************************/
void setup() {
  Serial.begin(9600); //Begin Serial communication and debugging
  Serial.println("EL Sequencer's XBee is Ready to Receive Characters");

  val = 'A'; //save as default character

  //Initialize pins
  pinMode(status_LED, OUTPUT); //Set pin mode as output for status LED
  pinMode(2, OUTPUT); //Set pin mode as output for Channel A
  pinMode(3, OUTPUT); //Set pin mode as output for Channel B
  pinMode(4, OUTPUT); //Set pin mode as output for Channel C
  pinMode(5, OUTPUT); //Set pin mode as output for Channel D
  pinMode(6, OUTPUT); //Set pin mode as output for Channel E
  pinMode(7, OUTPUT); //Set pin mode as output for Channel F
  pinMode(8, OUTPUT); //Set pin mode as output for Channel G
  pinMode(9, OUTPUT); //Set pin mode as output for Channel H

  //Status LED to see if the EL Sequencer is initializing
  for (int i = 0; i < 3; ++i) {
    digitalWrite(status_LED, HIGH);//set Status LED on
    delay(50);
    digitalWrite(status_LED, LOW); //set Status LED off
    delay(50);
  }

  all_ON();//turn on all EL channels

  delay(100); //Wait a little
}

/*******************Main Loop***************************/
void loop() {
  if (XBee_sent == false) {
    //we have not received a character yet after a certain period of time
    //lets check to see if the Master XBee has sent any characters recently
    if (Serial.available()) {
      //check if slave XBee is receiving data from master XBee
      val = Serial.read();//save whatever is in the buffer to the variable
      counter = 0;        //set counter to 0 to prevent false button presses
      XBee_sent = true;   //we have received a character

      //if debugging, we can see what character is recevied
      Serial.print("Character Received = ");
      Serial.println(val);

      //Check to see if character sent is any of the recognized characters and jump to the sequence
      if (val == 'A') {
        Seq_0();
      }
      else if (val == 'B') {
        Seq_1();
      }
      else if (val == 'C') {
        Seq_2();
      }
      else if (val == 'D') {
        Seq_3();
      }
      else if (val == 'E') {
        Seq_4();
      }

    }//end buffer check
  }//end check to see if we have not received a character after a certain period of time

  if (counter >= 10) {//this section of code will reset the flag "XBee_Sent" so we can begin listening for characters again
    if (XBee_sent == true) {
      Serial.println("Counter = 10, we are ready to receive characters again");
    }
    XBee_sent = false;
  }

  if (XBee_sent == true) {//this section of code is used as a delay to prevent false button presses
    counter = ++counter;//keep adding until we reach 10, then we can reset flag and begin receiving again

    //if connected to a computer, check to see the duration of the delay
    Serial.print("Counter = ");
    Serial.println(counter);

    temp_delete = Serial.read();//try to clear false triggers in buffer provided by Master XBee until counter resets
  }

}//end loop()

//**********MODULAR SEQUENCED FUNCTIONS**********

void all_ON() {
  //this function is used to turn all channels ON
  //Bobby
  digitalWrite(2, HIGH); //Channel A, hoodie
  digitalWrite(3, HIGH); //Channel B, pants
  //Antuan, Kaden
  digitalWrite(4, HIGH); //Channel C, hoodie
  digitalWrite(5, HIGH); //Channel D, pants
  //Mireku, Talon
  digitalWrite(6, HIGH); //Channel E, hoodie
  digitalWrite(7, HIGH); //Channel F, pants
  //Madi, Henry
  digitalWrite(8, HIGH); //Channel G, hoodie
  digitalWrite(9, HIGH); //Channel H, pants
}


void all_OFF() {
  //this function is used to turn all channels OFF
  //Bobby
  digitalWrite(2, LOW); //Channel A, hoodie
  digitalWrite(3, LOW); //Channel B, pants
  //Antuan, Kaden
  digitalWrite(4, LOW); //Channel C, hoodie
  digitalWrite(5, LOW); //Channel D, pants
  //Mireku, Talon
  digitalWrite(6, LOW); //Channel E, hoodie
  digitalWrite(7, LOW); //Channel F, pants
  //Madi, Henry
  digitalWrite(8, LOW); //Channel G, hoodie
  digitalWrite(9, LOW); //Channel H, pants
}

void Seq_0() {
  //function used to flash Bobby's suit with the beat at 5 seconds into the edited C2C track

  digitalWrite(status_LED, LOW); //turn OFF Status LED
  digitalWrite(2, LOW); //Channel A
  digitalWrite(3, LOW); //Channel B
  delay(500);

  digitalWrite(status_LED, HIGH); //turn on Status LED

  digitalWrite(2, HIGH); //Channel A
  delay(500);
  digitalWrite(3, HIGH); //Channel B
  delay(500);
  digitalWrite(2, LOW); //Channel A
  digitalWrite(3, LOW); //Channel B
  delay(500);
  digitalWrite(2, HIGH); //Channel A
  digitalWrite(3, HIGH); //Channel B

  delay(500);
  digitalWrite(2, LOW); //Channel A
  digitalWrite(3, LOW); //Channel B
  delay(250);
  digitalWrite(2, HIGH); //Channel A
  digitalWrite(3, HIGH); //Channel B

  delay(500);

  digitalWrite(2, LOW); //Channel A
  digitalWrite(3, LOW); //Channel B
  delay(500);
  digitalWrite(2, HIGH); //Channel A
  digitalWrite(3, HIGH); //Channel B

  delay(250);
  digitalWrite(2, LOW); //Channel A
  digitalWrite(3, LOW); //Channel B
  delay(250);
  digitalWrite(2, HIGH); //Channel A
  digitalWrite(3, HIGH); //Channel B
  delay(250);
  digitalWrite(2, LOW); //Channel A
  digitalWrite(3, LOW); //Channel B
  delay(250);
  digitalWrite(2, HIGH); //Channel A
  digitalWrite(3, HIGH); //Channel B
  delay(250);
  digitalWrite(2, LOW); //Channel A
  digitalWrite(3, LOW); //Channel B
  delay(250);
  digitalWrite(2, HIGH); //Channel A
  digitalWrite(3, HIGH); //Channel B
  delay(250);
  digitalWrite(2, LOW); //Channel A
  digitalWrite(3, LOW); //Channel B
  delay(250);
  digitalWrite(2, HIGH); //Channel A
  digitalWrite(3, HIGH); //Channel B

  all_ON();//turn everything back on
}

void Seq_1() {
  //function used to toggle the channels back & forth and hit the beat at 45 seconds into the edited C2C track

  for (int i = 0; i < 8; ++i) {
    //Bobby
    digitalWrite(2, HIGH); //Channel A, hoodie
    digitalWrite(3, LOW); //Channel B, pants
    //Antuan, Kaden
    digitalWrite(4, HIGH); //Channel C, hoodie
    digitalWrite(5, LOW); //Channel D, pants
    //Mireku, Talon
    digitalWrite(6, LOW); //Channel E, hoodie
    digitalWrite(7, HIGH); //Channel F, pants
    //Madi, Henry
    digitalWrite(8, LOW); //Channel G, hoodie
    digitalWrite(9, HIGH); //Channel H, pants
    delay(250);

    //Bobby
    digitalWrite(2, LOW); //Channel A, hoodie
    digitalWrite(3, HIGH); //Channel B, pants
    //Antuan, Kaden
    digitalWrite(4, LOW); //Channel C, hoodie
    digitalWrite(5, HIGH); //Channel D, pants
    //Mireku, Talon
    digitalWrite(6, HIGH); //Channel E, hoodie
    digitalWrite(7, LOW); //Channel F, pants
    //Madi, Henry
    digitalWrite(8, HIGH); //Channel G, hoodie
    digitalWrite(9, LOW); //Channel H, pants
    delay(250);
  }

  all_ON();
  delay(750);
  all_OFF();
  delay(100);
  all_ON();

}

void Seq_2() {
  //this function turns all the channels ON just in case something happens

  all_ON();
}

void Seq_3() {
  //function used to as a ripple effect and turn on the channels to the beat scratch at 2 minutes 10 seconds into the edited C2C track

  all_OFF();
  delay(100);
  //Madi, Henry
  digitalWrite(8, HIGH); //Channel G, hoodie
  digitalWrite(9, LOW); //Channel H, pants
  delay(100);
  //Antuan, Kaden
  digitalWrite(4, HIGH); //Channel C, hoodie
  digitalWrite(5, LOW); //Channel D, pants
  delay(100);
  //Bobby
  digitalWrite(2, HIGH); //Channel A, hoodie
  digitalWrite(3, LOW); //Channel B, pants

  //Mireku, Talon
  digitalWrite(6, HIGH); //Channel E, hoodie
  digitalWrite(7, LOW); //Channel F, pants
  delay(100);

  //Madi, Henry
  digitalWrite(8, HIGH); //Channel G, hoodie
  digitalWrite(9, HIGH); //Channel H, pants
  delay(100);
  //Antuan, Kaden
  digitalWrite(4, HIGH); //Channel C, hoodie
  digitalWrite(5, HIGH); //Channel D, pants
  delay(100);
  //Bobby
  digitalWrite(2, HIGH); //Channel A, hoodie
  digitalWrite(3, HIGH); //Channel B, pants

  //Mireku, Talon
  digitalWrite(6, HIGH); //Channel E, hoodie
  digitalWrite(7, HIGH); //Channel F, pants
  delay(100);

}

void Seq_4() {
  //this function turns all the channels ON just in case something happens

  all_ON();
}
