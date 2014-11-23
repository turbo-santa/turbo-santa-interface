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

#include "sam.h"

#define PERIPHERAL_A	 0
#define PERIPHERAL_B	 1
#define PERIPHERAL_C	 2
#define PERIPHERAL_D	 3
#define PERIPHERAL_SYS	 4
#define PERIPHERAL_GPIO	 5
#define PERIPHERAL_EXTRA 6

// USB Pins:
//	Data- on PB10 is DDM on UDP (System Peripheral)
#define TS_USB_DM_CONTROLLER	PIOB
#define TS_USB_DM_PERIPHERAL	PERIPHERAL_SYS
#define TS_USB_DM_PIO_NUM		10
#define TS_USB_DM_PIO			(1u << TS_USB_DM_PIO_NUM)
//	Data+ on PB11 is DDP on UDP (System Peripheral)
#define TS_USB_DP_CONTROLLER	PIOB
#define TS_USB_DP_PERIPHERAL	PERIPHERAL_SYS
#define TS_USB_DP_PIO_NUM		11
#define TS_USB_DP_PIO			(1u << TS_USB_DP_PIO_NUM)
//	Note: UDP is attached to Interrupt UDP which has an ID of 34
//	Note: There exists a TI TUSB2036 Hub upstream, creating a compound device between this and the FTDI.

// User Facing LED Pins:
//	LED0 on PB14 is PWMH3 on PWM which is attached to Peripheral B
#define TS_PWM_LED0_CONTROLLER		PIOB
#define TS_PWM_LED0_PERIPHERAL		PERIPHERAL_B
#define TS_PWM_LED0_PIO_NUM			14
#define TS_PWM_LED0_PIO				(1u << TS_PWM_LED0_PIO_NUM)
//	LED1 on PA0  is PWMH0 on PWM which is attached to Peripheral A
#define TS_PWM_LED1_CONTROLLER		PIOA
#define TS_PWM_LED1_PERIPHERAL		PERIPHERAL_A
#define TS_PWM_LED1_PIO_NUM			0
#define TS_PWM_LED1_PIO				(1u << TS_PWM_LED1_PIO_NUM)
//	LED2 on PA1  is PWMH1 on PWM which is attached to Peripheral A
#define TS_PWM_LED2_CONTROLLER		PIOA
#define TS_PWM_LED2_PERIPHERAL		PERIPHERAL_A
#define TS_PWM_LED2_PIO_NUM			1
#define TS_PWM_LED2_PIO				(1u << TS_PWM_LED2_PIO_NUM)
//	LED3 on PA2  is PWMH2 on PWM which is attached to Peripheral A
#define TS_PWM_LED3_CONTROLLER		PIOA
#define TS_PWM_LED3_PERIPHERAL		PERIPHERAL_A
#define TS_PWM_LED3_PIO_NUM			2
#define TS_PWM_LED3_PIO				(1u << TS_PWM_LED2_PIO_NUM)
//	Note: PWM is attached to Interrupt PWM which has an ID of 31

// FTDI UART-USB Bridge Pins
// Port is configured as 115200 8-E-1
// Note: RX/TX terminology is w/r/t the FTDI chip
//	FTDI TX  on PA5 is RXD0 on USART0 which is attached to Peripheral A
#define TS_FTDI_TX_CONTROLLER		PIOA
#define TS_FTDI_TX_PERIPHERAL		PERIPHERAL_A
#define TS_FTDI_TX_PIO_NUM			5
#define TS_FTDI_TX_PIO				(1u << TS_FTDI_TX_PIO_NUM)
//	FTDI RX  on PA6 is TXD0 on USART0 which is attached to Peripheral A
#define TS_FTDI_RX_CONTROLLER		PIOA
#define TS_FTDI_RX_PERIPHERAL		PERIPHERAL_A
#define TS_FTDI_RX_PIO_NUM			6
#define TS_FTDI_RX_PIO				(1u << TS_FTDI_RX_PIO_NUM)
//	FTDI CTS on PA7 is RTS0 on USART0 which is attached to Peripheral A
#define TS_FTDI_CTS_CONTROLLER		PIOA
#define TS_FTDI_CTS_PERIPHERAL		PERIPHERAL_A
#define TS_FTDI_CTS_PIO_NUM			7
#define TS_FTDI_CTS_PIO				(1u << TS_FTDI_CTS_PIO_NUM)
//	FTDI RTS on PA8 is CTS0 on USART0 which is attached to Peripheral A
#define TS_FTDI_RTS_CONTROLLER		PIOA
#define TS_FTDI_RTS_PERIPHERAL		PERIPHERAL_A
#define TS_FTDI_RTS_PIO_NUM			8
#define TS_FTDI_RTS_PIO				(1u << TS_FTDI_RTS_PIO_NUM)
//	Note: USART0 is attached to Interrupt USART0 which has an ID of 14
//	Note: There exists a TI TUSB2036 Hub upstream, creating a compound device between this and the UDP.

