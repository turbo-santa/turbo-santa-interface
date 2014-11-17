/*
 * TurboSantaInterface\startup.c - Startup and initialization sequences for the SAM4S ARM controller.
 *
 * Created: 16 Nov 2014 8:08:51 PM
 * Author: Andrew Hennessy (ajb200@case.edu)
 */ 

// Initialize the watchdog (In this case disable it)
void initialize_watchdog(void) {
	// Disable the Watchdog Timer
	// WDT_MR = Watchdog Timer Mode Register (P322)
	//		WDT_MR_WDDIS = Watchdog Disable (1 = Disables the Watchdog Timer.)
	WDT->WDT_MR = WDT_MR_WDDIS;
}

// Initialize the main crystal oscillator. This (more or less) follows the 29.14 Programming Sequence on Page 513 of the SAM4S datasheet
// With a 16MHz Crystal Oscillator connected to XIN and a 120MHz operational frequency.
void initialize_xtal(void) {	
	// CKGR_MOR = PMC Clock Generator Main Oscillator Register (P528)
	//		CKGR_MOR_KEY_PASSWD = You need a password to write to this register)
	//		CKGR_MOR_MOSCXTST   = Specifies the number of Slow Clock cycles multiplied by 8 for the Main Crystal Oscillator start-up time.
	//		CKGR_MOR_MOSCRCEN   = Main On-Chip RC Oscillator Enable (1 = The Main On-Chip RC Oscillator is enabled.)
	//		CKGR_MOR_MOSCXTEN   = Main Crystal Oscillator Enable (1 = The Main Crystal Oscillator is enabled.)
	PMC->CKGR_MOR =  CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCXTST_VALUE | CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN;
	// We need to delay until the MOSCXTS flag in the PMC_SR register is cleared
	while(!(PMC->PMC_SR & PMC_SR_MOSCXTS)) {}
	// Switch over to the 16MHz external oscillator
	// Values same as before except:
	//		CKGR_MOR_MOSCSEL  = Main Oscillator Selection (1 = The Main Crystal Oscillator is selected)
	PMC->CKGR_MOR =  CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCXTST_VALUE | CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN | CKGR_MOR_MOSCSEL;
	// We need to delay until the MOSCXTS flag in PMC_SR is register is cleared
	while(!(PMC->PMC_SR & PMC_SR_MOSCSELS)) {}
	// PMC_MCKR = PMC Master Clock Register (P533)
	//		PMC_MCKR_CSS_Msk = Master Clock Source Selection
	//		PMC_MCKR_CSS_MAIN_CLK = Main Clock is Selected
	PMC->PMC_MCKR = (PMC->PMC_MCKR & ~(uint32_t)PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_MAIN_CLK;
	// We need to delay until the Main Clock is ready
	while(!(PMC->PMC_SR & PMC_SR_MCKRDY)) {}
	// Initialize PLL A
	// CKGR_PLLAR = PMC Clock Generator PLLA Register
	//		CKGR_PLLAR_ONE       = Bit 29 must always be set to 1 when programming the CKGR_PLLAR register.
	//		CKGR_PLLAR_MULA      = The PLLA Clock Frequency is the PLLA input frequency multiplied by 15
	//		CKGR_PLLAR_PLLACOUNT = Specifies the number of Slow Clock cycles before the LOCKA bit is set in PMC_SR after CKGR_PLLAR is written.
	//		CKGR_PLLAR_DIVA      = PLLA Front_End Divider (2 = Divide by 2 and PLLA is enabled)
	// The net result is that PLLA is configured to run at 120MHz because 16MHz / 2 = 8MHz; 8MHz * 15 = 120MHz
	PMC->CKGR_PLLAR = CKGR_PLLAR_ONE | CKGR_PLLAR_MULA(0xFU) | CKGR_PLLAR_PLLACOUNT(0x3fU) | CKGR_PLLAR_DIVA(0x2U);
	// We need to delay until PLLA is locked onto it's frequency
	while(!(PMC->PMC_SR & PMC_SR_LOCKA)) {}
	// Switch to the Main Clock while configuring PLLA
	//		PMC_MCKR_PRES_CLK_2   = Prescale the selected clock by 2 (e.g. divide it by 2)
	//		PMC_MCKR_CSS_PLLA_CLK = Select the PLLA output as the master clock
	//		PMC_MCKR_CSS_MAIN_CLK = Select the Main Clock as the master clock
	PMC->PMC_MCKR = ((PMC_MCKR_PRES_CLK_2 | PMC_MCKR_CSS_PLLA_CLK) & ~PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_MAIN_CLK;
	// We need to delay until the Main Clock is ready
	while(!(PMC->PMC_SR & PMC_SR_MCKRDY)) {}
	// Switch to PLLA as the Main Clock
	PMC->PMC_MCKR = PMC_MCKR_CSS_MAIN_CLK;
	// We need to delay until the Main Clock is ready
	while(!(PMC->PMC_SR & PMC_SR_MCKRDY)) {}
	// We are now running at 120MHz
}
