/*
 * TurboSantaInterface\startup.c - Startup and initialization sequences for the SAM4S ARM controller.
 *
 * Created: 16 Nov 2014 8:08:51 PM
 * Author: Andrew Hennessy (ajb200@case.edu)
 */ 
#include "startup.h"

// Initialize the watchdog (In this case disable it)
void initialize_watchdog(void) {
	// Disable the Watchdog Timer
	// WDT_MR = Watchdog Timer Mode Register (Page 322)
	//		WDT_MR_WDDIS = Watchdog Disable (1 = Disables the Watchdog Timer.)
	WDT->WDT_MR = WDT_MR_WDDIS;
}

// Initialize the main crystal oscillator. This (more or less) follows the "29.14 Programming Sequence" on Page 513 of the SAM4S datasheet
// With a 16MHz Crystal Oscillator connected to XIN and a 120MHz operational frequency.
void initialize_xtal(void) {	
	// CKGR_MOR = PMC Clock Generator Main Oscillator Register (Page 528)
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
	// PMC_MCKR = PMC Master Clock Register (Page 533)
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

// Initialize all of the clocks for the peripherals used.
void initialize_pmc_clocks(void) {
	// PMC_PCER0: PMC Peripheral Clock Enable Register 0
	// Only peripherals with ID's between 31 and 8 (inclusive) can 
	// be enabled through writing to this register.
	// All of our peripherals used fall into this range, with the exception of USB:
	//	PWM: ID_PWM: 31
	//	ADC: ID_ADC: 29
	//	SPI: ID_SPI: 21
	//	USART1: ID_USART1: 15
	//	USART0: ID_USART0: 14
	//	PIOC: ID_PIOC: 13
	//	PIOB: ID_PIOB: 12
	//	PIOA: ID_PIOA: 11
	//	SMC: ID_SMC: 10
	PMC->PMC_PCER0 = (1 << ID_PWM) | (1 << ID_ADC) | (1 << ID_SPI) | (1 << ID_USART1) | (1 << ID_USART0) | (1 << ID_PIOC) | (1 << ID_PIOB) | (1 << ID_PIOA) | (1 << ID_SMC);
	
	// USB is special. We need to configure PLLB as well: ID_UDP: 34
	// TODO: do USB
}

void initialize_pio(void) {
	// Step 1: Disable the built-in pull up resistors that are enabled by default for all used PIO pins.
	// PIOx_PUDR is in effect regardless of the status of PIO_PDR (i.e. if control is still with PIO or a peripheral)
	PIOA->PIO_PUDR = (TS_PWM_LED1_PIO) | (TS_PWM_LED2_PIO) | (TS_PWM_LED3_PIO) | (TS_FTDI_TX_PIO) | (TS_FTDI_RX_PIO) | (TS_FTDI_CTS_PIO) | (TS_FTDI_RTS_PIO) |
					 (TS_IRDA_RX_PIO) | (TS_IRDA_TX_PIO) | (TS_LINK_SO_PIO) | (TS_LINK_SI_PIO) | (TS_LINK_SC_PIO) | (TS_LINK_SD_PIO) | (TS_LINK_ENA_PIO) |
					 (TS_CART_AOE_PIO) | (TS_CART_ADIR_PIO) | (TS_CART_ADDR14_PIO) | (TS_CART_ADDR15_PIO) | (TS_CART_RST_PIO) | (TS_CART_CLK_PIO);
	PIOB->PIO_PUDR = (TS_USB_DM_PIO) | (TS_USB_DP_PIO) | (TS_PWM_LED0_PIO) | (TS_CART_DOE_PIO) | (TS_CART_DDIR_PIO) | (TS_CART_VIN_PIO);
	PIOC->PIO_PUDR = (TS_CART_DATA0_PIO) | (TS_CART_DATA1_PIO) | (TS_CART_DATA2_PIO) | (TS_CART_DATA3_PIO) | (TS_CART_DATA4_PIO) | (TS_CART_DATA5_PIO) | 
					 (TS_CART_DATA6_PIO) | (TS_CART_DATA7_PIO) | (TS_CART_ADDR0_PIO) | (TS_CART_ADDR1_PIO) | (TS_CART_ADDR2_PIO) | (TS_CART_ADDR3_PIO) | 
					 (TS_CART_ADDR4_PIO) | (TS_CART_ADDR5_PIO) | (TS_CART_ADDR6_PIO) | (TS_CART_ADDR7_PIO) | (TS_CART_ADDR8_PIO) | (TS_CART_ADDR9_PIO) | 
					 (TS_CART_ADDR10_PIO) | (TS_CART_ADDR11_PIO) | (TS_CART_ADDR12_PIO) | (TS_CART_ADDR13_PIO) | (TS_CART_NWE_PIO) | (TS_CART_NRD_PIO) |
					 (TS_CART_NCS0_PIO);
		
	configure_pin_as_pio_output(TS_PWM_LED0_CONTROLLER,  TS_PWM_LED0_PIO, INITIAL_STATE_LOW);
	configure_pin_as_pio_output(TS_PWM_LED1_CONTROLLER,  TS_PWM_LED1_PIO, INITIAL_STATE_LOW);
	configure_pin_as_pio_output(TS_PWM_LED2_CONTROLLER,  TS_PWM_LED2_PIO, INITIAL_STATE_LOW);
	configure_pin_as_pio_output(TS_PWM_LED3_CONTROLLER,  TS_PWM_LED3_PIO, INITIAL_STATE_HIGH);	// Provide heartbeat of initialization progress
	
	
	configure_pin_as_pio_output(TS_LINK_ENA_CONTROLLER,  TS_LINK_ENA_PIO, INITIAL_STATE_LOW);	// Disable the EXT Port voltage shifter until we're ready
	configure_pin_as_pio_output(TS_CART_DOE_CONTROLLER,  TS_CART_DOE_PIO, INITIAL_STATE_HIGH);	// Disable the Cart Port voltage shifter until we're ready
	configure_pin_as_pio_output(TS_CART_AOE_CONTROLLER,  TS_CART_AOE_PIO, INITIAL_STATE_HIGH);	// Disable the Cart Port voltage shifter until we're ready
	
	// The following PIO GPIO pins will have no affect until the above enable pins are activated.
	configure_pin_as_pio_output(TS_CART_DDIR_CONTROLLER,  TS_CART_DDIR_PIO, INITIAL_STATE_HIGH);	// Set the voltage shifter to operate from the A bus to the B bus
	configure_pin_as_pio_output(TS_CART_ADIR_CONTROLLER,  TS_CART_ADIR_PIO, INITIAL_STATE_HIGH);	// Set the voltage shifter to operate from the A bus to the B bus
	configure_pin_as_pio_output(TS_CART_RST_CONTROLLER,   TS_CART_RST_PIO,  INITIAL_STATE_LOW);	// Low = GameBoy Cart is in Reset Mode. High = Active Mode
	configure_pin_as_pio_output(TS_CART_CLK_CONTROLLER,   TS_CART_CLK_PIO,  INITIAL_STATE_LOW);	// An unused pin to provide a clock signal to the GameBoy Cart
	configure_pin_as_pio_output(TS_LINK_SD_CONTROLLER,    TS_LINK_SD_PIO,   INITIAL_STATE_LOW);	// An unused pin to exchange data on the EXT port
	configure_pin_as_pio_input(TS_CART_VIN_CONTROLLER,    TS_CART_VIN_PIO);						// An unused pin to provide us with a fifth channel of audio on the GameBoy Cart.
	
	// Attach the USB pins to the USB interface
	//configure_pin_as_peripheral(TS_USB_DM_CONTROLLER,     TS_USB_DM_PIO,       TS_USB_DM_PERIPHERAL);
	//configure_pin_as_peripheral(TS_USB_DP_CONTROLLER,     TS_USB_DP_PIO,       TS_USB_DP_PERIPHERAL);
	
	// Attach the LEDs to the PWM interface
	//configure_pin_as_peripheral(TS_PWM_LED0_CONTROLLER,   TS_PWM_LED0_PIO,     TS_PWM_LED0_PERIPHERAL);
	//configure_pin_as_peripheral(TS_PWM_LED1_CONTROLLER,   TS_PWM_LED1_PIO,     TS_PWM_LED1_PERIPHERAL);
	//configure_pin_as_peripheral(TS_PWM_LED2_CONTROLLER,   TS_PWM_LED2_PIO,     TS_PWM_LED2_PERIPHERAL);
	//configure_pin_as_peripheral(TS_PWM_LED3_CONTROLLER,   TS_PWM_LED3_PIO,     TS_PWM_LED3_PERIPHERAL);
		
	// Attach the pins connected to the FTDI converter to the USART0 interface
	//configure_pin_as_peripheral(TS_FTDI_TX_CONTROLLER,    TS_FTDI_TX_PIO,      TS_FTDI_TX_PERIPHERAL);
	//configure_pin_as_peripheral(TS_FTDI_RX_CONTROLLER,    TS_FTDI_RX_PIO,      TS_FTDI_RX_PERIPHERAL);
	//configure_pin_as_peripheral(TS_FTDI_CTS_CONTROLLER,   TS_FTDI_CTS_PIO,     TS_FTDI_CTS_PERIPHERAL);
	//configure_pin_as_peripheral(TS_FTDI_RTS_CONTROLLER,   TS_FTDI_RTS_PIO,     TS_FTDI_RTS_PERIPHERAL);
	
	// Attach the pins connected to the IR converter to the USART1 interface
	//configure_pin_as_peripheral(TS_IRDA_RX_CONTROLLER,    TS_IRDA_RX_PIO,      TS_IRDA_RX_PERIPHERAL);
	//configure_pin_as_peripheral(TS_IRDA_TX_CONTROLLER,    TS_IRDA_TX_PIO,      TS_IRDA_TX_PERIPHERAL);
	
	// Attach the pins connected to the EXT/Link port to the SPI interface
	//configure_pin_as_peripheral(TS_LINK_SO_CONTROLLER,    TS_LINK_SO_PIO,	   TS_LINK_SO_PERIPHERAL);
	//configure_pin_as_peripheral(TS_LINK_SI_CONTROLLER,    TS_LINK_SI_PIO,	   TS_LINK_SI_PERIPHERAL);
	//configure_pin_as_peripheral(TS_LINK_SC_CONTROLLER,    TS_LINK_SC_PIO,	   TS_LINK_SC_PERIPHERAL);
	
	// Attach the pins connected to the Game Cart's Data Bus to the SMC interface
	//configure_pin_as_peripheral(TS_CART_DATA0_CONTROLLER, TS_CART_DATA0_PIO,   TS_CART_DATA0_PERIPHERAL);
	//configure_pin_as_peripheral(TS_CART_DATA1_CONTROLLER, TS_CART_DATA1_PIO,   TS_CART_DATA1_PERIPHERAL);
	//configure_pin_as_peripheral(TS_CART_DATA2_CONTROLLER, TS_CART_DATA2_PIO,   TS_CART_DATA2_PERIPHERAL);
	//configure_pin_as_peripheral(TS_CART_DATA3_CONTROLLER, TS_CART_DATA3_PIO,   TS_CART_DATA3_PERIPHERAL);
	//configure_pin_as_peripheral(TS_CART_DATA4_CONTROLLER, TS_CART_DATA4_PIO,   TS_CART_DATA4_PERIPHERAL);
	//configure_pin_as_peripheral(TS_CART_DATA5_CONTROLLER, TS_CART_DATA5_PIO,   TS_CART_DATA5_PERIPHERAL);
	//configure_pin_as_peripheral(TS_CART_DATA6_CONTROLLER, TS_CART_DATA6_PIO,   TS_CART_DATA6_PERIPHERAL);
	//configure_pin_as_peripheral(TS_CART_DATA7_CONTROLLER, TS_CART_DATA7_PIO,   TS_CART_DATA7_PERIPHERAL);
	
	// Attach the pins connected to the Game Cart's Address Bus to the SMC interface
	//configure_pin_as_peripheral(TS_CART_ADDR0_CONTROLLER,  TS_CART_ADDR0_PIO,  TS_CART_ADDR0_PERIPHERAL);
	//configure_pin_as_peripheral(TS_CART_ADDR1_CONTROLLER,  TS_CART_ADDR1_PIO,  TS_CART_ADDR1_PERIPHERAL);
	//configure_pin_as_peripheral(TS_CART_ADDR2_CONTROLLER,  TS_CART_ADDR2_PIO,  TS_CART_ADDR2_PERIPHERAL);
	//configure_pin_as_peripheral(TS_CART_ADDR3_CONTROLLER,  TS_CART_ADDR3_PIO,  TS_CART_ADDR3_PERIPHERAL);
	//configure_pin_as_peripheral(TS_CART_ADDR4_CONTROLLER,  TS_CART_ADDR4_PIO,  TS_CART_ADDR4_PERIPHERAL);
	//configure_pin_as_peripheral(TS_CART_ADDR5_CONTROLLER,  TS_CART_ADDR5_PIO,  TS_CART_ADDR5_PERIPHERAL);
	//configure_pin_as_peripheral(TS_CART_ADDR6_CONTROLLER,  TS_CART_ADDR6_PIO,  TS_CART_ADDR6_PERIPHERAL);
	//configure_pin_as_peripheral(TS_CART_ADDR7_CONTROLLER,  TS_CART_ADDR7_PIO,  TS_CART_ADDR7_PERIPHERAL);
	//configure_pin_as_peripheral(TS_CART_ADDR8_CONTROLLER,  TS_CART_ADDR8_PIO,  TS_CART_ADDR8_PERIPHERAL);
	//configure_pin_as_peripheral(TS_CART_ADDR9_CONTROLLER,  TS_CART_ADDR9_PIO,  TS_CART_ADDR9_PERIPHERAL);
	//configure_pin_as_peripheral(TS_CART_ADDR10_CONTROLLER, TS_CART_ADDR10_PIO, TS_CART_ADDR10_PERIPHERAL);
	//configure_pin_as_peripheral(TS_CART_ADDR11_CONTROLLER, TS_CART_ADDR11_PIO, TS_CART_ADDR11_PERIPHERAL);
	//configure_pin_as_peripheral(TS_CART_ADDR12_CONTROLLER, TS_CART_ADDR12_PIO, TS_CART_ADDR12_PERIPHERAL);
	//configure_pin_as_peripheral(TS_CART_ADDR13_CONTROLLER, TS_CART_ADDR13_PIO, TS_CART_ADDR13_PERIPHERAL);
	//configure_pin_as_peripheral(TS_CART_ADDR14_CONTROLLER, TS_CART_ADDR14_PIO, TS_CART_ADDR14_PERIPHERAL);
	//configure_pin_as_peripheral(TS_CART_ADDR15_CONTROLLER, TS_CART_ADDR15_PIO, TS_CART_ADDR15_PERIPHERAL);
	
	// Attach the pins connected to the Game Cart's Control Bus to the SMC interface
	//configure_pin_as_peripheral(TS_CART_NWE_CONTROLLER,    TS_CART_NWE_PIO,    TS_CART_NWE_PERIPHERAL);
	//configure_pin_as_peripheral(TS_CART_NRD_CONTROLLER,    TS_CART_NRD_PIO,    TS_CART_NRD_PERIPHERAL);
	//configure_pin_as_peripheral(TS_CART_NCS0_CONTROLLER,   TS_CART_NCS0_PIO,   TS_CART_NCS0_PERIPHERAL);
}
