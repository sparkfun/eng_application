/*
Name:				lab4.c
By:				Chris Borke 4/1/2013

I2C Code used and modified was originally written in 2003 by Pedro Ignacio Martos.
This code is surrounded by asterisk bars, and was modified by myself to english.
The original code can be found in my project submission, and may be downloaded for
free at (http://sdccokr.dl9sec.de/dld/top03_pac07.zip) and used according to the 
GNU General Public License.

Notable Changes to Pedro's Code include:
	Proper (N)ACK handling in weeprom(), reeprom(), and read()
		-Previously, nacks were disregarded.  Function now resends if not acked.

Compiler:			SDCC 2.6.0
Eclipse_SDCC_Comp:	sdcc -c -mmcs51 --std-sdcc99 --verbose
Eclipse_SDCC_Link:	sdcc --code-loc 0x2040 --code-size 0x8000 --xram-loc 0x0000 --xram-size 0x8000 --model-large 

LCD Errors:
	#1 - Not a valid x,y coordinate
	#2 - Not an ascii hex
	#3 - Malloc Error for putstr
	#4 - Not a valid menu command
	#5 - Not a valid character to convert to hex
*/

#include <at89c51ed2.h>     //Includes the 8051/2 headers
#include <mcs51reg.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>
#include <ctype.h>

/*+++++++++++++++++++++++Defines and Global Variables++++++++++++++++++++*/

	#define	LCD_CMD ((unsigned char*)0xF000)
	#define	LCD_RD_INSTR ((unsigned char*)0xF300)
	#define	LCD_WR_INSTR ((unsigned char*)0xF200)
	#define LCD_BUSY ((unsigned char*)0xF100)
	#define UNLOCK 0x30
	#define BUSYFLAG 0x80
	#define FUNCTION_SET 0x38					//8-Bit, 2 Lines, 5x8 Dot Font
	#define DISP_OFF 0x08						//Display off, cursor off, blinking off
	#define DISP_ON 0xE							//Display on, cursor on, blinking off
	#define ENTRY 0x06							//Increment Cursor
	#define CLEAR 0x01							//Home and Clear the screen
	#define INVALID ((unsigned char*)0xB400)  	//For demonstration    
	
/*-----------------------------------------------------------------------*/

/*+++++++++++++++++++++++LCD Command Set+++++++++++++++++++++++++++++++++*/

	volatile xdata at LCD_CMD unsigned char lcd_cmd;
	volatile xdata at LCD_BUSY unsigned char lcd_busy;
	volatile xdata at LCD_WR_INSTR unsigned char lcd_write;
	
	volatile xdata at INVALID unsigned char invalid_command;
    
/*-----------------------------------------------------------------------*/

/*+++++++++++++++++++Initial Function Declarations+++++++++++++++++++++++*/

    void lcdinit();
    void delay_ms(unsigned int ms);
    void lcdbusywait();
    void lcdgotoaddr(unsigned char addr);
    void lcdgotoxy(unsigned char row, unsigned char column);
    void lcdputch(unsigned char cc);
    void lcdputstr(unsigned char *ss);
    void error(unsigned char errnum);
    void clear();
    void lcd_test();
   	char axtox(char c);
   	char xtoax(char c);
   	void i2c_demo();
   	void write_spin();

/*-----------------------------------------------------------------------*/

//I2C Function and port Declarations by Pedro Ignacio Martos
//**********************************************************************
// Port pin declaration
	volatile bit at 0x96 SCK;
	volatile bit at 0x97 SDA;
	
	
	// Prototype declaration
	void weeprom (char page, char address, char datum);
	char reeprom (char page, char address);
	char read (char readcmd);
	char outs (char datum);
	char out (char datum);
	char in (void);
	void stop (void);
//**********************************************************************

_sdcc_external_startup()
{	
	return 0;
}// End _sdcc_external_startup()



void main()
{	
	//For Grading Session
	//lcd_test();

	i2c_demo();

}


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*++++++++++++++++++++++Function Declarations+++++++++++++++++++++++++++++*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

