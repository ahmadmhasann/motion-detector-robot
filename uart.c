/*
 * UART.c
 *
 *  Created on: Oct 12, 2019
 *      Author: Ahmed Hassan
 */
#include "Types.h"
#include "Macros.h"
#include <avr/io.h>
#include "DIO.h"



void uart_vid_init (void) {

	//SETTING BAUD RATE = 9600
	UBRRH = 0b00000000;
	UBRRL = 103;
	setBit(UCSRA,1);


	/*
	 * UCSRC REGISTER
	 * BIT 7:   1 to access UCSRC
	 * BIT 6:   0 for Asynchronous Operation
	 * BIT 5,4: 00 to disable Parity Mode
	 * BIT 3:   0 for 1 Stop Bit
	 * BIT 2,1: 11 for 8 Bit Character Size
	 * BIT 0:   0 Not Used (Synchronous mode only)
	 */
	dio_vid_set_pin_direction(D, 1, OUTPUT);
	dio_vid_set_pin_direction(D, 0, INPUT);

	UCSRC = 0b10000110;

	setBit(UCSRB,4);
	setBit(UCSRB,3);

	//clrBit(UCSRB,2);
	clrBit(UCSRB,6);
	clrBit(UCSRB,7);





}



void uart_vid_transmit (u8 data)
{
	/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<UDRE)) );
	/* Put data into buffer, sends the data */
	UDR = data;
}

u8 uart_u8_recieve( void )
{
	/* Wait for data to be received */
	while ( !(UCSRA & (1<<RXC)) );
	/* Get and return received data from buffer */
	return UDR;
}