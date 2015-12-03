/*******************************************************************************

Copyright (c) 2015 Caleb Jamison

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

********************************************************************************

This is a simple library of helper functions for the atmega32uX family of
microcontrollers. It includes enable/init functions, interrupt helpers,
and synchronous and asynchronous send and receive functions. The library's only
dependency is avr-libc.

The library uses the tx/rx circuitry attached to pins PD2 (RX, aka Arduino
pin 0 on the leonardo and similar) and PD3 (TX, aka Arduino pin 1) and
timer1 for a readloop timeout.

The best leonardo pinout:
http://www.pighixxx.com/test/portfolio-items/leonardo/

*******************************************************************************/

// This is the output baud of the serial port,
// define must be before util/setbaud
#define BAUD 19200

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/setbaud.h>

#include "uart.h"

volatile char* uart_tx_buff;
volatile unsigned int uart_tx_len;
volatile unsigned int uart_tx_hdptr;

volatile char* uart_rx_buff;
volatile unsigned int uart_rx_len;
volatile unsigned int uart_rx_hdptr;

void uart_init( void )
{
  // Define Baud rate and 2x speed with the help of the setbaud macro
  UBRR1H = UBRRH_VALUE;
  UBRR1L = UBRRL_VALUE;
  #if USE_2X
  UCSR1A |= (1 << U2X1);
  #else
  UCSR1A &= ~(1 << U2X1);
  #endif

  // 8-bit data
  UCSR1C |= (1 << USBS1) | (1 << UCSZ11) | (1 << UCSZ10);

  // Setup timer for clock timout
  // Clk = 16Mhz
  // Prescale Clk/1024
  TCCR1B |= ( ( 1 << CS12 ) | ( 1 << CS10 ) );
}

void uart_start( void )
{
  // Enable TX & RX circuitry
  UCSR1B |= (1 << RXEN1) | (1 << TXEN1);
}

void uart_stop( void )
{
  // Disable TX & RX circuitry
  UCSR1B &= ~( (1 << RXEN1) | (1 << TXEN1) );
}

void uart_etxint( void )
{
  // Enable TX interrupt
  UCSR1B |= (1 << TXCIE1);
}

void uart_dtxint( void )
{
  // Disable TX interrupt
  UCSR1B &= ~(1 << TXCIE1);
}

void uart_erxint( void )
{
  // Enable RX interrupt
  UCSR1B |= (1 << RXCIE1);
}

void uart_drxint( void )
{
  // Disable RX interrupt
  UCSR1B &= ~(1 << RXCIE1);
}

void uart_putchar( const char c )
{
  // Wait for uart to be ready
  while( ( UCSR1A & (1 << UDRE1) ) == 0 ) {};
  // Senc char to uart
  UDR1 = c;
}

char uart_getchar( char* c )
{
  // Reset timer
  TCNT1 = 0;

  while( ( UCSR1A & (1 <<  RXC1) ) == 0 )
  {
    // If timer > 1s return fail
    if ( TCNT1 >= 15625 )
    {
      *c = 0;
      return 0;
    }
  }

  // Copy the char from the uart
  *c = UDR1;
  return 1;
}

unsigned int uart_sync_write( const char* buff, const unsigned int len )
{
  // Loop through buff printing chars to serial
  unsigned int i;
  for( i = 0; i < len; i++ )
  {
    uart_putchar( buff[i] );
  }

  // Make sure everything is out before returning
  while( ( UCSR1A & (1 << TXC1) ) == 0 ) {};
  return i;
}

unsigned int uart_sync_write_to( const char* buff, const unsigned int len, const char endchar )
{
  // Loop through buff printing chars to serial
  unsigned int i;
  for( i = 0; i < len; i++ )
  {
    uart_putchar( buff[i] );
    if( buff[i] == endchar )
    {
      // Make sure everything is out before returning
      while( (UCSR1A & (1 << TXC1) ) == 0) {};
      return i;
    }
  }

  // Make sure everything is out before returning
  while( ( UCSR1A & (1 << TXC1) ) == 0 ) {};
  return i;
}

void uart_async_write( char* buff, const unsigned int len )
{
  // Set local vars to argument vars
  uart_tx_buff = buff;
  uart_tx_len = len;
  // Reset uart_tx_hdprt
  uart_tx_hdptr = 0;
  // Enable interrupt
  uart_etxint( );
  // Kick off transmission
  uart_putchar( uart_tx_buff[uart_tx_hdptr++] );
}

ISR( USART1_TX_vect )
{
  if( uart_tx_hdptr < uart_tx_len )
  {
    UDR1 = uart_tx_buff[uart_tx_hdptr++];
  }
  else
  {
    uart_dtxint( );
  }
}

int uart_sync_read( char* buff, const unsigned int len )
{
  unsigned int i;
  for( i = 0; i < len; i++ )
  {
    if ( !uart_getchar( buff + i ) )
    {
      return -1;
    }
  }
  return i;
}

int uart_sync_read_to( char* buff, const unsigned int len, const char endchar )
{
  unsigned int i;
  for( i = 0; i < len; i++ )
  {
    if ( !uart_getchar( buff + i ) )
    {
      return -1;
    }
    if( buff[i] == endchar )
    {
      return i;
    }
  }
  return i;
}

void uart_async_read( char* buff, const unsigned int len )
{
  // Set local vars to argument vars
  uart_rx_buff = buff;
  uart_rx_len = len;
  // Reset hdprt
  uart_rx_hdptr = 0;
  // Enable uart tx interrupt
  uart_erxint( );
}

ISR( USART1_RX_vect )
{
  if( uart_rx_hdptr < uart_rx_len )
  {
    uart_rx_buff[uart_rx_hdptr++];
  }
  else
  {
    uart_drxint( );
  }
}