//**********************************************************************
//* Function Name: void i2c_demo()
//**********************************************************************
//* Function Description: Demonstrates I2C bitbang functionality via LCD
//**********************************************************************
//* Input parameters: NONE
//**********************************************************************
//* Output parameters: NONE
//**********************************************************************
/// Notes: This function will post a specific series of values to LCD for
//		   grading purposes, and disallow invalid specifications.
//**********************************************************************
/*++++++++++++++++++++++++++i2c_demo() definition+++++++++++++++++++++++++*/
	void i2c_demo()
	{		
			
			unsigned char option;
			bool exit = false;
			unsigned char addr0;
			unsigned char addr1;
			unsigned char addr2;
			unsigned char addr3;
			unsigned char end0;
			unsigned char end1;
			unsigned char end2;
			unsigned char end3;
			unsigned char data0;
			unsigned char data1;
			unsigned char read;
			unsigned char y=0;
			unsigned char fill;
			unsigned char i,j,k;
			lcdinit();
			while(!exit)
			{
				printf_tiny("Chris Borke's I2C Program\n\r");
				printf_tiny("Supported Command Characters Include:\n\r");
				printf_tiny("\t!\t\t- Write Byte\n\r");
				printf_tiny("\t@\t\t- Read Byte\n\r");
				printf_tiny("\t#\t\t- Block Fill\n\r");
				printf_tiny("\t$\t\t- LCD Display\n\r");
				printf_tiny("\t^\t\t- Hex Dump\n\r");
				printf_tiny("\t&\t\t- exit()\n\r");
				printf_tiny("\n\r>> ");
				
				option = getchar();
				putchar(option);
				
				if(option == '!')//Start Write Byte
				{
					printf_tiny("Enter an address between 0x0000 and 0x07FF and data to write to.\n\r>> 0x");
								
					addr3 = getchar();
					putchar(addr3);
					
					addr2 = getchar();
					putchar(addr2);

					addr1 = getchar();
					putchar(addr1);
					
					addr0 = getchar();
					putchar(addr0);
					
					printf_tiny(":");
				
					data1 = getchar();
					putchar(data1);
					
					data0 = getchar();
					putchar(data0);
					
					printf_tiny("\n\r");
					printf_tiny("\n\r");
					
					if((axtox(addr3)) != 0)
					{
						printf_tiny("Invalid Address\n\r");
					}
					else if((axtox(addr2)) >= 0x08)
					{
						printf_tiny("Invalid Address\n\r");
					}
					else
					{
						weeprom((axtox(addr2)), (((axtox(addr1))<<4) | (axtox(addr0))) , (((axtox(data1))<<4) | (axtox(data0))) );

					}
				}//End Write Byte
				else if(option == '@')//Start Read Byte
				{
					printf_tiny("Enter an address between 0x0000 and 0x07FF to read.\n\r>> 0x");
								
					addr3 = getchar();
					putchar(addr3);

					
					addr2 = getchar();
					putchar(addr2);


					addr1 = getchar();
					putchar(addr1);

					
					addr0 = getchar();
					putchar(addr0);

					printf_tiny("\n\r\n\r");

					if(axtox(addr3) != 0)
					{
						printf_tiny("Invalid Address\n\r");
					}
					else if(axtox(addr2) >= 0x08)
					{
						printf_tiny("Invalid Address\n\r");
					}
					else
					{
						printf_tiny("Returned:\n\r");
						putchar(addr3);
						putchar(addr2);
						putchar(addr1);
						putchar(addr0);
						
						printf_tiny(":");
						
					
					read = reeprom(axtox(addr2), (((axtox(addr1))<<4) | (axtox(addr0))) );
					printf_tiny("%x", read);
					printf_tiny("\n\r\n\r");
					}
					

				}//End Read Byte
				else if(option == '#')//Start Block Fill
				{
				//Get Start, Stop, and Fill
					printf_tiny("Enter a start and end address between 0x0000 and 0x07FF to fill.\n\rStart>> 0x");
				
					addr3 = getchar();
					putchar(addr3);
					addr3 = axtox(addr3);
					
					addr2 = getchar();
					putchar(addr2);
					addr2 = axtox(addr2);
					
					addr1 = getchar();
					putchar(addr1);
					addr1 = axtox(addr1);
										
					addr0 = getchar();
					putchar(addr0);
					addr0 = axtox(addr0);
										
					printf_tiny("\n\rEnd>> 0x");
					
					end3 = getchar();
					putchar(end3);
					end3 = axtox(end3);
					
					end2 = getchar();
					putchar(end2);
					end2 = axtox(end2);
					
					end1 = getchar();
					putchar(end1);
					end1 = axtox(end1);
					
					end0 = getchar();
					putchar(end0);					
					end0 = axtox(end0);
					printf_tiny("\n\rFill>> 0x");				
				
					data1 = getchar();
					putchar(data1);
					data1 = axtox(data1);
					
					data0 = getchar();
					putchar(data0);
					data0 = axtox(data0);
					printf_tiny("\n\r\n\r");
					
				//Test if valid Start and Stop addresses
					if(addr3 != 0)
					{
						printf_tiny("Invalid Address1\n\r");
					}
					else if(addr2 >= 0x08)
					{
						printf_tiny("Invalid Address2\n\r");
					}
					else if((end2 < addr2))
					{
						printf_tiny("Start Address is smaller than End Address\n\r");
					}
					else if( ( (end2 == addr2) && (end1 < addr1) ) )
					{
						printf_tiny("Start Address is smaller than End Address\n\r");
					}
					else if( ( (end2 == addr2) && (end1 == addr1) && (end0 < addr0) ) )
					{
						printf_tiny("Start Address is smaller than End Address\n\r");
					}
					else if( ( (end2 == addr2) && (end1 == addr1) && (end0 == addr0) ) )
					{
						printf_tiny("Start Address same as End Address\n\r");
					}
					else	//Is Valid, fill memory
					{
							
							//Loop through memory blocks
							for( i = addr2; i <= end2; i++)
							{
								if( i == addr2 )//In first block
								{	
									fill=1;
									j = (((addr1)<<4) | (addr0));//Load j with starting address
									while(fill)
									{
										weeprom(i, j , (((data1)<<4) | (data0)) );
										if( (j == ((((end1)<<4) | (end0)))) && (i == end2) )//We've reached the end of the fill in a block
										{
											fill=0;//break
										}
										else if( j == 0xFF)//At end of block
										{
											fill=0;//break
										}
										j++;
									}
								}
								else if( i == end2 )//In last block
								{
									fill=1;
									j = 0;//Load j with starting address
									while(fill)
									{
										weeprom(i, j , (((data1)<<4) | (data0)) );
										if( (j == ((((end1)<<4) | (end0)))) && (i == end2) )//We've reached the end of the fill in a block
										{
											fill=0;//break
										}
										j++;
									}
								}
								else //Full blocks in between start and end
								{
									fill=1;
									j = 0;//Load j with starting address
									while(fill)
									{
										weeprom(i, j , (((data1)<<4) | (data0)) );
										if( j == 0xFF)//At end of block
										{
											fill=0;//break
										}
										j++;
									}
								}
								
							}//End block loop
					}
	
				}//End Block Fill
				else if(option == '$')//Start LCD Put
				{
					printf_tiny("Enter an address between 0x0000 and 0x07FF to read to the LCD.\n\r>> 0x");
								
					addr3 = getchar();
					putchar(addr3);

					addr2 = getchar();
					putchar(addr2);

					addr1 = getchar();
					putchar(addr1);
					
					addr0 = getchar();
					putchar(addr0);

					printf_tiny("\n\r\n\r");

					if(axtox(addr3) != 0)
					{
						printf_tiny("Invalid Address\n\r");
					}
					else if(axtox(addr2) >= 0x08)
					{
						printf_tiny("Invalid Address\n\r");
					}
					else
					{				
						read = reeprom(axtox(addr2), (((axtox(addr1))<<4) | (axtox(addr0))) );
						
						if(y == 4)
						{
							y = 0;	
						}
						
						lcdgotoxy(y,0);
						y++;
						
						lcdputch(addr3);
						lcdputch(addr2);
						lcdputch(addr1);
						lcdputch(addr0);
						lcdputch(':');
						lcdputch(xtoax(((read & 0xF0)>>4)));
						lcdputch(xtoax((read & 0x0F)));
						
						printf_tiny("\n\r\n\r");
					}
				}//End LCD Put
				else if(option == '^')//Start Block Read
				{
					//Get Start and Stop
					printf_tiny("Enter a start and end address between 0x0000 and 0x07FF to read.\n\rStart>> 0x");
				
					addr3 = getchar();
					putchar(addr3);
					addr3 = axtox(addr3);
					
					addr2 = getchar();
					putchar(addr2);
					addr2 = axtox(addr2);
					
					addr1 = getchar();
					putchar(addr1);
					addr1 = axtox(addr1);
										
					addr0 = getchar();
					putchar(addr0);
					addr0 = axtox(addr0);
										
					printf_tiny("\n\rEnd>> 0x");
					
					end3 = getchar();
					putchar(end3);
					end3 = axtox(end3);
					
					end2 = getchar();
					putchar(end2);
					end2 = axtox(end2);
					
					end1 = getchar();
					putchar(end1);
					end1 = axtox(end1);
					
					end0 = getchar();
					putchar(end0);					
					end0 = axtox(end0);

					printf_tiny("\n\r\n\r");
					
				//Test if valid Start and Stop addresses
					if(addr3 != 0)
					{
						printf_tiny("Invalid Address1\n\r");
					}
					else if(addr2 >= 0x08)
					{
						printf_tiny("Invalid Address2\n\r");
					}
					else if((end2 < addr2))
					{
						printf_tiny("Start Address is smaller than End Address\n\r");
					}
					else if( ( (end2 == addr2) && (end1 < addr1) ) )
					{
						printf_tiny("Start Address is smaller than End Address\n\r");
					}
					else if( ( (end2 == addr2) && (end1 == addr1) && (end0 < addr0) ) )
					{
						printf_tiny("Start Address is smaller than End Address\n\r");
					}
					else if( ( (end2 == addr2) && (end1 == addr1) && (end0 == addr0) ) )
					{
						printf_tiny("Start Address same as End Address\n\r");
					}
					else//Is Valid, read and put to screen
					{
						printf_tiny("\n\rADDR: 00|01|02|03|04|05|06|07|08|09|0A|0B|0C|0D|0E|0F\n\r");
						fill=1;//reused for first block run
						for( i = addr2; i <= end2; i++)//Traverse Blocks
						{

							for(j = 0; j <= 0x0F; j++)//Traverse 16-Byte Segments
							{
								//for first block filled
								if( (i == addr2) && (fill) )
								{
									j = addr1;
									fill=0;	
								}
								
								if( (i == end2) && (j == end1) )
								{
									break;
								}
								
								putchar('0');
								putchar(xtoax(i));
								putchar(xtoax(j));
								putchar('0');
								putchar(':');
								

								
								for( k = 0; k <= 0x0F; k++)//Traverse through 16 bytes
								{//printf_tiny("In k loop\n\r");
									printf_tiny(" ");
									read = reeprom(i, (((j)<<4) | (k)) );
	
										printf("%02x", read);
	
									
								}
								
								printf_tiny("\n\r");//Printed a whole line
							
							}
	
						}
						//Print the last line
						putchar('0');
						putchar(xtoax(end2));
						putchar(xtoax(end1));
						putchar('0');
						putchar(':');
						for( k = 0; k <= 0x0F; k++)//Traverse through 16 bytes
						{
							printf_tiny(" ");
							read = reeprom(end2, (((end1)<<4) | (k)) );
								printf("%02x", read);
						}
						
					printf_tiny("\n\r\n\r");		 
					}
					
				}//End Block Read
				else if(option == '&')
				{
					exit = true;
				}
				else
				{
					error('4');
					printf_tiny("This command character not recognized.\n\r");
				}
			}
	}
