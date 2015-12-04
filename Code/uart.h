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

#ifndef _uart_h
#define _uart_h

// Configure uart
void uart_init( void );
// Start tx/rx circutry
void uart_start( void );
// Stop tx/rx circutry
void uart_stop( void );

// Enable tx intrrupt
void uart_etxint( void );
// Disable tx intrrupt
void uart_dtxint( void );
// Enable rx intrrupt
void uart_erxint( void );
// Disable rx intrrupt
void uart_drxint( void );

// Send a single char to the uart
void uart_putchar( const char c );
// Get a single char from the uart
char uart_getchar( char* c );

// Synchronously write a buffer to the uart of a fixed length
unsigned int uart_sync_write( const char* buff, const unsigned int len );
// Synchronously write a buffer to the uart of a fixed length, but stop if a specific char is found
unsigned int uart_sync_write_to( const char* buff, const unsigned int len, const char endchar );
// Asynchronously write a buffer to the uart of a fixed length
void uart_async_write( char* buff, const unsigned int len );

// Synchronously read a buffer from the uart of a fixed length
int uart_sync_read( char* buff, const unsigned int len );
// Synchronously read a buffer from the uart of a fixed length, but stop if a specific char is found
int uart_sync_read_to( char* buff, const unsigned int len, const char endchar );
// Asynchronously read a buffer from the uart of a fixed length
void uart_async_read( char* buff, const unsigned int len );

#endif /* _uart_h */

