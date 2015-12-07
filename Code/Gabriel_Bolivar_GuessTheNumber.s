/* May 08, 2014*/
/*Gabriel Bolivar*/
/*Altera DE1 Board*/
/*For instruction set reference check: https://www.altera.com/content/dam/altera-www/global/en_US/pdfs/literature/hb/nios2/n2cpu_nii51017.pdf*/
/*This program simulates a game where the user needs to guess a random generated number. The user has the option to use the switches on the Altera DE1 Board
or the computer keyboard to interact with the system. Depending on the user's decision the system assigns some parameters to make the game more interesting:
if the user chooses the switches, he/she will have 3 attempts (credits) and 10 seconds to guess the number. On the other hand, if the users chooses to play 
with the keyboard, the number of attempts are set to 3 and the time to 5 seconds. The system displays the number entered by the user, if it is greater than, 
less than, or equal to the generated number, number of remaining attempts and time. It is also possible to reset the system at any time with a pushButton*/

/*Qsys Addresses*/			  
                            
.equ TIMER,   0x00009000
.equ jtag,    0x000090d0		
.equ hex0,    0x00009050
.equ hex1,    0x00009040
.equ hex2,    0x00009030
.equ hex3,    0x00009020
.equ ledr,    0x00009070
.equ ledg,    0x00009060
.equ sw,      0x000090c0
.equ pButton0, 0x000090b0
.equ pButton1, 0x000090a0
.equ pButton2, 0x00009090
.equ pButton3, 0x00009080


/*Text Block*/
.text

/*Start of Main Program*/
.global _start
_start:

/* Assign the addresses to registers *********************************************************************/  

movia	r2, TIMER		/*Register assignment for TIMER*/
movia	r3, jtag		/*Register assignment for jtag*/
movia	r4, hex0		/*Register assignment for hex0*/
movia	r5, hex1		/*Register assignment for hex1*/
movia	r6, hex2		/*Register assignment for hex2*/
movia	r7, hex3		/*Register assignment for hex3*/
movia	r8, ledr		/*Register assignment for ledr*/
movia	r9, ledg		/*Register assignment for ledg*/
movia	r10, sw		  /*Register assignment for sw*/
movia	r11, pButton0	/*Register assignment for pButton0*/
movia r12, pButton1	/*Register assignment for pButton1*/
movia r13, pButton2	/*Register assignment for pButton2*/
movia r14, pButton3	/*Register assignment for pButton3*/


main:

/*Initial message*****************************************************************************************/
movia sp, 0x07FFFFC /* stack starts from highest memory address in SDRAM */
movia r15, TEXT_STRING

string:
ldb r16, 0(r15) /*Loads TEXT_STRING*/
beq r16, zero, GET_JTAG /* string is null-terminated */
call PUT_JTAG
addi r15, r15, 1 
br string

/* read and echo characters */
GET_JTAG:
ldwio r17, 0(r3) /* read the JTAG UART data register */
andi r15, r17, 0x8000 /* check if there is new data */
beq r15, r0, GET_JTAG /* if no data, wait */
andi r16, r17, 0x00ff /* the data is the least significant byte */

call PUT_JTAG /* echo character */
br GET_JTAG


/***
* Subroutine to send a character to the JTAG UART
* r16 = character to send
* r3 = JTAG UART base address
***/

.global PUT_JTAG
PUT_JTAG:
/* save any modified registers */
subi sp, sp, 4 /* reserve space on the stack */
stw r17, 0(sp) /* save register */

ldwio r17, 4(r3) /* read the JTAG UART Control register */
andhi r17, r17, 0xffff /* check for write space */
beq r17, r0, END_PUT /* if no space, ignore the character */
stwio r16, 0(r3) /* send the character */

END_PUT:
/* restore registers */
ldw r17, 0(sp)
addi sp, sp, 4

