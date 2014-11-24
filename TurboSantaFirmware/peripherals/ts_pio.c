/*
 * ts_pio.c
 *
 * Created: 23 Nov 2014 6:27:44 PM
 *  Author: Andrew
 */ 
#include "ts_pio.h"

void configure_pin_as_pio_output(Pio* controller, uint32_t pin, uint8_t initial_state) {
	// PIO_OER  = Output Enable Register (1 = Enables the output on the I/O line)
	controller->PIO_OER = pin;
	// PIO_PER  = PIO Enable Register (1 = Enable PIO control of the corresponding pin, disables peripheral control of the pin)
	controller->PIO_PER = pin;
	
	(initial_state) ? set_pin_high(controller, pin) : set_pin_low(controller, pin);
}

void configure_pin_as_pio_input(Pio* controller, uint32_t pin) {
	// PIO_OER  = Output Disable Register (1 = Enables the input function of the pin)
	controller->PIO_ODR = pin;
	// PIO_PER  = PIO Enable Register (1 = Enable PIO control of the corresponding pin, disables peripheral control of the pin)
	controller->PIO_PER = pin;
}

void set_pin_high(Pio* controller, uint32_t pin) {
	controller->PIO_SODR = pin;
}

void set_pin_low(Pio* controller, uint32_t pin) {
	controller->PIO_CODR = pin;
}