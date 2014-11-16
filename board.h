/*
 * board.h - Created 16 November 2014
 *
 * Pin Definitions on the Atmel SAM4SA16C for TurboSanta Interface
 * Note: All pages numbers are referenced against: http://www.atmel.com/images/atmel-11100-32-bit-cortex-m4-microcontroller_datasheet.pdf
 *
 * The following peripherals are used in this project:
 *	- USB (Called the USB Data Port)
 *	- PWM
 *	- USART0 (FTDI)
 *	- USART1 (IR) - This is somewhat questionable however -- the interface might end up being bitbanged.
 *	- SPI (Link)  - This is the most questionable however -- the interface might end up being bitbanged.
 *	- SMC (Cart)  - This is the least questionable however -- the interface might end up being bitbanged.
 *  - ADC (Cart)  - This functionality is so rare that it might not even be implemented.
 *  - PIO (GPIO)
 */ 

#ifndef BOARD_H_
#define BOARD_H_

// USB Pins:
//	Data- on PB10 is DDM on UDP (System Peripheral)
//	Data+ on PB11 is DDP on UDP (System Peripheral)
//	Note: UDP is attached to Interrupt UDP which has an ID of 34
//	Note: There exists a TI TUSB2036 Hub upstream, creating a compound device between this and the FTDI.
// TODO: Enable MCK and UDPCK in the PMC

// User Facing LED Pins:
//	LED0 on PB14 is PWMH3 on PWM which is attached to Peripheral B
//	LED1 on PA0  is PWMH0 on PWM which is attached to Peripheral A
//	LED2 on PA1  is PWMH1 on PWM which is attached to Peripheral A
//	LED3 on PA2  is PWMH2 on PWM which is attached to Peripheral A
//	Note: PWM is attached to Interrupt PWM which has an ID of 31
// TODO: Enable the PWM Clock in the PMC

// FTDI UART-USB Bridge Pins
// Port is configured as 115200 8-E-1
// Note: RX/TX terminology is w/r/t the FTDI chip
//	FTDI TX  on PA5 is RXD0 on USART0 which is attached to Peripheral A
//	FTDI RX  on PA6 is TXD0 on USART0 which is attached to Peripheral A
//	FTDI CTS on PA7 is RTS0 on USART0 which is attached to Peripheral A
//	FTDI RTS on PA8 is CTS0 on USART0 which is attached to Peripheral A
//	Note: USART0 is attached to Interrupt USART0 which has an ID of 14
//	Note: There exists a TI TUSB2036 Hub upstream, creating a compound device between this and the UDP.
// TODO: Enable the USART Clock in the PMC (The clock is shared with USART1)

// CGB IR Port Pins
// Note: RX/TX terminology is w/r/t the IR transceiver, which is backwards of convention (AKA there is no convention!)
//		e.g. The TX pin on the IrDA module is the input, while the RX pin on the IrDA module is the output
//			 The SAM4S expects the TX pin to be the output, while the RX pin should be an output.
//	IR RX on PA21 is RXD1 on USART1 which is attached to Peripheral A
//	IR TX on PA22 is TXD1 on USART1 which is attached to Peripheral A
//	Note: USART1 is attached to Interrupt USART1 which has an ID of 15
// TODO: Enable the USART Clock in the PMC (The clock is shared with USART0)

// CBG Link Port Pins
//	Serial Out   on PA12 is MISO on SPI which is attached to Peripheral A
//	Serial In    on PA13 is MOSI on SPI which is attached to Peripheral A
//	Serial Clock on PA14 is SPCK on SPI which is attached to Peripheral A
//	Serial Data  on PA26 is GPIO 
//  Note: There is a board jumper to control the routing of MISO, MOSI, SO and SI. This is after the level shifter.
//	Note: SPI is attached to Interrupt SPI which has an ID of 21
// TODO: Enable the SPI Clock in the PMC

// CGB Link Port Level Shifter Control Pins
//	Port Enable  on PA15 is GPIO

// CGB Cartridge Port Level Shifter Control Pins
//	Address Output Enable on PA20 is GPIO
//	Address Direction	  on PA16 is GPIO
//	Data Output Enable	  on PB8  is GPIO
//	Data Direction		  on PB0  is GPIO

// CGB Cartridge Data Pins
//	D0 on PC0 is D0 on PIO/SMC which is attached to Peripheral A
//	D1 on PC1 is D1 on PIO/SMC which is attached to Peripheral A
//	D2 on PC2 is D2 on PIO/SMC which is attached to Peripheral A
//	D3 on PC3 is D3 on PIO/SMC which is attached to Peripheral A
//	D4 on PC4 is D4 on PIO/SMC which is attached to Peripheral A
//	D5 on PC5 is D5 on PIO/SMC which is attached to Peripheral A
//	D6 on PC6 is D6 on PIO/SMC which is attached to Peripheral A
//	D7 on PC7 is D7 on PIO/SMC which is attached to Peripheral A
//  Note: The SMC lines are multiplexed with the equivalent PIO pins
// TODO: Enable the PIO Clock in the PMC
// TODO: Enable the SMC Clock in the PMC

// CGB Cartridge Address Pins
//	A0  on PC18 is A0  on PIO/SMC which is attached to Peripheral A 
//	A1  on PC19 is A1  on PIO/SMC which is attached to Peripheral A 
//	A2  on PC20 is A2  on PIO/SMC which is attached to Peripheral A 
//	A3  on PC21 is A3  on PIO/SMC which is attached to Peripheral A 
//	A4  on PC22 is A4  on PIO/SMC which is attached to Peripheral A 
//	A5  on PC23 is A5  on PIO/SMC which is attached to Peripheral A 
//	A6  on PC24 is A6  on PIO/SMC which is attached to Peripheral A 
//	A7  on PC25 is A7  on PIO/SMC which is attached to Peripheral A 
//	A8  on PC26 is A8  on PIO/SMC which is attached to Peripheral A 
//	A9  on PC27 is A9  on PIO/SMC which is attached to Peripheral A 
//	A10 on PC28 is A10 on PIO/SMC which is attached to Peripheral A 
//	A11 on PC29 is A11 on PIO/SMC which is attached to Peripheral A 
//	A12 on PC30 is A12 on PIO/SMC which is attached to Peripheral A 
//	A13 on PC31 is A13 on PIO/SMC which is attached to Peripheral A 
//	A14 on PA18 is A14 on PIO/SMC which is attached to Peripheral C 
//	A15 on PA19 is A15 on PIO/SMC which is attached to Peripheral C 

// CGB Cartridge Control Pins
//	WR  on PC8  is NWE  on PIO/SMC which is attached to Peripheral A
//	!RD on PC11 is NRD  on PIO/SMC which is attached to Peripheral A
//	!CS on PC14 is NCS0 on PIO/SMC which is attached to Peripheral A
//	VIN on PB3  is AD7  on ADC (System Peripheral)
//	RST	on PA9  is GPIO
//	CLK on PA27 is GPIO
//	Note: ADC is attached to Interrupt ADC which has an ID of 29
// TODO: Enable the ADC Clock in the PMC


#endif /* BOARD_H_ */