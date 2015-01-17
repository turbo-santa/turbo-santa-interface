/*
 * ts_usart_ftdi.c
 *
 * Created: 17 Jan 2015 12:45:55 PM
 *  Author: Andrew
 */ 

#include "ts_usart_ftdi.h"

void usart0_ftdi_putchar(const char c) {
	while ( !(USART0->US_CSR & US_CSR_TXRDY)) {}
		__asm__ volatile("nop");
		__asm__ volatile("nop");
		__asm__ volatile("nop");
		__asm__ volatile("nop");
		__asm__ volatile("nop");
		__asm__ volatile("nop");
		__asm__ volatile("nop");
		__asm__ volatile("nop");
		__asm__ volatile("nop");
		__asm__ volatile("nop");
		__asm__ volatile("nop");
		__asm__ volatile("nop");
		__asm__ volatile("nop");
		__asm__ volatile("nop");
		__asm__ volatile("nop");
		__asm__ volatile("nop");
		__asm__ volatile("nop");
		__asm__ volatile("nop");
		
	USART0->US_THR = c;
}

void usart0_ftdi_putstring(const char *c) {
	while (*c != '\0') {
		usart0_ftdi_putchar(*(c++));
	}
}