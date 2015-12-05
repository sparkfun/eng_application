//----------------------------------------------------------
//- Keypad Lock
//- Firmware for MSP430G2553 (IN20, DIP)
//- By Don "AGlass0fMilk" Beckstein
//- 4/16/2012
//----------------------------------------------------------

/* Todo:
 * Interfacing with a lock mechanism
 * Interfacing with indicator LEDs, red/green
 * Allow and Deny mode implementation
 * Get PWM of servo working
 */

/*
 * ======== Standard MSP430 includes ========
 */
#include <msp430.h>

/*
 * ======== Grace related includes ========
 */
#include <ti/mcu/msp430/csl/CSL.h>

/*
 *  ======== User Code ========
 */
 										 	 //Moving 1
 #define OUT1 (BIT0 + !BIT1 + !BIT2 + !BIT3) //1000 R1
 #define OUT2 (!BIT0 + BIT1 + !BIT2 + !BIT3) //0100 R2
 #define OUT3 (!BIT0 + !BIT1 + BIT2 + !BIT3) //0010 R3
 #define OUT4 (!BIT0 + !BIT1 + !BIT2 + BIT3) //0001 R4
 
 #define C1IN BIT4
 #define C2IN BIT5
 #define C3IN BIT6
 
 #define INFO_B0 0x10BF
 
 #define STAR 10
 #define POUND 11
 
 /* Pin assignments:
  * P1.0 - Keypad Row 1
  * P1.1 - Keypad Row 2
  * P1.2 - Keypad Row 3
  * P1.3 - Keypad Row 4
  * P1.4 - Keypad Column 1
  * P1.5 - Keypad Column 2
  * P1.6 - Keypad Column 3
  * P1.7 - Reed Switch Input (Detect if door is opened or closed)
  * P2.0 - Red Deny Indicator LED
  * P2.1 - Door Lock Mechanism (servo output, TA1.1)
  * P2.2 - Green Allow Indicator LED
  * P2.3 - Speaker Beep Output (optional - broken on test MCU)
  * ----
  * For the future: May implement a small serial-enabled LCD display, would require P1.2 (TXD) for communication
  */
  
  /*#define LOCK_P BIT7
  #define DENY BIT0
  #define ALLW BIT1
  #define BEEP BIT2*/
 
 //The Out codes scan the row pins, making each row high at a certain time. If a button is pressed, the row is high, column goes high
 //The column and row give us the number pressed
 
 int C1[] = {-1, 1, 4, 7, 10};
 int C2[] = {-1, 2, 5, 8, 0};
 int C3[] = {-1, 3, 6, 9, 11};
 
 int keyPressed=0; //=1, keyPressed already, =0, obvious
 
 unsigned int* passcode_ptr = (unsigned int*) 0x1000;
 
 int inWrite = 0; //inWrite is an iterator for writing to inPass, 
 //is incremented everytime input is received
 int inPass[4] = {0, 0, 0, 0};
 int rstCode[4] = {10, 10, 10, 10}; //**** to reset
 int notSet[4] = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
 
 int flashes = 0; //# of flashes iterator, see flashLight()
 int maxFlash = 0; //# of flashes to count to
 int light_p = 0; //Light to flash (Pin bit)
 
 int mode = 0; //0: Normal Mode, 1: Reset Mode 1, 2:  Reset Mode 2
 /*Description of above modes: -Normal mode, normal operating mode while waiting for input
							   -Reset Mode 1, Preliminary reset mode where MCU accepts previous password as input
							   -Reset Mode 2, Reset mode after previous password correctly gived, allows password to be reset
*/
 
 //Scan each
 
 int iter = 0; //Iterator
 
 void scanKeyPad(void)
 {
 	iter++;
 	if(iter > 4)
 		iter = 1; //Reset iterator
 	switch(iter)
 	{
 		case 1:
 			P1OUT &= ~OUT4;
 			P1OUT |= OUT1; //sets output to 1000
 			break;
 			
 		case 2:
 			P1OUT &= ~OUT1;
 			P1OUT |= OUT2; //sets output to 0100
 			break;
 			
 		case 3:
 			P1OUT &= ~OUT2;
 			P1OUT |= OUT3; //sets output to 0010
 			break;
 			
 		case 4:
 			P1OUT &= ~OUT3;
 			P1OUT |= OUT4; //sets output to 0001
 			break;
 	}
 }
 