/*------------------------------------------------------------------------*/

//**********************************************************************
//* Function Name: char xtoax(char c)
//**********************************************************************
//* Function Description: Converts desired hex value to ASCII character
//**********************************************************************
//* Input parameters: Hex Represented Value
//**********************************************************************
//* Output parameters: ASCII Character
//**********************************************************************
/// Notes: This function will post error if unexpected conversion occurs
//**********************************************************************
/*+++++++++++++++++++++++++++++xtoax() definition+++++++++++++++++++++++++*/

	char xtoax(char c)
	{
		if((0x00 <= c) && (c <= 0x09))
		{
			return (c+0x30);
		}
		else if((0x0A <= c) && (c <= 0x0F))
		{
			return (c+0x37);
		}
		error('5');
		return 0xFF;
	}

/*------------------------------------------------------------------------*/

//**********************************************************************
//* Function Name: char axtox(char c)
//**********************************************************************
//* Function Description: Converts desired character to hexadecimal value
//**********************************************************************
//* Input parameters: ASCII Character
//**********************************************************************
//* Output parameters: Hex Represented Value
//**********************************************************************
/// Notes: This function will post error if unexpected conversion occurs
//**********************************************************************
/*+++++++++++++++++++++++++++++axtox() definition+++++++++++++++++++++++++*/

	char axtox(char c)
	{
		//Return 1-10 Hex
		if((0x30 <= c) && (c <= 0x39))
		{
			return (c-0x30);
		}
		//Return A-F Hex
		else if((0x41 <= c) && (c <= 0x46))
		{
			return (c-0x37);
		}
		//Return a-f Hex
		else if((0x61 <= c) && (c <= 0x66))
		{
			return (c-0x57);
		}
		error('2');
		return 0xFF;
	}