movia r19, 0x32 /*Number 2 ascii*/
beq r16, r19, loop /*Check if number is equal to 2 = user interacts with switches*/
movia r19, 0x31 /*Number 1 ascii*/
beq r16, r19, loopk /*Check if number is equal to 1 = user interacts with keyboard*/
ret 

.data /* data follows */
TEXT_STRING:
.asciz "\nEnter ONE to use the keyboard or TWO to use the switches "
/*.end*/

loop:
/*Display initial configuration for game with switches************************************************************************/
movia r16, 0x07  /* start with 3 credits */
stwio r16, 0(r9) /*display attempts in ledg */
movia r16, 0x3ff  /* start with 10 seconds */
stwio r16, 0(r8) /*display seconds in ledr */

/*Set credits: decimal number */
movia r1, 0x03 /*3 credits*/

/*Seven Segments*/
movia r17, 0x40 /*number 0 for initial 10 seconds*/
movia r18, 0x79 /*number 1 for initial 10 seconds*/
stwio r17, 0(r4) /*display 0 to HEX0*/
stwio r18, 0(r5) /*display 1 to HEX1*/
movia r19, 0x7f /* blank value in HEX*/
stwio r19, 0(r6) /*display BLANK HEX2*/
stwio r19, 0(r7) /*display BLANK HEX3*/

waitIN: /*Main function*********************************************************************************/ 

/*generate number **************************************************************************************/
movia r15, 0xf00  /*to obtain one digit value by ANDing*/
movia r16, 0b10  /*check if time has finished */
movia r17, 0b100 /*start timer */
movia r18, 0x01  /*value of 0x01 stops timer (this value is used as the random number generated)*/
movia r19, 0x09  /*to compare if the number is between 0 and 9 */
movia r23, 0x08  /*places to shift the number*/

number:
stwio r17, 4(r2) /*starts timer */
stwio r18, 16(r2) /*stop timer */
ldwio r20, 16(r2) /*read timer */
and r21, r15, r20 /*obtain one digic value */
srl r21, r21, r23 
bgtu r21, r19, number /* if number obtained is greater than 9 obtain other number*/
mov r22, r21 /*move numbers to register 21 */

ldwio r20, 3(r11) /* read the pButton0 to check for reset */
beq r20, r0, reset /*if hit go the reset funtion */

ldwio r20, 3(r14) /* read the pButton3 to go initiate again */
beq r20, r0, loop /*if hit go the loop funtion */

ldwio r20, 3(r13) /* read the pButton2 to start */
beq r20, r0, start /*if hit go the start funtion */

movia r15, 0x200 /*Re-start from initial message to choose from keyboard or switches*/
ldwio r16, 0(r10)
beq r16, r15, main

br waitIN

waitIN2:

ldwio r20, 3(r14) /* read the pButton3 to go initiate again */
beq r20, r0, loop /*if hit go the loop funtion */

br waitIN2



/*Reset function******************************************************************************************/
reset:
movia r19, 0x7f /* blank value in HEX*/
stwio r0, 0(r8) /*display 0 attempts in ledr */
stwio r0, 0(r9) /*display 0 credits in ledg */
stwio r19, 0(r4) /*display blank in HEXO */
stwio r19, 0(r5) /*display blank in HEX1 */
stwio r19, 0(r6) /*display blank in HEX2 */
stwio r19, 0(r7) /*display blank in HEX3 */
br waitIN2


/*addCredit function******************************************************************************************/
addCredit:
br waitIN


/*start function**********************************************************************************************/
start:
movia r19, 0x7f /* blank value in HEX*/
stwio r19, 0(r6) /*clear HEX2*/
stwio r19, 0(r7) /*clear HEX3*/

/*movi r22, 0x03*/
/*Timer*/
movia r21, 0b10  /*check if time has finished */
movia r23, 0b100 /*number to start timer */
movia r17, 0x40 /*number 0 for initial 10 seconds*/
movia r18, 0x79 /*number 1 for initial 10 seconds*/ 
stwio r18, 0(r5) /*1 in HEX1 -- counting 10*/
stwio r17, 0(r4) /*0 in HEX0 -- counting 10*/
movia r15, 0x3ff /*Turn on 10 LEDs*/
stwio r15, 0(r8) /*Turn on red LEDs*/


