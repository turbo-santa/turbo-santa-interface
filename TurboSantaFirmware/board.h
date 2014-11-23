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
#define TS_USB_DM_PIO		(1u << 10)
//	Data+ on PB11 is DDP on UDP (System Peripheral)
#define TS_USB_DP_PIO		(1u << 11)
//	Note: UDP is attached to Interrupt UDP which has an ID of 34
//	Note: There exists a TI TUSB2036 Hub upstream, creating a compound device between this and the FTDI.

// User Facing LED Pins:
//	LED0 on PB14 is PWMH3 on PWM which is attached to Peripheral B
#define TS_PWM_LED0_PIO		(1u << 14)
//	LED1 on PA0  is PWMH0 on PWM which is attached to Peripheral A
#define TS_PWM_LED1_PIO		(1u << 0)
//	LED2 on PA1  is PWMH1 on PWM which is attached to Peripheral A
#define TS_PWM_LED2_PIO		(1u << 1)
//	LED3 on PA2  is PWMH2 on PWM which is attached to Peripheral A
#define TS_PWM_LED3_PIO		(1u << 2)
//	Note: PWM is attached to Interrupt PWM which has an ID of 31

// FTDI UART-USB Bridge Pins
// Port is configured as 115200 8-E-1
// Note: RX/TX terminology is w/r/t the FTDI chip
//	FTDI TX  on PA5 is RXD0 on USART0 which is attached to Peripheral A
#define TS_FTDI_TX_PIO		(1u << 5)
//	FTDI RX  on PA6 is TXD0 on USART0 which is attached to Peripheral A
#define TS_FTDI_RX_PIO		(1u << 6)
//	FTDI CTS on PA7 is RTS0 on USART0 which is attached to Peripheral A
#define TS_FTDI_CTS_PIO		(1u << 7)
//	FTDI RTS on PA8 is CTS0 on USART0 which is attached to Peripheral A
#define TS_FTDI_RTS_PIO		(1u << 8)
//	Note: USART0 is attached to Interrupt USART0 which has an ID of 14
//	Note: There exists a TI TUSB2036 Hub upstream, creating a compound device between this and the UDP.

// CGB IR Port Pins
// Note: RX/TX terminology is w/r/t the IR transceiver, which is backwards of convention (AKA there is no convention!)
//		e.g. The TX pin on the IrDA module is the input, while the RX pin on the IrDA module is the output
//			 The SAM4S expects the TX pin to be the output, while the RX pin should be an output.
//	IR RX on PA21 is RXD1 on USART1 which is attached to Peripheral A
#define TS_IRDA_RX_PIO		(1u << 21)
//	IR TX on PA22 is TXD1 on USART1 which is attached to Peripheral A
#define TS_IRDA_TX_PIO		(1u << 22)
//	Note: USART1 is attached to Interrupt USART1 which has an ID of 15

// CBG Link Port Pins
//	Serial Out   on PA12 is MISO on SPI which is attached to Peripheral A
#define TS_LINK_SO_PIO		(1u << 12)
//	Serial In    on PA13 is MOSI on SPI which is attached to Peripheral A
#define TS_LINK_SI_PIO		(1u << 13)
//	Serial Clock on PA14 is SPCK on SPI which is attached to Peripheral A
#define TS_LINK_SC_PIO		(1u << 14)
//	Serial Data  on PA26 is GPIO 
#define TS_LINK_SD_PIO		(1u << 26)
//  Note: There is a board jumper to control the routing of MISO, MOSI, SO and SI. This is after the level shifter.
//	Note: SPI is attached to Interrupt SPI which has an ID of 21

// CGB Link Port Level Shifter Control Pins
//	Port Enable  on PA15 is GPIO
#define TS_LINK_ENA_PIO		(1u << 15)

// CGB Cartridge Port Level Shifter Control Pins
//	Address Output Enable on PA20 is GPIO
#define TS_CART_AOE_PIO		(1u << 20)
//	Address Direction	  on PA16 is GPIO
#define TS_CART_ADIR_PIO	(1u << 20)
//	Data Output Enable	  on PB8  is GPIO
#define TS_CART_DOE_PIO		(1u << 8)
//	Data Direction		  on PB0  is GPIO
#define TS_CART_DDIR_PIO	(1u << 0)

