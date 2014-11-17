/*
 * TurboSantaInterface\startup.h - Startup and initialization sequences for the SAM4S ARM controller.
 *
 * Created: 16 Nov 2014 8:07:52 PM
 * Author: Andrew Hennessy (ajb200@case.edu)
 */ 

#ifndef STARTUP_H_
#define STARTUP_H_

// MOSCXTST = Main Crystal Oscillator Start-Up Time (P529)
// Specifies the number of Slow Clock cycles multiplied by 8 for the Main Crystal Oscillator start-up time.
#define CKGR_MOR_MOSCXTST_VALUE (CKGR_MOR_MOSCXTST(0x8U))

// Initialize the watchdog (In this case disable it)
void initialize_watchdog(void);

// Initialize the main crystal oscillator. This follows the 29.14 Programming Sequence on Page 513 of the SAM4S datasheet
void initialize_xtal(void);

#endif /* STARTUP_H_ */