int readKeyPad(void)
 {
 	if(P1IN & C1IN && P1IFG & C1IN) //Column 1
 	{
 		TA0CTL &= ~MC_1; //shut off clock (stops scanning, powered row stays the same and it saves on power consumption)
 		P1IE = C1IN; //Disable interrupts for all other columns
 		P1IES = C1IN; //Interrupt on the falling edge (when the button is released)
 		keyPressed = 1;
 		return C1[iter];
 	}
 	else if(P1IN & C2IN && P1IFG & C2IN) //Column 2
 	{
 		TA0CTL &= ~MC_1; //shut off clock (stops scanning, powered row stays the same and it saves on power consumption)
 		P1IE = C2IN; //Disable interrupts for all other columns
 		P1IES = C2IN; //Interrupt on the falling edge (when the button is released)
 		keyPressed = 1;
 		return C2[iter];
 	}
 	else if(P1IN & C3IN && P1IFG & C3IN) //Column 3
 	{
 		TA0CTL &= ~MC_1; //shut off clock (stops scanning, powered row stays the same and it saves on power consumption)
 		P1IE = C3IN; //Disable interrupts for all other columns
 		P1IES = C3IN; //Interrupt on the falling edge (when the button is released)
 		keyPressed = 1;
 		return C3[iter];
 	}
 	return -1;
 }
 
 //Flash implementation of above function
 void setPasscode(int nPass[])
 {
 	unsigned int i;
 	

  //Flash_ptr = (char *) 0x1080;              // Initialize Flash pointer
  FCTL1 = FWKEY + ERASE;                    // Set Erase bit
  FCTL3 = FWKEY;                            // Clear Lock bit
  //*Flash_ptr = 0;                           // Dummy write to erase Flash segment
  	*passcode_ptr = 0; //dummy write

  FCTL1 = FWKEY + WRT;                      // Set WRT bit for write operation

  for (i=0; i<=3; i++)
  {
    *passcode_ptr = nPass[i];                   // Write value to flash
    passcode_ptr++;
  }

  FCTL1 = FWKEY;                            // Clear WRT bit
  FCTL3 = FWKEY + LOCK;                     // Set LOCK bit
  
  //passcode_ptr = (unsigned int*) INFO_B0;
  passcode_ptr = (unsigned int*) 0x1000;
  
  //__enable_interrupt();
  
 }
 
 int compareCodes(int code1[], int code2[])
 {
 	int i;
 	for(i=0; i<=3; i++)
 	{
 		if(code1[i] != code2[i])
 			return 0; //if any of the numbers aren't equivalent, return 0
 	}
 	return 1; //Else, they are the same, return 1
 }
 
 int compareCodes_pt(int code1[], unsigned int *code2)
 {
 	int i;
 	for(i=0; i<=3; i++)
 	{
 		if(code1[i] != *code2)
 			return 0;
 		code2++;
 	}
 	return 1;
 }
 
 //Flashes light on pin indicated by light variable, at a certain speed for x # of times
 void flashLight(int light, int speed, int times)
 {	
 	TA1CCR0 = speed;
 	TA1R = 0; //Reset counter
 	
 	maxFlash = times;
 	light_p = light;
 	
 	P2OUT |= light;
 	
 	TA1CTL |= MC_1; //Turn on timer1 in Up mode
 }
 
 //Stops flashing sequence
 void stopLight(void)
 {
 	P2OUT &= ~(light_p); //Turn light off
 	TA1CTL &= ~MC_1; //Turn off Timer_A1
 	flashes = 0; //Reset flashes counter
 }
 
 void toggleLock(int toggle) //Toggle = 1, unlock, toggle = 0, lock
 {
 	if(toggle == 0) //Lock door
 	{
 		TA1CTL &= ~TAIE;
 		TA1CCR0 = 10000 - 1;
 		TA1CCR1 = 500;
 		TA1CCTL1 = OUTMOD_7;
 		TA1CTL |= MC_1;
 	}
 	else if(toggle == 1) //Unlock door
 	{
		//Turn timer A1 on, up mode, set TA1CCR0 to unlock PWM, turn off output after certain period of time, close once door closed detected
		TA1CTL &= ~TAIE; //Turn off TA1 Interrupt (not flashing lights)
		TA1CCR0 = 10000-1;
		TA1CCR1 = 2000;
		TA1CCTL1 = OUTMOD_7;
		TA1CTL |= MC_1; //Up mode
 	}
 }
 
 void denyAccess(void)
 {
 	
 }
 
 void allowAccess(void)
 {
 	
 }
 