// CGB IR Port Pins
// Note: RX/TX terminology is w/r/t the IR transceiver, which is backwards of convention (AKA there is no convention!)
//		e.g. The TX pin on the IrDA module is the input, while the RX pin on the IrDA module is the output
//			 The SAM4S expects the TX pin to be the output, while the RX pin should be an output.
//	IR RX on PA21 is RXD1 on USART1 which is attached to Peripheral A
#define TS_IRDA_RX_CONTROLLER		PIOA
#define TS_IRDA_RX_PERIPHERAL		PERIPHERAL_A
#define TS_IRDA_RX_PIO_NUM			21
#define TS_IRDA_RX_PIO				(1u << TS_IRDA_RX_PIO_NUM)
//	IR TX on PA22 is TXD1 on USART1 which is attached to Peripheral A
#define TS_IRDA_TX_CONTROLLER		PIOA
#define TS_IRDA_TX_PERIPHERAL		PERIPHERAL_A
#define TS_IRDA_TX_PIO_NUM			22
#define TS_IRDA_TX_PIO				(1u << TS_IRDA_TX_PIO_NUM)
//	Note: USART1 is attached to Interrupt USART1 which has an ID of 15

// CBG Link Port Pins
//	Serial Out   on PA12 is MISO on SPI which is attached to Peripheral A
#define TS_LINK_SO_CONTROLLER		PIOA
#define TS_LINK_SO_PERIPHERAL		PERIPHERAL_A
#define TS_LINK_SO_PIO_NUM			12
#define TS_LINK_SO_PIO				(1u << TS_LINK_SO_PIO_NUM)
//	Serial In    on PA13 is MOSI on SPI which is attached to Peripheral A
#define TS_LINK_SI_CONTROLLER		PIOA
#define TS_LINK_SI_PERIPHERAL		PERIPHERAL_A
#define TS_LINK_SI_PIO_NUM			13
#define TS_LINK_SI_PIO				(1u << TS_LINK_SI_PIO_NUM)
//	Serial Clock on PA14 is SPCK on SPI which is attached to Peripheral A
#define TS_LINK_SC_CONTROLLER		PIOA
#define TS_LINK_SC_PERIPHERAL		PERIPHERAL_A
#define TS_LINK_SC_PIO_NUM			14
#define TS_LINK_SC_PIO				(1u << TS_LINK_SC_PIO_NUM)
//	Serial Data  on PA26 is GPIO 
#define TS_LINK_SD_CONTROLLER		PIOA
#define TS_LINK_SD_PERIPHERAL		PERIPHERAL_GPIO
#define TS_LINK_SD_PIO_NUM			26
#define TS_LINK_SD_PIO				(1u << TS_LINK_SD_PIO_NUM)
//  Note: There is a board jumper to control the routing of MISO, MOSI, SO and SI. This is after the level shifter.
//	Note: SPI is attached to Interrupt SPI which has an ID of 21

// CGB Link Port Level Shifter Control Pins
//	Port Enable  on PA15 is GPIO
#define TS_LINK_ENA_CONTROLLER		PIOA
#define TS_LINK_ENA_PERIPHERAL		PERIPHERAL_GPIO
#define TS_LINK_ENA_PIO_NUM			15
#define TS_LINK_ENA_PIO				(1u << TS_LINK_ENA_PIO_NUM)