/*------------------------------------------------------------------------*/

//**********************************************************************
//* Function Name: void lcd_test()
//**********************************************************************
//* Function Description: Prints Desired Lab4 Grading Output for TA
//**********************************************************************
//* Input parameters: NONE
//**********************************************************************
//* Output parameters: NONE
//**********************************************************************
/// Notes: This function will output a series of characters and strings
//		   as per the PA4 grading rubric
//**********************************************************************
/*+++++++++++++++++++++++++lcd_test() definition++++++++++++++++++++++++++*/

	void lcd_test( void )
	{	//printf_tiny("\n\r");
		//lcdgotoxy(0x03,0);
		//lcdputstr("Chris Borke's");
		//lcdputstr("Lab#4 Signoff");
		unsigned char hex1;
		unsigned char hex2;
		unsigned char xdata putstr[100];
		unsigned char option;
		bool exit = false;
		while(!exit)
		{
			printf_tiny("Please test my LCD functionality!\n\r");
			printf_tiny("Supported Command Characters Include:\n\r");
			printf_tiny("\t!\t\t- lcdinit()\n\r");
			printf_tiny("\t@\t\t- lcdgotoaddr()\n\r");
			printf_tiny("\t#\t\t- lcdgotoxy()\n\r");
			printf_tiny("\t$\t\t- lcdputch()\n\r");
			printf_tiny("\t^\t\t- lcdputstr()\n\r");
			printf_tiny("\t&\t\t- exit()\n\r");
			printf_tiny("\n\r>> ");
			
			option = getchar();
			putchar(option);
			
			if(option == '!')
			{
				printf_tiny("Initializing LCD...\n\r");
				lcdinit();
			}
			else if(option == '@')
			{
				printf_tiny("Enter the two-hex address to goto, remember to stay on the screen!.\n\r>> 0x");
				hex1 = getchar();
				putchar(hex1);
				hex1 = axtox(hex1);
				hex2 = getchar();
				putchar(hex2);
				hex2 = axtox(hex2);
				printf_tiny("\n\r");
				lcdgotoaddr(((hex1<<4) | (hex2)));
			}		
			else if(option == '#')
			{
				printf_tiny("Enter a row and column in hex.\n\rRow>> 0x");
				hex1 = getchar();
				putchar(hex1);
				hex1 = axtox(hex1);
				printf_tiny("\n\rCol>> 0x");
				hex2 = getchar();
				putchar(hex2);
				hex2 = axtox(hex2);
				printf_tiny("\n\r");
				lcdgotoxy(hex1,hex2);
			}
			else if(option == '$')
			{
				printf_tiny("Enter a character to write to the lcd.\n\r>> ");
				hex1 = getchar();
				putchar(hex1);
				printf_tiny("\n\r");
				lcdputch(hex1);
			}
			else if(option == '^')
			{
				printf_tiny("Enter a string to write to the lcd.\n\r>> ");
				lcdputstr(gets(putstr));
				printf_tiny("\n\r");
				free(putstr);
			}
			else if(option == '&')
			{
				exit = true;
			}
			else
			{
				printf_tiny("This command character not recognized.\n\r");
				error('4');
			}
		}
	}