int main(int argc, char *argv[])
{
    CSL_init();                     // Activate Grace-generated configuration
    
    __enable_interrupt();           // Set global interrupt enable
    _BIS_SR(LPM0 + GIE);
    
    return (0);
}

//P1 Interrupt Handler, deciphers key input
void P1IH(void)
{
	if(!(P1IES & BIT4) && !(P1IES & BIT5) && !(P1IES & BIT6)) //Rising edge
	{
		if(!keyPressed) //no Key pressed, just for safety
		{
			//Flash green light briefly to indicate button press, beep buzzer (@todo)
			flashLight(BIT0, 2401, 1); //On/off in 200ms, edit later to work with Green LED @todo
			int input = readKeyPad();
			if(input != POUND) //Pound = clear code
			{
				inPass[inWrite] = input;
				inWrite++;
				if(inWrite > 3) //4 keys pressed
				{
					inWrite = 0; //reset counter
					switch(mode)
					{
						case 0: //Normal Mode
							if(compareCodes_pt(notSet, passcode_ptr))
							{
								setPasscode(inPass);
							}
						
							if(compareCodes(inPass, rstCode)) //inPass = rstCode, enter reset mode
							{
								mode = 1; //Enter reset mode 1
								break;
							}
							if(compareCodes_pt(inPass, passcode_ptr)) //inPass = passcode, unlock door
							{
								TA0CTL = MC_0;
								P1OUT = BIT7;
								break;
							}
							break;
							
						case 1: //Reset Mode 1, require passcode before reset, blink green LED slowly
							if(compareCodes_pt(inPass, passcode_ptr))
								mode = 2; //Proceed to reset mode 2, flash green LED fast
							else
							{
								//flash red LED
								mode = 0; //exit reset mode, should enter deny mode (@todo)
							}
							break;
						case 2: //Reset Mode 2, accept passcode input
							if(!(compareCodes(inPass, rstCode))) //If the new password isn't the reset code (can't have that)
							{
								setPasscode(inPass); //Set new passcode, flash green LED fast
								mode = 0; //reset mode to normal, forgot to do this before, doh!
							}
							//else
								//enter deny mode
							break;
						default:
							break;
							
					}
				}
			}
			else //Pound pressed, clear input code/reset counter
			{
				int i;
				for(i=0;i<=3;i++)
					inPass[i] = 0;
				inWrite = 0;
			}
			
		}
	}
	else //Falling edge
	{
		//Reset system to standby state, restart clock, reenable interrupts for all columns, reset edge setting
		TA0CTL |= MC_1; //Start clock/scanning
		P1IES = 0; //All interrupts on the rising edge
		P1IE |= C1IN + C2IN + C3IN; //Enable interrupts for all column inputs
		keyPressed = 0;
	}
	P1IFG = 0;
}

//Timer Interrupt Handler (runs every 10ms to save energy)
void Timer_A0(void)
{
	scanKeyPad();
}

//Timer_1 Interrupt Handler (times the LED flashes)
void Timer_A1(void)
{
	if(maxFlash != -1) //Not indefinite
	{	
		if(flashes < maxFlash)
		{
			P2OUT ^= light_p; //toggle light
			flashes++;
		}
		else //Done flashing
		{
			stopLight();
		}
	}
	else //Indefinite flashing
	{
		P2OUT ^= light_p;
	}
}