// CGB Cartridge Port Level Shifter Control Pins
//	Address Output Enable on PA20 is GPIO
#define TS_CART_AOE_CONTROLLER		PIOA
#define TS_CART_AOE_PERIPHERAL		PERIPHERAL_GPIO
#define TS_CART_AOE_PIO_NUM			20
#define TS_CART_AOE_PIO				(1u << TS_CART_AOE_PIO_NUM)
//	Address Direction	  on PA16 is GPIO
#define TS_CART_ADIR_CONTROLLER		PIOA
#define TS_CART_ADIR_PERIPHERAL		PERIPHERAL_GPIO
#define TS_CART_ADIR_PIO_NUM		16
#define TS_CART_ADIR_PIO			(1u << TS_CART_ADIR_PIO_NUM)
//	Data Output Enable	  on PB8  is GPIO
#define TS_CART_DOE_CONTROLLER		PIOB
#define TS_CART_DOE_PERIPHERAL		PERIPHERAL_GPIO
#define TS_CART_DOE_PIO_NUM			8
#define TS_CART_DOE_PIO				(1u << TS_CART_DOE_PIO_NUM)
//	Data Direction		  on PB0  is GPIO
#define TS_CART_DDIR_CONTROLLER		PIOA
#define TS_CART_DDIR_PERIPHERAL		PERIPHERAL_GPIO
#define TS_CART_DDIR_PIO_NUM		0
#define TS_CART_DDIR_PIO			(1u << TS_CART_DDIR_PIO_NUM)

// CGB Cartridge Data Pins
//	D0 on PC0 is D0 on PIO/SMC which is attached to Peripheral A
#define TS_CART_DATA0_CONTROLLER	PIOC
#define TS_CART_DATA0_PERIPHERAL	PERIPHERAL_A
#define TS_CART_DATA0_PIO_NUM		0
#define TS_CART_DATA0_PIO			(1u << TS_CART_DATA0_PIO_NUM)
//	D1 on PC1 is D1 on PIO/SMC which is attached to Peripheral A
#define TS_CART_DATA1_CONTROLLER	PIOC
#define TS_CART_DATA1_PERIPHERAL	PERIPHERAL_A
#define TS_CART_DATA1_PIO_NUM		1
#define TS_CART_DATA1_PIO			(1u << TS_CART_DATA1_PIO_NUM)
//	D2 on PC2 is D2 on PIO/SMC which is attached to Peripheral A
#define TS_CART_DATA2_CONTROLLER	PIOC
#define TS_CART_DATA2_PERIPHERAL	PERIPHERAL_A
#define TS_CART_DATA2_PIO_NUM		2
#define TS_CART_DATA2_PIO			(1u << TS_CART_DATA2_PIO_NUM)
//	D3 on PC3 is D3 on PIO/SMC which is attached to Peripheral A
#define TS_CART_DATA3_CONTROLLER	PIOC
#define TS_CART_DATA3_PERIPHERAL	PERIPHERAL_A
#define TS_CART_DATA3_PIO_NUM		3
#define TS_CART_DATA3_PIO			(1u << TS_CART_DATA3_PIO_NUM)
//	D4 on PC4 is D4 on PIO/SMC which is attached to Peripheral A
#define TS_CART_DATA4_CONTROLLER	PIOC
#define TS_CART_DATA4_PERIPHERAL	PERIPHERAL_A
#define TS_CART_DATA4_PIO_NUM		4
#define TS_CART_DATA4_PIO			(1u << TS_CART_DATA4_PIO_NUM)
//	D5 on PC5 is D5 on PIO/SMC which is attached to Peripheral A
#define TS_CART_DATA5_CONTROLLER	PIOC
#define TS_CART_DATA5_PERIPHERAL	PERIPHERAL_A
#define TS_CART_DATA5_PIO_NUM		5
#define TS_CART_DATA5_PIO			(1u << TS_CART_DATA5_PIO_NUM)
//	D6 on PC6 is D6 on PIO/SMC which is attached to Peripheral A
#define TS_CART_DATA6_CONTROLLER	PIOC
#define TS_CART_DATA6_PERIPHERAL	PERIPHERAL_A
#define TS_CART_DATA6_PIO_NUM		6
#define TS_CART_DATA6_PIO			(1u << TS_CART_DATA6_PIO_NUM)
//	D7 on PC7 is D7 on PIO/SMC which is attached to Peripheral A
#define TS_CART_DATA7_CONTROLLER	PIOC
#define TS_CART_DATA7_PERIPHERAL	PERIPHERAL_A
#define TS_CART_DATA7_PIO_NUM		7
#define TS_CART_DATA7_PIO			(1u << TS_CART_DATA7_PIO_NUM)
//  Note: The SMC lines are multiplexed with the equivalent PIO pins

