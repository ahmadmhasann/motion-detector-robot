/*
 * UART.h
 *
 *  Created on: Oct 12, 2019
 *      Author: Ahmed Hassan
 */

#ifndef UART_H_
#define UART_H_

void uart_vid_init (void);
u8 uart_u8_recieve( void );
void uart_vid_transmit (u8 data);


#endif /* UART_H_ */
