/*
 * TurboSantaInterface\startup.h - Startup and initialization sequences for the SAM4S ARM controller.
 *
 * Created: 16 Nov 2014 8:07:52 PM
 * Author: Andrew Hennessy (ajb200@case.edu)
 */ 

#ifndef STARTUP_H_
#define STARTUP_H_

#include "sam.h"
#include "board.h"

// MOSCXTST = Main Crystal Oscillator Start-Up Time (Page 529)
// Specifies the number of Slow Clock cycles multiplied by 8 for the Main Crystal Oscillator start-up time.
#define CKGR_MOR_MOSCXTST_VALUE (CKGR_MOR_MOSCXTST(0x8U))

// Initialize the watchdog (In this case disable it)
void initialize_watchdog(void);

// Initialize the main crystal oscillator. This (more or less) follows the "29.14 Programming Sequence" on Page 513 of the SAM4S datasheet
void initialize_xtal(void);

// Initialize all of the clocks for the peripherals used.
void initialize_pmc_clocks(void);

// Initialize all of the PIO pins
void initialize_pio(void);


#endif /* STARTUP_H_ */