/*------------------------------------------------------------------------*/

//**********************************************************************
//* Function Name: void lcdputstr(unsigned char *ss)
//**********************************************************************
//* Function Description: Prints Desired String to LCD Display
//**********************************************************************
//* Input parameters: Pointer to NULL Terminated String to be Displayed
//**********************************************************************
//* Output parameters: NONE
//**********************************************************************
/// Notes: This function will automatically wrap string to the next line
//		   in the even it wraps to the end of the LCD Column Limit
//**********************************************************************
/*++++++++++++++++++++++++++++lcdputstr() definition++++++++++++++++++++++*/

	void lcdputstr(unsigned char *ss)
	{		 
		//Get current position
		unsigned char pos = (lcd_busy & 0x7F);
		while( *ss )
		{
			lcdputch( *ss++ );
			pos++;
			//Wrap
			if((pos & 0x0F) == 0x0F)
			{ 
				lcdputch( *ss++ );
				if(pos == 0x0F)
				{
					pos = 0x40;
					lcdgotoxy( 0x01,0 );
				}
				else if(pos == 0x4F)
				{
					pos = 0x10;
					lcdgotoxy( 0x02,0 );
				}
				else if(pos == 0x1F)
				{
					pos = 0x50;
					lcdgotoxy( 0x03,0 );
				}
				else if(pos == 0x5F)
				{
					pos = 0x00;
					lcdgotoxy( 0x00,0 );
				}
			}
		}
	}

/*------------------------------------------------------------------------*/

//**********************************************************************
//* Function Name: void clear()
//**********************************************************************
//* Function Description: Clears Entire LCD Display
//**********************************************************************
//* Input parameters: NONE
//**********************************************************************
//* Output parameters: NONE
//**********************************************************************
/// Notes: NONE
//**********************************************************************
/*++++++++++++++++++++++++++++clear() definition++++++++++++++++++++++++++*/

	void clear()
	{
		lcdbusywait();
		lcd_cmd = CLEAR;
		lcdbusywait();
	}

/*------------------------------------------------------------------------*/

//**********************************************************************
//* Function Name: void error(unsigned char errnum)
//**********************************************************************
//* Function Description: Puts Error Code to First Line of LCD
//**********************************************************************
//* Input parameters: Error Code to be echo
//**********************************************************************
//* Output parameters: NONE
//**********************************************************************
/// Notes: Error Code will overwrite previously written data
//**********************************************************************
/*++++++++++++++++++++++++++++error() definition++++++++++++++++++++++++++*/

	void error(unsigned char errnum)
	{
		clear();
		lcdgotoxy(0x00,0);
		lcdputch('E');
		lcdputch('R');
		lcdputch('R');
		lcdputch('O');
		lcdputch('R');
		lcdputch(' ');
		lcdputch('#');
		lcdputch(errnum);
		
	}

/*------------------------------------------------------------------------*/