stwio r0, 0(r2) /*Reset timer*************/
stwio r23, 4(r2) /*Start timer*/


/************************************The next loop reads the input from the switches, displays the remaining time for the user to 
guess the number, displays the number inputted by the user, compares it, and displays if it's less than, greater than or equal to the number to be guessed*/

countA:
movia r20, 0x01 
ldwio r18, 3(r12)  /*Read pushbutton1*/
beq r18, r20, next /*Check if pButton1 has been hit, if not go to next*/
ldwio r17, 0(r10)  /*Read switches*/

movia r18, 0x00   /*Check input from switches*/
beq r18, r17, cero
movia r18, 0x01
beq r18, r17, uno
movia r18, 0x02
beq r18, r17, dos
movia r18, 0x03
beq r18, r17, tres
movia r18, 0x04
beq r18, r17, cuatro
movia r18, 0x05
beq r18, r17, cinco
movia r18, 0x06
beq r18, r17, seis
movia r18, 0x07
beq r18, r17, siete
movia r18, 0x08
beq r18, r17, ocho
movia r18, 0x09
beq r18, r17, nueve

cero:           /*Display the value for switches in HEX3*/
movia r18, 0x40
stwio r18, 0(r7)
br continue
uno:
movia r18, 0x79
stwio r18, 0(r7)
br continue
dos:
movia r18, 0x24
stwio r18, 0(r7)
br continue
tres:
movia r18, 0x30
stwio r18, 0(r7)
br continue
cuatro:
movia r18, 0x19
stwio r18, 0(r7)
br continue
cinco:
movia r18, 0x12
stwio r18, 0(r7)
br continue
seis:
movia r18, 0x02
stwio r18, 0(r7)
br continue
siete:
movia r18, 0x78
stwio r18, 0(r7)
br continue
ocho:
movia r18, 0x00
stwio r18, 0(r7)
br continue
nueve:
movia r18, 0x18
stwio r18, 0(r7)
br continue

continue:
beq r22, r17, equal   /*Compare number*/
bgtu r17, r22, greater
bltu r17, r22, less 
greater:
movia r15, 0x02 /*display G = Greater*/
stwio r15, 0(r6) /*display in HEX2*/
br next
less:
movia r15, 0x47 /*display L = Less*/
stwio r15, 0(r6) /*display in HEX2*/
br next
equal:
movia r15, 0x06 /*display E = Equal*/
stwio r15, 0(r6) /*display in HEX2*/
br waitIN
next:
ldwio r20, 3(r14) /* read the pButton3 to go to initiate again */
beq r20, r0, loop /*if hit go the loop funtion */
ldwio r16, 0(r2) /*Store value of timer*/
beq r16, r21, countA /*one second loop*/


beq r24, 0x09, nine
beq r24, 0x08, eight
beq r24, 0x07, seven
beq r24, 0x06, six
beq r24, 0x05, five
beq r24, 0x04, four
beq r24, 0x03, three
beq r24, 0x02, two
beq r24, 0x01, one
beq r24, 0x00, zero

nine:
stwio r0, 0(r2) /*Reset timer***********/
stwio r19, 0(r5) /*display blank in HEX1 */
movia r15, 0x18 /*number 9*/
stwio r15, 0(r4) /*Display number*/
movia r15, 0x1ff /*Turn on 9 LEDs*/
stwio r15, 0(r8) /*Turn on red LEDs*/
stwio r23, 4(r2) /*Start timer*/
br countA

