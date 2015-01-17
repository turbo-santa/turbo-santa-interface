/*
 * TurboSantaInterface.c
 *
 * Created: 16 Nov 2014 3:55:30 PM
 *  Author: Andrew
 */ 

#include "sam.h"
#include "startup.h"


int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	
	// Our board-specific configuration
	initialize_watchdog();
	initialize_xtal();
	initialize_pmc_clocks();
	initialize_pio();
	initialize_pwm();
	initialize_usart0();
	initialize_cart_pio();
	
	usart0_ftdi_putstring("Welcome to the TurboSanta Interface GameBoy (Color) ROM Dumper!\r\n");
	usart0_ftdi_putstring("\r\n");
	usart0_ftdi_putstring("Press any key to continue...");

	uint16_t address = 0x0100;

    while (1) {
			set_pin_high(TS_CART_NWE_CONTROLLER, TS_CART_NWE_PIO);
			
			set_pin_low(TS_CART_NRD_CONTROLLER, TS_CART_NRD_PIO);
			set_pin_high(TS_CART_NCS0_CONTROLLER, TS_CART_NCS0_PIO);
			set_pin_low(TS_CART_CLK_CONTROLLER, TS_CART_CLK_PIO);
			
			NOPNOPNOPTOAST
			NOPNOPNOPTOAST
			NOPNOPNOPTOAST
			NOPNOPNOPTOAST
			NOPNOPNOPTOAST
			NOPNOPNOPTOAST
			NOPNOPNOPTOAST
			NOPNOPNOPTOAST
			NOPNOPNOPTOAST
			NOPNOPNOPTOAST
			NOPNOPNOPTOAST
			NOPNOPNOPTOAST
			NOPNOPNOPTOAST
			NOPNOPNOPTOAST
			NOPNOPNOPTOAST
			NOPNOPNOPTOAST
			NOPNOPNOPTOAST
			NOPNOPNOPTOAST
			
			write_to_address(address);
			
			NOPNOPNOPTOAST
			NOPNOPNOPTOAST
			NOPNOPNOPTOAST
			NOPNOPNOPTOAST
			NOPNOPNOPTOAST
			NOPNOPNOPTOAST
			NOPNOPNOPTOAST
			NOPNOPNOPTOAST
			NOPNOPNOPTOAST
			NOPNOPNOPTOAST
			NOPNOPNOPTOAST
			NOPNOPNOPTOAST
			NOPNOPNOPTOAST
			NOPNOPNOPTOAST
			NOPNOPNOPTOAST
			NOPNOPNOPTOAST
			NOPNOPNOPTOAST
			NOPNOPNOPTOAST
			
			volatile uint8_t data = read_from_data();
			
			address++;
			
    }
}