//**********************************************************************
//* Function Name: void lcdputch(unsigned char cc)
//**********************************************************************
//* Function Description: Puts Character to Current LCD Location
//**********************************************************************
//* Input parameters: Character To Place at Current LCD Location
//**********************************************************************
//* Output parameters: NONE
//**********************************************************************
/// Notes: Be sure to specify desired location before putting character!
//**********************************************************************
/*+++++++++++++++++++++++++lcdputch() definition++++++++++++++++++++++++++*/

	void lcdputch(unsigned char cc)
	{
		lcdbusywait();
		lcd_write = cc;
		lcdbusywait();
	}

/*------------------------------------------------------------------------*/

//**********************************************************************
//* Function Name: void lcdgotoxy(unsigned char row, unsigned char column)
//**********************************************************************
//* Function Description: Invoke Specific LED Address Location
//**********************************************************************
//* Input parameters: X,Y Coordinates of Specific LCD Screen Segment to Address
//**********************************************************************
//* Output parameters: NONE
//**********************************************************************
/// Notes: Will throw error if exceeding the bounds of LCD Row Limit
//**********************************************************************
/*++++++++++++++++++++++++lcdgotoxy() definition++++++++++++++++++++++++++*/

	void lcdgotoxy(unsigned char row, unsigned char column)
	{
		if(row == 0x00)
		{
			lcdgotoaddr(column);
		}
		else if(row == 0x01)
		{
			column |= 0x40;
			lcdgotoaddr(column);
		}
		else if(row == 0x02)
		{
			column |= 0x10;
			lcdgotoaddr(column);
		}
		else if(row == 0x03)
		{
			column |= 0x50;
			lcdgotoaddr(column);
		}
		else
		{
			error('1');
		}
	}

/*------------------------------------------------------------------------*/

//**********************************************************************
//* Function Name: void lcdgotoaddr(unsigned char addr)
//**********************************************************************
//* Function Description: Invoke Specific LCD Address Location
//**********************************************************************
//* Input parameters: Address location of LCD screen segment to address
//**********************************************************************
//* Output parameters: NONE
//**********************************************************************
/// Notes: NONE
//**********************************************************************
/*++++++++++++++++++++++lcdgotoaddr() definition++++++++++++++++++++++++++*/

	void lcdgotoaddr(unsigned char addr)
	{
			addr |= 0x80;
			lcdbusywait();
			lcd_cmd = addr;
			lcdbusywait();
	}

/*------------------------------------------------------------------------*/

//**********************************************************************
//* Function Name: void lcdinit()
//**********************************************************************
//* Function Description: Initalizes LCD
//**********************************************************************
//* Input parameters: NONE
//**********************************************************************
//* Output parameters: NONE
//**********************************************************************
/// Notes: See Datasheet, Specific interrupt sequence required for init
//**********************************************************************
/*++++++++++++++++++++++++init_lcd() declaration++++++++++++++++++++++++++*/
	void lcdinit()
	{
		delay_ms(17);
		lcd_cmd = UNLOCK;
		delay_ms(5);
		lcd_cmd = UNLOCK;
		delay_ms(1);
		lcd_cmd = UNLOCK;
		lcdbusywait();
		lcd_cmd = FUNCTION_SET;
		lcdbusywait();
		lcd_cmd = DISP_OFF;	
		lcdbusywait();
		lcd_cmd = DISP_ON;
		lcdbusywait();
		lcd_cmd = ENTRY;
		lcdbusywait();
		lcd_cmd = CLEAR;
		lcdbusywait();
	}
/*------------------------------------------------------------------------*/

//**********************************************************************
//* Function Name: void delay_ms(unsigned int ms)
//**********************************************************************
//* Function Description: Delay Function Specified in mS
//**********************************************************************
//* Input parameters: Milliseconds to delay
//**********************************************************************
//* Output parameters: NONE
//**********************************************************************
/// Notes: Accuracy is not guaranteed, architecture specific
//**********************************************************************
/*+++++++++++++++++++++++++++delay() declaration++++++++++++++++++++++++++*/
	void delay_ms(unsigned int ms)
	{
		ms*=50;
		ms-=9;
		while(ms)
			ms--;
	}
/*------------------------------------------------------------------------*/

//**********************************************************************
//* Function Name: void lcdbusywait()
//**********************************************************************
//* Function Description: Interruptable SpinLock for LCD
//**********************************************************************
//* Input parameters: NONE
//**********************************************************************
//* Output parameters: NONE
//**********************************************************************
/// Notes: Will break on free LCD or not BUSYFLAG
//**********************************************************************
/*++++++++++++++++++++++++busy_block() declaration++++++++++++++++++++++++++*/
	void lcdbusywait()
	{
		while( lcd_busy & BUSYFLAG)
		{
			//no-op
		}
	}
