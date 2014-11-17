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

    while (1) 
    {
    }
}
