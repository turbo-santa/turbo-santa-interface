/*
 * TurboSantaInterface.c
 *
 * Created: 16 Nov 2014 3:55:30 PM
 *  Author: Andrew
 */ 

#include "sam.h"

/**
 * \brief Application entry point.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	// Disable the WatchDog Timer
	// Write a 1 to the WDDIS bit in the WDT_MR register (P322)
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	
	
    while (1) 
    {
        //TODO:: Please write your application code 
    }
}