eight:
stwio r0, 0(r2) /*Reset timer**********/
movia r15, 0x00 /*number 8*/
stwio r15, 0(r4) /*Display number*/
movia r15, 0xff /*Turn on 8 LEDs*/
stwio r15, 0(r8) /*Turn on red LEDs*/
stwio r23, 4(r2) /*Start timer*/
br countA

seven:
stwio r0, 0(r2) /*Reset timer**********/
movia r15, 0x78 /*number 7*/
stwio r15, 0(r4) /*Display number*/
movia r15, 0x7f /*Turn on 7 LEDs*/
stwio r15, 0(r8) /*Turn on red LEDs*/
stwio r23, 4(r2) /*Start timer*/
br countA

six:
stwio r0, 0(r2) /*Reset timer*********/
movia r15, 0x02 /*number 6*/
stwio r15, 0(r4) /*Display number*/
movia r15, 0x3f /*Turn on 6 LEDs*/
stwio r15, 0(r8) /*Turn on red LEDs*/
stwio r23, 4(r2) /*Start timer*/
br countA

five:
stwio r0, 0(r2) /*Reset timer**********/
movia r15, 0x12 /*number 5*/
stwio r15, 0(r4) /*Display number*/
movia r15, 0x1f /*Turn on 5 LEDs*/
stwio r15, 0(r8) /*Turn on red LEDs*/
stwio r23, 4(r2) /*Start timer*/
br countA

four:
stwio r0, 0(r2) /*Reset timer*********/
movia r15, 0x19 /*number 4*/
stwio r15, 0(r4) /*Display number*/
movia r15, 0x0f /*Turn on 4 LEDs*/
stwio r15, 0(r8) /*Turn on red LEDs*/
stwio r23, 4(r2) /*Start timer*/
br countA

three:
stwio r0, 0(r2) /*Reset timer*******/
movia r15, 0x30 /*number 3*/
stwio r15, 0(r4) /*Display number*/
movia r15, 0x07 /*Turn on 3 LEDs*/
stwio r15, 0(r8) /*Turn on red LEDs*/
stwio r23, 4(r2) /*Start timer*/
br countA

two:
stwio r0, 0(r2) /*Reset timer**********/
movia r15, 0x24 /*number 2*/
stwio r15, 0(r4) /*Display number*/
movia r15, 0x03 /*Turn on 2 LEDs*/
stwio r15, 0(r8) /*Turn on red LEDs*/
stwio r23, 4(r2) /*Start timer*/
br countA

one:
stwio r0, 0(r2) /*Reset timer**********/
movia r15, 0x79 /*number 1*/
stwio r15, 0(r4) /*Display number*/
movia r15, 0x01 /*Turn on 1 LEDs*/
stwio r15, 0(r8) /*Turn on red LEDs*/
stwio r23, 4(r2) /*Start timer*/
br countA

zero:
stwio r0, 0(r2) /*Reset timer*********/
movia r15, 0x40 /*number 0*/
stwio r15, 0(r4) /*Display number*/
movia r15, 0x00 /*Turn on 0 LEDs*/
stwio r15, 0(r8) /*Turn on red LEDs*/
ldwio r20, 3(r14) /* read the pButton3 to go initiate again */
beq r20, r0, loop /*if hit go the loop funtion */

movia r15, 0x01 /*Substract Credit*/
sub r1, r1, r15

credits:
movia r15, 0x00
beq r15, r1, total

movia r15, 0x01
beq r15, r1, total1

movia r15, 0x02
beq r15, r1, total2

movia r15, 0x03
beq r15, r1, total3

movia r15, 0x04
beq r15, r1, total4

movia r15, 0x05
beq r15, r1, total5

movia r15, 0x06
beq r15, r1, total6

movia r15, 0x07
beq r15, r1, total7

movia r15, 0x08
beq r15, r1, total8

total:
movia r20, 0x00
stwio r20, 0(r9)
br reset

total1:
movia r20, 0x01
stwio r20, 0(r9)
br waitIN

total2:
movia r20, 0x03
stwio r20, 0(r9)
br waitIN

