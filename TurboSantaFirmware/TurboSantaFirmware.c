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
	
	usart0_ftdi_putstring("Welcome to the TurboSanta Interface GameBoy (Color) ROM Dumper!\r\n");
	usart0_ftdi_putstring("\r\n");
	usart0_ftdi_putstring("Press any key to continue...");

    while (1) 
    {
			usart0_ftdi_putstring("Welcome to the TurboSanta Interface GameBoy (Color) ROM Dumper!\r\n");
			usart0_ftdi_putstring("\r\n");
			usart0_ftdi_putstring("Press any key to continue...\r\n");
    }
}