// CGB Cartridge Data Pins
//	D0 on PC0 is D0 on PIO/SMC which is attached to Peripheral A
#define TS_CART_DATA0_PIO	(1u << 0)
//	D1 on PC1 is D1 on PIO/SMC which is attached to Peripheral A
#define TS_CART_DATA1_PIO	(1u << 1)
//	D2 on PC2 is D2 on PIO/SMC which is attached to Peripheral A
#define TS_CART_DATA2_PIO	(1u << 2)
//	D3 on PC3 is D3 on PIO/SMC which is attached to Peripheral A
#define TS_CART_DATA3_PIO	(1u << 3)
//	D4 on PC4 is D4 on PIO/SMC which is attached to Peripheral A
#define TS_CART_DATA4_PIO	(1u << 4)
//	D5 on PC5 is D5 on PIO/SMC which is attached to Peripheral A
#define TS_CART_DATA5_PIO	(1u << 5)
//	D6 on PC6 is D6 on PIO/SMC which is attached to Peripheral A
#define TS_CART_DATA6_PIO	(1u << 6)
//	D7 on PC7 is D7 on PIO/SMC which is attached to Peripheral A
#define TS_CART_DATA7_PIO	(1u << 7)
//  Note: The SMC lines are multiplexed with the equivalent PIO pins

// CGB Cartridge Address Pins
//	A0  on PC18 is A0  on PIO/SMC which is attached to Peripheral A
#define TS_CART_ADDR0_PIO	(1u << 0) 
//	A1  on PC19 is A1  on PIO/SMC which is attached to Peripheral A 
#define TS_CART_ADDR1_PIO	(1u << 1)
//	A2  on PC20 is A2  on PIO/SMC which is attached to Peripheral A 
#define TS_CART_ADDR2_PIO	(1u << 2)
//	A3  on PC21 is A3  on PIO/SMC which is attached to Peripheral A 
#define TS_CART_ADDR3_PIO	(1u << 3)
//	A4  on PC22 is A4  on PIO/SMC which is attached to Peripheral A 
#define TS_CART_ADDR4_PIO	(1u << 4)
//	A5  on PC23 is A5  on PIO/SMC which is attached to Peripheral A 
#define TS_CART_ADDR5_PIO	(1u << 5)
//	A6  on PC24 is A6  on PIO/SMC which is attached to Peripheral A 
#define TS_CART_ADDR6_PIO	(1u << 6)
//	A7  on PC25 is A7  on PIO/SMC which is attached to Peripheral A 
#define TS_CART_ADDR7_PIO	(1u << 7)
//	A8  on PC26 is A8  on PIO/SMC which is attached to Peripheral A 
#define TS_CART_ADDR8_PIO	(1u << 8)
//	A9  on PC27 is A9  on PIO/SMC which is attached to Peripheral A 
#define TS_CART_ADDR9_PIO	(1u << 9)
//	A10 on PC28 is A10 on PIO/SMC which is attached to Peripheral A 
#define TS_CART_ADDR10_PIO	(1u << 10)
//	A11 on PC29 is A11 on PIO/SMC which is attached to Peripheral A 
#define TS_CART_ADDR11_PIO	(1u << 11)
//	A12 on PC30 is A12 on PIO/SMC which is attached to Peripheral A 
#define TS_CART_ADDR12_PIO	(1u << 12)
//	A13 on PC31 is A13 on PIO/SMC which is attached to Peripheral A 
#define TS_CART_ADDR13_PIO	(1u << 13)
//	A14 on PA18 is A14 on PIO/SMC which is attached to Peripheral C 
#define TS_CART_ADDR14_PIO	(1u << 14)
//	A15 on PA19 is A15 on PIO/SMC which is attached to Peripheral C 
#define TS_CART_ADDR14_PIO	(1u << 15)

// CGB Cartridge Control Pins
//	WR  on PC8  is NWE  on PIO/SMC which is attached to Peripheral A
#define TS_CART_NWE_PIO		(1u << 8)
//	!RD on PC11 is NRD  on PIO/SMC which is attached to Peripheral A
#define TS_CART_NRD_PIO		(1u << 11)
//	!CS on PC14 is NCS0 on PIO/SMC which is attached to Peripheral A
#define TS_CART_NCS0_PIO	(1u << 14)
//	VIN on PB3  is AD7  on ADC (System Peripheral)
#define TS_CART_VIN_PIO		(1u << 3)
//	RST	on PA9  is GPIO
#define TS_CART_RST_PIO		(1u << 9)
//	CLK on PA27 is GPIO
#define TS_CART_CLK_PIO		(1u << 27)
//	Note: ADC is attached to Interrupt ADC which has an ID of 29

#endif /* BOARD_H_ */