total3:
movia r20, 0x07
stwio r20, 0(r9)
br waitIN

total4:
movia r20, 0x0f
stwio r20, 0(r9)
br waitIN

total5:
movia r20, 0x1f
stwio r20, 0(r9)
br waitIN

total6:
movia r20, 0x3f
stwio r20, 0(r9)
br waitIN

total7:
movia r20, 0x7f
stwio r20, 0(r9)
br waitIN

total8:
movia r20, 0xff
stwio r20, 0(r9)
br waitIN




/*******************Configuration for Game with keyboard*******************************************************************************/
loopk:
/*Display initial configuration for game with keyboard************************************************************************/
movia r16, 0x07  /* start with 3 credits */
stwio r16, 0(r9) /*display attempts in ledr */
movia r16, 0x1f  /* start with 5 seconds */
stwio r16, 0(r8) /*display seconds in ledg */

/*Set credits: decimal number */
movia r1, 0x03 /*3 credits*/

/*Seven Segments*/
movia r17, 0x12 /*number 5 for initial 5 seconds*/
stwio r17, 0(r4) /*display 5 to HEX0*/
movia r19, 0x7f /* blank value in HEX*/
stwio r19, 0(r6) /*display BLANK HEX2*/
stwio r19, 0(r7) /*display BLANK HEX3*/
stwio r19, 0(r5) /*display BLANK HEX1*/

waitINk: /*Main function*********************************************************************************/ 

/*generate number **************************************************************************************/
movia r15, 0xf00  /*to obtain one digit value by ANDing*/
movia r16, 0b10  /*check if time has finished */
movia r17, 0b100 /*start timer */
movia r18, 0x01  /*value of 1 to stop de snap of timer */
movia r19, 0x09  /*to compare if the number is between 0 and 9 */
movia r23, 0x08  /*places to shift the number*/

numberk:
stwio r17, 4(r2) /*starts timer */
stwio r18, 16(r2) /*stop snap */
ldwio r20, 16(r2) /*read timer */
and r21, r15, r20 /*obtain one digic value */
srl r21, r21, r23 
bgtu r21, r19, numberk /* if number obtained is greater than 9 obtain other number*/
mov r22, r21 /*move numbers to register 21 */

inputk:
movia r15, 0x200 /*Re-start from initial message to chosse from keyboard or switches*/
ldwio r16, 0(r10)
beq r16, r15, main

ldwio r18, 0(r3) /* read the JTAG UART data register */
andi r15, r18, 0x8000 /* check if there is new data */
beq r15, r0, inputk /* if no data, wait */
movia r15, 0xff
and r18, r18, r15


movia r16, 0x72 /*ascii for r = reset*/
movia r17, 0x73 /*ascii for s = start*/
movia r19, 0x6f /*ascii for O = off*/

beq r18, r19, loopk /*if o go the loop funtion */
beq r18, r16, resetk /*if r go the reset funtion */
beq r18, r17, startk /*if s go the start funtion */
br waitINk

waitIN2k:
ldwio r18, 0(r3) /* read the JTAG UART data register */
andi r15, r18, 0x8000 /* check if there is new data */
beq r15, r0, waitIN2k /* if no data, wait */
movia r15, 0xff
and r18, r18, r15
movia r16, 0x72 /*ascii for r = reset*/
beq r18, r16, loopk /*if r go the loop funtion */


/*Reset function******************************************************************************************/
resetk:
movia r19, 0x7f /* blank value in HEX*/
stwio r0, 0(r9) /*display 0 credits in ledg */
stwio r0, 0(r8) /*display 0 credits in ledg */
stwio r19, 0(r4) /*display blank in HEXO */
stwio r19, 0(r5) /*display blank in HEX1 */
stwio r19, 0(r6) /*display blank in HEX2 */
stwio r19, 0(r7) /*display blank in HEX3 */
br waitIN2k


