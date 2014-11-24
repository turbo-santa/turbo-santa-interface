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
	// PIO_IDR  = PIO Interrupt Disable Register (1 = Disable interrupts on the pin)
	controller->PIO_IDR = pin;
	// Set the initial state
	(initial_state) ? set_pin_high(controller, pin) : set_pin_low(controller, pin);
}

void configure_pin_as_pio_input(Pio* controller, uint32_t pin) {
	// PIO_OER  = Output Disable Register (1 = Enables the input function of the pin)
	controller->PIO_ODR = pin;
	// PIO_PER  = PIO Enable Register (1 = Enable PIO control of the corresponding pin, disables peripheral control of the pin)
	controller->PIO_PER = pin;
	// PIO_IDR  = PIO Interrupt Disable Register (1 = Disable interrupts on the pin)
	controller->PIO_IDR = pin;
}

void configure_pin_as_peripheral(Pio* controller, uint32_t pin, uint8_t peripheral) {
	// PIO_PDR  = PIO Disable Register (1 = Disables PIO from controlling the corresponding pin (enables peripheral control of the pin))
	controller->PIO_PDR = pin;
	// PIO_IDR  = PIO Interrupt Disable Register (1 = Disable interrupts on the pin)
	controller->PIO_IDR = pin;
	// We need to configure the PIO_ABCDSR0 and PIO_ABCDSR1 registers to route this pin to the specified peripheral
	switch (peripheral) {
		case PERIPHERAL_A:
			// The pin is routed to Peripheral A if PIO_ABCDSR0 is set to 0 and if PIO_ABCDSR1 is set to 0
			controller->PIO_ABCDSR[0] = controller->PIO_ABCDSR[0] & (!pin);
			controller->PIO_ABCDSR[1] = controller->PIO_ABCDSR[1] & (!pin);
			break;
		case PERIPHERAL_B:
			// The pin is routed to Peripheral B if PIO_ABCDSR0 is set to 1 and if PIO_ABCDSR1 is set to 0
			controller->PIO_ABCDSR[0] = controller->PIO_ABCDSR[0] |   pin;
			controller->PIO_ABCDSR[1] = controller->PIO_ABCDSR[1] & (!pin);
			break;
		case PERIPHERAL_C:
			// The pin is routed to Peripheral C if PIO_ABCDSR0 is set to 0 and if PIO_ABCDSR1 is set to 1
			controller->PIO_ABCDSR[0] = controller->PIO_ABCDSR[0] & (!pin);
			controller->PIO_ABCDSR[1] = controller->PIO_ABCDSR[1] |	  pin;
			break;
		case PERIPHERAL_D:
			// The pin is routed to Peripheral D if PIO_ABCDSR0 is set to 1 and if PIO_ABCDSR1 is set to 1
			controller->PIO_ABCDSR[0] = controller->PIO_ABCDSR[0] |   pin;
			controller->PIO_ABCDSR[1] = controller->PIO_ABCDSR[1] |	  pin;
			break;
		case PERIPHERAL_SYS:
		case PERIPHERAL_GPIO:
		case PERIPHERAL_EXTRA:
		default:
			// The other peripherals are not set via this method 
			break;
		
	}
}

void set_pin_high(Pio* controller, uint32_t pin) {
	controller->PIO_SODR = pin;
}

void set_pin_low(Pio* controller, uint32_t pin) {
	controller->PIO_CODR = pin;
}