// CGB Cartridge Address Pins
//	A0  on PC18 is A0  on PIO/SMC which is attached to Peripheral A
#define TS_CART_ADDR0_CONTROLLER    PIOC
#define TS_CART_ADDR0_PERIPHERAL    PERIPHERAL_A
#define TS_CART_ADDR0_PIO_NUM       18
#define TS_CART_ADDR0_PIO           (1u << TS_CART_ADDR0_PIO_NUM)
//	A1  on PC19 is A1  on PIO/SMC which is attached to Peripheral A 
#define TS_CART_ADDR1_CONTROLLER    PIOC
#define TS_CART_ADDR1_PERIPHERAL    PERIPHERAL_A
#define TS_CART_ADDR1_PIO_NUM       19
#define TS_CART_ADDR1_PIO           (1u << TS_CART_ADDR1_PIO_NUM)
//	A2  on PC20 is A2  on PIO/SMC which is attached to Peripheral A 
#define TS_CART_ADDR2_CONTROLLER    PIOC
#define TS_CART_ADDR2_PERIPHERAL    PERIPHERAL_A
#define TS_CART_ADDR2_PIO_NUM       20
#define TS_CART_ADDR2_PIO           (1u << TS_CART_ADDR2_PIO_NUM)
//	A3  on PC21 is A3  on PIO/SMC which is attached to Peripheral A 
#define TS_CART_ADDR3_CONTROLLER    PIOC
#define TS_CART_ADDR3_PERIPHERAL    PERIPHERAL_A
#define TS_CART_ADDR3_PIO_NUM       21
#define TS_CART_ADDR3_PIO           (1u << TS_CART_ADDR3_PIO_NUM)
//	A4  on PC22 is A4  on PIO/SMC which is attached to Peripheral A 
#define TS_CART_ADDR4_CONTROLLER    PIOC
#define TS_CART_ADDR4_PERIPHERAL    PERIPHERAL_A
#define TS_CART_ADDR4_PIO_NUM       22
#define TS_CART_ADDR4_PIO           (1u << TS_CART_ADDR4_PIO_NUM)
//	A5  on PC23 is A5  on PIO/SMC which is attached to Peripheral A 
#define TS_CART_ADDR5_CONTROLLER    PIOC
#define TS_CART_ADDR5_PERIPHERAL    PERIPHERAL_A
#define TS_CART_ADDR5_PIO_NUM       23
#define TS_CART_ADDR5_PIO           (1u << TS_CART_ADDR5_PIO_NUM)
//	A6  on PC24 is A6  on PIO/SMC which is attached to Peripheral A 
#define TS_CART_ADDR6_CONTROLLER    PIOC
#define TS_CART_ADDR6_PERIPHERAL    PERIPHERAL_A
#define TS_CART_ADDR6_PIO_NUM       24
#define TS_CART_ADDR6_PIO           (1u << TS_CART_ADDR6_PIO_NUM)
//	A7  on PC25 is A7  on PIO/SMC which is attached to Peripheral A 
#define TS_CART_ADDR7_CONTROLLER    PIOC
#define TS_CART_ADDR7_PERIPHERAL    PERIPHERAL_A
#define TS_CART_ADDR7_PIO_NUM       25
#define TS_CART_ADDR7_PIO           (1u << TS_CART_ADDR7_PIO_NUM)
//	A8  on PC26 is A8  on PIO/SMC which is attached to Peripheral A 
#define TS_CART_ADDR8_CONTROLLER    PIOC
#define TS_CART_ADDR8_PERIPHERAL    PERIPHERAL_A
#define TS_CART_ADDR8_PIO_NUM       26
#define TS_CART_ADDR8_PIO           (1u << TS_CART_ADDR8_PIO_NUM)
//	A9  on PC27 is A9  on PIO/SMC which is attached to Peripheral A 
#define TS_CART_ADDR9_CONTROLLER    PIOC
#define TS_CART_ADDR9_PERIPHERAL    PERIPHERAL_A
#define TS_CART_ADDR9_PIO_NUM       27
#define TS_CART_ADDR9_PIO           (1u << TS_CART_ADDR9_PIO_NUM)
//	A10 on PC28 is A10 on PIO/SMC which is attached to Peripheral A 
#define TS_CART_ADDR10_CONTROLLER   PIOC
#define TS_CART_ADDR10_PERIPHERAL   PERIPHERAL_A
#define TS_CART_ADDR10_PIO_NUM      28
#define TS_CART_ADDR10_PIO          (1u << TS_CART_ADDR10_PIO_NUM)
//	A11 on PC29 is A11 on PIO/SMC which is attached to Peripheral A 
#define TS_CART_ADDR11_CONTROLLER   PIOC
#define TS_CART_ADDR11_PERIPHERAL   PERIPHERAL_A
#define TS_CART_ADDR11_PIO_NUM      29
#define TS_CART_ADDR11_PIO          (1u << TS_CART_ADDR11_PIO_NUM)
//	A12 on PC30 is A12 on PIO/SMC which is attached to Peripheral A 
#define TS_CART_ADDR12_CONTROLLER   PIOC
#define TS_CART_ADDR12_PERIPHERAL   PERIPHERAL_A
#define TS_CART_ADDR12_PIO_NUM      30
#define TS_CART_ADDR12_PIO          (1u << TS_CART_ADDR12_PIO_NUM)
//	A13 on PC31 is A13 on PIO/SMC which is attached to Peripheral A 
#define TS_CART_ADDR13_CONTROLLER   PIOC
#define TS_CART_ADDR13_PERIPHERAL   PERIPHERAL_A
#define TS_CART_ADDR13_PIO_NUM      31
#define TS_CART_ADDR13_PIO          (1u << TS_CART_ADDR13_PIO_NUM)
//	A14 on PA18 is A14 on PIO/SMC which is attached to Peripheral C 
#define TS_CART_ADDR14_CONTROLLER   PIOA
#define TS_CART_ADDR14_PERIPHERAL   PERIPHERAL_A
#define TS_CART_ADDR14_PIO_NUM      14
#define TS_CART_ADDR14_PIO          (1u << TS_CART_ADDR14_PIO_NUM)
//	A15 on PA19 is A15 on PIO/SMC which is attached to Peripheral C 
#define TS_CART_ADDR15_CONTROLLER   PIOA
#define TS_CART_ADDR15_PERIPHERAL   PERIPHERAL_A
#define TS_CART_ADDR15_PIO_NUM      15
#define TS_CART_ADDR15_PIO          (1u << TS_CART_ADDR15_PIO_NUM)