/*start function**********************************************************************************************/
startk:
movia r19, 0x7f /* blank value in HEX*/
stwio r19, 0(r6) /*clear HEX2*/
stwio r19, 0(r7) /*clear HEX3*/

/*Timer*/
movia r21, 0b10  /*check if time has finished */
movia r23, 0b100 /*number to start timer */
movia r17, 0x12 /*number 5 for initial 5 seconds*/ 
stwio r17, 0(r4) /*5 in HEX0 -- counting 5*/
movia r15, 0x1f /*Turn on 5 LEDs*/
stwio r15, 0(r8) /*Turn on red LEDs*/

stwio r0, 0(r2) /*Reset timer**********/
movia r15, 0x12 /*number 5*/
stwio r15, 0(r4) /*Display number*/
movia r15, 0x1f /*Turn on 5 LEDs*/
stwio r15, 0(r8) /*Turn on red LEDs*/
stwio r23, 4(r2) /*Start timer*/

countAk:
movia r20, 0x01
ldwio r19, 0(r3) /* read the JTAG UART data register */
andi r15, r19, 0x8000 /* check if there is new data */
beq r15, r0, next5k /* if no data, move */
movia r15, 0xff
and r19, r19, r15 /*ascii value*/

movia r18, 0x30   /*Check input from keyboard*/
beq r18, r19, zerok
movia r18, 0x31
beq r18, r19, onek
movia r18, 0x32
beq r18, r19, twok
movia r18, 0x33
beq r18, r19, threek
movia r18, 0x34
beq r18, r19, fourk
movia r18, 0x35
beq r18, r19, fivek
movia r18, 0x36
beq r18, r19, sixk
movia r18, 0x37
beq r18, r19, sevenk
movia r18, 0x38
beq r18, r19, eightk
movia r18, 0x39
beq r18, r19, ninek
movia r16, 0x72 /*ascii for r = reset*/
beq r19, r16, loopk /*if r go the loop funtion */


zerok:           /*Display the value for keyboard in HEX3*/
movia r18, 0x40
stwio r18, 0(r7)
movia r17, 0x00
br continuek
onek:
movia r18, 0x79
stwio r18, 0(r7)
movia r17, 0x01
br continuek
twok:
movia r18, 0x24
stwio r18, 0(r7)
movia r17, 0x02
br continuek
threek:
movia r18, 0x30
stwio r18, 0(r7)
movia r17, 0x03
br continuek
fourk:
movia r18, 0x19
stwio r18, 0(r7)
movia r17, 0x04
br continuek
fivek:
movia r18, 0x12
stwio r18, 0(r7)
movia r17, 0x05
br continuek
sixk:
movia r18, 0x02
stwio r18, 0(r7)
movia r17, 0x06
br continuek
sevenk:
movia r18, 0x78
stwio r18, 0(r7)
movia r17, 0x07
br continuek
eightk:
movia r18, 0x00
stwio r18, 0(r7)
movia r17, 0x08
br continuek
ninek:
movia r18, 0x18
stwio r18, 0(r7)
movia r17, 0x09
br continuek

continuek:
beq r22, r17, equalk   /*Compare number*/
bgtu r17, r22, greaterk
bltu r17, r22, lessk 
greaterk:
movia r15, 0x02 /*display G = Greater*/
stwio r15, 0(r6) /*display in HEX2*/
br nextk
lessk:
movia r15, 0x47 /*display L = Less*/
stwio r15, 0(r6) /*display in HEX2*/
br nextk
equalk:
movia r15, 0x06 /*display E = Equal*/
stwio r15, 0(r6) /*display in HEX2*/
br waitINk
nextk:
ldwio r19, 0(r3) /* read the JTAG UART data register */
andi r15, r19, 0x8000 /* check if there is new data */
beq r15, r0, go5 /* if no data, move */
movia r15, 0xff
and r19, r19, r15 /*ascii value*/
movia r16, 0x72 /*ascii for r = reset*/
beq r19, r16, loopk /*if r go the loop funtion */
go5:
ldwio r16, 0(r2) /*Store value of timer*/
beq r16, r21, countAk /*one second loop*/