/*------------------------------------------------------------------------*/

//**********************************************************************
//* Function Name: void putchar(char c)
//**********************************************************************
//* Function Description: get character from input
//**********************************************************************
//* Input parameters: Transmit Ascii Character
//**********************************************************************
//* Output parameters: NONE
//**********************************************************************
/// Notes: NONE
//**********************************************************************
/*++++++++++++++++++++++putchar() definition+++++++++++++++++++++++++++++*/

    void putchar(char c)
    {
        while (!TI); //Spin until TX ready
        SBUF = c;
        TI = 0;
    }//End putchar()

/*-----------------------------------------------------------------------*/

//**********************************************************************
//* Function Name: char getchar()
//**********************************************************************
//* Function Description: get character from input
//**********************************************************************
//* Input parameters: NONE
//**********************************************************************
//* Output parameters: Received Ascii Character
//**********************************************************************
/// Notes: NONE
//**********************************************************************
/*++++++++++++++++++++++getchar() definition+++++++++++++++++++++++++++++*/

    char getchar()
    {
        while (!RI); //Spin until TX ready
        RI = 0;
        return SBUF;
    }//End getchar

/*-----------------------------------------------------------------------*/

//I2C Function Implementations by Pedro Ignacio Martos, edited by Chris Borke
//**********************************************************************
//**********************************************************************
//**********************************************************************
//**********************************************************************
//**********************************************************************

//**********************************************************************
//* Function Name: void write_spin()
//**********************************************************************
//* Function Description: Spins on write wait
//**********************************************************************
//* Input parameters: NONE
//**********************************************************************
//* Output parameters: NONE
//**********************************************************************
/// Notes: NONE
//**********************************************************************
/*++++++++++++++++++++++++++++i2c_demo() definition+++++++++++++++++++++++*/

	//Using Code I2C code borrowed by Pedro Ignacio Martos
	void write_spin( void )
	{
		volatile unsigned char nack = 1;
		while(nack)
		{
			//Send Start
			//Send Control Byte, R/~W =0
			//outs (0xA0); // send start and dummy write control to block 0x00
			nack = SDA;	//Breaks if acked
		}
	}
/*------------------------------------------------------------------------*/

//**********************************************************************
//* Function Name: void weeprom (char page, char address, char datum)
//**********************************************************************
//* Function Description: write routine for I2C EEPROM
//**********************************************************************
//* Input parameters:
//* - char page: number of 256 bytes block to use
//* - char address: position inside the 256 bytes block
//* - char datum: 1 byte data to write
//**********************************************************************
//* Output parameters: NONE
//**********************************************************************
/// Notes: NONE
//**********************************************************************
void weeprom (char page, char address, char datum)
{
	char WRCMD; // auxiliary storage to build the write command
	char nack0 = 1;
	char nack1 = 1;
	char nack2 = 1;
	page = page << 1; // move A0, A1 & A2 to their positions
	page = page & 0xFE; // clear r/w bit
	WRCMD = page | 0xA0; // build the write command
	while(nack0 || nack1 || nack2)
	{
		nack0 = outs (WRCMD); // send the write command with start condition
		nack1 = out (address); // send address
		nack2 = out (datum); // send data
		stop (); // send stop condition
	}
}


//**********************************************************************
//* Function Name: char reeprom (char page, char address)
//**********************************************************************
//* Function Description: read routine for I2C EEPROM
//**********************************************************************
//* Input parameters:
//* - char page: number of 256 bytes block to use
//* - char address: position inside the 256 bytes block
//**********************************************************************
//* Output parameters:
//* - char with the data from memory
//**********************************************************************
/// Notes: NONE
//**********************************************************************
char reeprom (char page, char address)
{
	char aux; // auxiliary storage
	char WRCMD; // aux. storage for the write command (to send the add.)
	//char nack0 = 1;
	//char nack1 = 1;
	aux = page; // preparing the write command
	aux = aux << 1; // move A0, A1 & A2 to their positions 
	aux = aux & 0xFE; // clear r/w bit
	WRCMD = aux | 0xA0; // build the write command
	
	//while(nack0 || nack1)
	//{
	outs (WRCMD); // send the write command with start condition
	out (address); // send addres
	aux = read (WRCMD); // send read command and receive data
	//}
	return (aux); // return solicited data 
}

//**********************************************************************
//* Function Name: char read (char readcmd)
//**********************************************************************
//* Function Description: reads 1 byte from current memory position
//**********************************************************************
//* Input parameters:
//* - char readcmd: read command byte with A0, A1 & A2 embedded
//**********************************************************************
//* Output parameters:
//* - char with current address data
//**********************************************************************
/// Notes: NONE
//**********************************************************************
char read (char readcmd) //Random Read
{
	char RDCMD; // auxiliary storage to build the read command
	char aux; // auxiliary storage
	RDCMD = readcmd | 0x01; // set r/w bit
	outs(RDCMD); // send read command with start condition
	aux = in(); // read current position
	stop (); // send stop condition
	return (aux); // return current position data
}


