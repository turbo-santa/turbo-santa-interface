/*
 * ts_usart_ftdi.c
 *
 * Created: 17 Jan 2015 12:45:55 PM
 *  Author: Andrew
 */ 

#include "ts_usart_ftdi.h"

void usart0_ftdi_putchar(const char c) {
	while ( !(USART0->US_CSR & US_CSR_TXRDY)) {}
		
	USART0->US_THR = c;
}

void usart0_ftdi_putstring(const char *c) {
	while (*c != '\0') {
		usart0_ftdi_putchar(*(c++));
	}
}

char usart0_ftdi_getchar(void) {
	while ( !(USART0->US_CSR & US_CSR_RXRDY)) {}
		
	return USART0->US_RHR;
}

char digit_to_char(int digit)
{
	if (digit >= 10) {
		return 'A' + (digit - 10);
	}
	else {
		return '0' + digit;
	}
}

void print_hex_byte(int hex)
{
	usart0_ftdi_putchar('0');
	usart0_ftdi_putchar('x');
	usart0_ftdi_putchar(digit_to_char((hex >> 4) & 0xF));
	usart0_ftdi_putchar(digit_to_char(hex & 0xF));
	usart0_ftdi_putchar('\r');
	usart0_ftdi_putchar('\n');
}