beq r24, 0x09, fourA
beq r24, 0x09, threeA
beq r24, 0x09, twoA
beq r24, 0x09, oneA
beq r24, 0x09, zeroA

fourA:
stwio r0, 0(r2) /*Reset timer*********/
movia r15, 0x19 /*number 4*/
stwio r15, 0(r4) /*Display number*/
movia r15, 0x0f /*Turn on 4 LEDs*/
stwio r15, 0(r8) /*Turn on red LEDs*/
stwio r23, 4(r2) /*Start timer*/
br countAk

threeA:
stwio r0, 0(r2) /*Reset timer*******/
movia r15, 0x30 /*number 3*/
stwio r15, 0(r4) /*Display number*/
movia r15, 0x07 /*Turn on 3 LEDs*/
stwio r15, 0(r8) /*Turn on red LEDs*/
stwio r23, 4(r2) /*Start timer*/
br countAk

twoA:
stwio r0, 0(r2) /*Reset timer**********/
movia r15, 0x24 /*number 2*/
stwio r15, 0(r4) /*Display number*/
movia r15, 0x03 /*Turn on 2 LEDs*/
stwio r15, 0(r8) /*Turn on red LEDs*/
stwio r23, 4(r2) /*Start timer*/
br countAk

oneA:
stwio r0, 0(r2) /*Reset timer**********/
movia r15, 0x79 /*number 1*/
stwio r15, 0(r4) /*Display number*/
movia r15, 0x01 /*Turn on 1 LEDs*/
stwio r15, 0(r8) /*Turn on red LEDs*/
stwio r23, 4(r2) /*Start timer*/
br countAk

zeroA:
stwio r0, 0(r2) /*Reset timer**********/
movia r15, 0x40 /*number 0*/
stwio r15, 0(r4) /*Display number*/
movia r15, 0x00 /*Turn on 1 LEDs*/
stwio r15, 0(r8) /*Turn on red LEDs*/
ldwio r19, 0(r3) /* read the JTAG UART data register */
andi r15, r19, 0x8000 /* check if there is new data */
beq r15, r0, go0 /* if no data, move */
movia r15, 0xff
and r19, r19, r15 /*ascii value*/
movia r16, 0x72 /*ascii for r = reset*/
beq r19, r16, loopk /*if r go the loop funtion */

go0:
movia r15, 0x01 /*Substract Credit*/
sub r1, r1, r15
br credits

creditsk:
movia r15, 0x00
beq r15, r1, totalk

movia r15, 0x01
beq r15, r1, total1k

movia r15, 0x02
beq r15, r1, total2k

movia r15, 0x03
beq r15, r1, total3k

movia r15, 0x04
beq r15, r1, total4k

movia r15, 0x05
beq r15, r1, total5k

movia r15, 0x06
beq r15, r1, total6k

movia r15, 0x07
beq r15, r1, total7k

movia r15, 0x08
beq r15, r1, total8k

totalk:
movia r20, 0x00
stwio r20, 0(r9)
br resetk

total1k:
movia r20, 0x01
stwio r20, 0(r9)
br waitINk

total2k:
movia r20, 0x03
stwio r20, 0(r9)
br waitINk

total3k:
movia r20, 0x07
stwio r20, 0(r9)
br waitINk

total4k:
movia r20, 0x0f
stwio r20, 0(r9)
br waitINk

total5k:
movia r20, 0x1f
stwio r20, 0(r9)
br waitINk

total6k:
movia r20, 0x3f
stwio r20, 0(r9)
br waitINk

total7k:
movia r20, 0x7f
stwio r20, 0(r9)
br waitINk

total8k:
movia r20, 0xff
stwio r20, 0(r9)
br waitINk

	
.end    /*End of Main Program*/
