/*
 * ts_usart_ftdi.h
 *
 * Created: 17 Jan 2015 12:46:39 PM
 *  Author: Andrew
 */ 


#ifndef TS_USART_FTDI_H_
#define TS_USART_FTDI_H_

#include "sam.h"

void usart0_ftdi_putchar(const char c);
void usart0_ftdi_putstring(const char *c);



#endif /* TS_USART_FTDI_H_ */