// CGB Cartridge Control Pins
//	WR  on PC8  is NWE  on PIO/SMC which is attached to Peripheral A
#define TS_CART_NWE_CONTROLLER		PIOC
#define TS_CART_NWE_PERIPHERAL		PERIPHERAL_A
#define TS_CART_NWE_PIO_NUM			8
#define TS_CART_NWE_PIO				(1u << TS_CART_NWE_PIO_NUM)
//	!RD on PC11 is NRD  on PIO/SMC which is attached to Peripheral A
#define TS_CART_NRD_CONTROLLER		PIOC
#define TS_CART_NRD_PERIPHERAL		PERIPHERAL_A
#define TS_CART_NRD_PIO_NUM			11
#define TS_CART_NRD_PIO				(1u << TS_CART_NRD_PIO_NUM)
//	!CS on PC14 is NCS0 on PIO/SMC which is attached to Peripheral A
#define TS_CART_NCS0_CONTROLLER		PIOC
#define TS_CART_NCS0_PERIPHERAL		PERIPHERAL_A
#define TS_CART_NCS0_PIO_NUM		14
#define TS_CART_NCS0_PIO			(1u << TS_CART_RST_PIO_NUM)
//	VIN on PB3  is AD7  on ADC (Extra Function/Peripheral)
#define TS_CART_VIN_CONTROLLER		PIOB
#define TS_CART_VIN_PERIPHERAL		PERIPHERAL_EXTRA
#define TS_CART_VIN_PIO_NUM			3
#define TS_CART_VIN_PIO				(1u << TS_CART_VIN_PIO_NUM)
#define TS_CART_VIN_ADC_NUM			7
#define TS_CART_VIN_ADC				(1u << TS_CART_VIN_ADC_NUM)
//	RST	on PA9  is GPIO
#define TS_CART_RST_CONTROLLER		PIOA
#define TS_CART_RST_PERIPHERAL		PERIPHERAL_GPIO
#define TS_CART_RST_PIO_NUM			9
#define TS_CART_RST_PIO				(1u << TS_CART_RST_PIO_NUM)
//	CLK on PA27 is GPIO
#define TS_CART_CLK_CONTROLLER		PIOA
#define TS_CART_CLK_PERIPHERAL		PERIPHERAL_GPIO
#define TS_CART_CLK_PIO_NUM			27
#define TS_CART_CLK_PIO				(1u << TS_CART_CLK_PIO_NUM)
//	Note: ADC is attached to Interrupt ADC which has an ID of 29

#endif /* BOARD_H_ */