//**********************************************************************
//* Function Name: void outs (char datum)
//**********************************************************************
//* Function Description: sends to memory the input parameter with a
//*                       start condition
//**********************************************************************
//* Input parameters:
//* - char datum: data byte to send to the memory
//**********************************************************************
//* Output parameters: NONE
//**********************************************************************
/// Notes: NONE
//**********************************************************************
char outs (char datum)
{
	char nack;
	char i; // index 
	char aux; // auxiliary storage

		SDA = 1; // set port pin SDA to insure data is HI
		SCK = 1; // set port pin SCK to insure clock is HI
	_asm
		nop
		nop
		nop
		nop
		nop
	_endasm;
		SDA = 0; // start condition, data = 0
	_asm
		nop
		nop
		nop
		nop
		nop
	_endasm;
		SCK = 0; // clock = 0
		for (i = 0; i < 8; i++) // bit shifting cycle
		{
			aux = datum & 0x80; // check MSB bit
			if (aux == 0) // MSB = 0
				SDA = 0; // then SDA = 0
			else
				SDA = 1; // else MSB =1, then SDA = 1
			SCK = 1; // clock = 1
			//Give the EEPROM time to read the bit
	_asm
			nop
			nop
			nop
			nop
			nop
	_endasm;
			SCK = 0; // clock = 0
			datum = datum << 1; // rotate for next bit 
		}
		SDA = 1; // set port pin for ack
	_asm
		nop
		nop
		nop
	_endasm;
		SCK = 1; // clock ack
	_asm
		nop
		nop
		nop
		nop
		nop
	_endasm;
	nack = SDA;//added for ack
	SCK = 0; // clock = 0
	return nack;

}


//**********************************************************************
//* Function Name: void out (char datum)
//**********************************************************************
//* Function Description: sends to memory the input parameter without a
//*                       start condition
//**********************************************************************
//* Input parameters:
//* - char datum: data byte to send to the memory
//**********************************************************************
//* Output parameters: NONE
//**********************************************************************
/// Notes: NONE
//**********************************************************************
char out (char datum)
{
	char nack;
	char i; // index
	char aux; // auxiliary storage

		for (i = 0; i < 8; i++) // bit shifting cycle
		{
			aux = datum & 0x80; // check MSB bit
			if (aux == 0) // MSB = 0
				SDA = 0; // then SDA = 0
			else
				SDA = 1; // else MSB = 1, the SDA = 1
			SCK = 1; // clock = 1
	_asm
			nop
			nop
			nop
			nop
			nop
	_endasm;
			SCK = 0; // clock = 0
			datum = datum << 1; // rotate for next bit
		}
		SDA = 1; // set port pin for ack
	_asm
		nop
		nop
		nop
	_endasm;
		SCK = 1; // clock ack
	_asm
		nop
		nop
		nop
		nop
		nop
	_endasm;
	nack = SDA;//added for ack
	SCK = 0; // clock = 0
	return nack;
}


//**********************************************************************
//* Function Name: char in (void)
//**********************************************************************
//* Function Description: receives a byte from memory 
//**********************************************************************
//* Input parameters: NONE
//**********************************************************************
//* Output parameters: 
//* - char with the byte received from memory
//**********************************************************************
/// Notes: NONE
//**********************************************************************
char in (void)
{
	char i; // index
	char aux = 0; // auxiliary storage
	SDA = 1; // insure port pin = 1 for input
	for (i = 0; i < 8; i++) // bit shifting cycle
	{
		SCK = 0; // clock = 0
_asm
		nop
		nop
		nop
		nop
		nop
		nop
_endasm;
		SCK = 1; // clock = 1
		aux = aux << 1; // load bit position
		if (SDA) // check SDA data from port pin
			aux = aux | 0x01; // if port pin = 1, set LSB (bit position)
		else
			aux = aux & 0xFE; // else port pin = ,clear LSB (bit position)
	}
	SCK = 0; // clock = 0
	return (aux); // return data received
}


//**********************************************************************
//* Function Name: void stop (void)
//**********************************************************************
//* Function Description: send stop condition 
//**********************************************************************
//* Input parameters: NONE
//**********************************************************************
//* Output parameters: NONE
//**********************************************************************
/// Notes: NONE
//**********************************************************************
void stop (void)
{
	SDA = 0; // stop condition, data = 0
_asm
	nop
	nop
	nop
	nop
	nop
_endasm;
	SCK = 1; // clock = 1
_asm
	nop
	nop
	nop
	nop
	nop
_endasm;
	SDA = 1; // data = 1
}

//**********************************************************************
