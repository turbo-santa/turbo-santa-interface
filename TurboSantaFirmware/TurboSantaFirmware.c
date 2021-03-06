/*
 * TurboSantaInterface.c
 *
 * Created: 16 Nov 2014 3:55:30 PM
 *  Author: Andrew
 */ 

#include "sam.h"
#include "startup.h"

#define PTYPE_READ        0x01
#define PTYPE_WRITE       0x02

#pragma pack(push, 1)

typedef struct _PKT_HEADER {
	unsigned char ptype;
	unsigned char checksum;
} PKT_HEADER, *PPKT_HEADER;

typedef struct _READ_PKT {
	PKT_HEADER header;
	unsigned short startAddress;
	unsigned short length;
	unsigned char ramAddr;
} READ_PKT, *PREAD_PKT;

typedef struct _WRITE_PKT {
	PKT_HEADER header;
	unsigned short startAddress;
	unsigned short length;
	unsigned char ramAddr;
} WRITE_PKT, *PWRITE_PKT;

typedef union _PKT_UNION {
	READ_PKT read;
	WRITE_PKT write;
} PKT_UNION, *PPKT_UNION;

#pragma pack(pop)

unsigned int
crc32(unsigned int crc, const void *buf, unsigned int size);

void sendCrc32(unsigned int crc) {
	usart0_ftdi_putchar(crc & 0xFF);
	usart0_ftdi_putchar(crc >> 8);
	usart0_ftdi_putchar(crc >> 16);
	usart0_ftdi_putchar(crc >> 24);
}

void readPacket(PPKT_UNION pktUnion)
{
	pktUnion->read.header.ptype = usart0_ftdi_getchar();
	pktUnion->read.header.checksum = usart0_ftdi_getchar();
	
	if (pktUnion->read.header.ptype == PTYPE_READ ||
	    pktUnion->read.header.ptype == PTYPE_WRITE) {
		pktUnion->read.startAddress = usart0_ftdi_getchar() | usart0_ftdi_getchar() << 8;
		pktUnion->read.length = usart0_ftdi_getchar() | usart0_ftdi_getchar() << 8;
		pktUnion->read.ramAddr = usart0_ftdi_getchar();
	}
}

void writeAddress(unsigned short address, unsigned char data, unsigned char ramAddr)
{
	set_pin_high(TS_CART_DDIR_CONTROLLER, TS_CART_DDIR_PIO);
	configure_pin_as_pio_output(TS_CART_DATA0_CONTROLLER, TS_CART_DATA0_PIO, INITIAL_STATE_LOW);
	configure_pin_as_pio_output(TS_CART_DATA1_CONTROLLER, TS_CART_DATA1_PIO, INITIAL_STATE_LOW);
	configure_pin_as_pio_output(TS_CART_DATA2_CONTROLLER, TS_CART_DATA2_PIO, INITIAL_STATE_LOW);
	configure_pin_as_pio_output(TS_CART_DATA3_CONTROLLER, TS_CART_DATA3_PIO, INITIAL_STATE_LOW);
	configure_pin_as_pio_output(TS_CART_DATA4_CONTROLLER, TS_CART_DATA4_PIO, INITIAL_STATE_LOW);
	configure_pin_as_pio_output(TS_CART_DATA5_CONTROLLER, TS_CART_DATA5_PIO, INITIAL_STATE_LOW);
	configure_pin_as_pio_output(TS_CART_DATA6_CONTROLLER, TS_CART_DATA6_PIO, INITIAL_STATE_LOW);
	configure_pin_as_pio_output(TS_CART_DATA7_CONTROLLER, TS_CART_DATA7_PIO, INITIAL_STATE_LOW);
	
	// WR starts high and RD starts low
	set_pin_high(TS_CART_NWE_CONTROLLER, TS_CART_NWE_PIO);
	set_pin_low(TS_CART_NRD_CONTROLLER, TS_CART_NRD_PIO);
	
	// Pull CS low to access RAM
	if (ramAddr) {
		set_pin_low(TS_CART_NCS0_CONTROLLER, TS_CART_NCS0_PIO);
	}
	else {
		set_pin_high(TS_CART_NCS0_CONTROLLER, TS_CART_NCS0_PIO);
	}
	
		NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	
	// Write the destination address
	write_to_address(address);
	
			NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	
	// Write the data
	write_to_data(data);
	
	// Bring RD up
	set_pin_high(TS_CART_NRD_CONTROLLER, TS_CART_NRD_PIO);
	
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	
	// Bring WR active for at least 300 ns
	set_pin_low(TS_CART_NWE_CONTROLLER, TS_CART_NWE_PIO);
	
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	
	// Bring WR back up and RD down
	set_pin_high(TS_CART_NWE_CONTROLLER, TS_CART_NWE_PIO);
	set_pin_low(TS_CART_NRD_CONTROLLER, TS_CART_NRD_PIO);
	
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	
	// Pull CS high again
	set_pin_high(TS_CART_NCS0_CONTROLLER, TS_CART_NCS0_PIO);
	
	set_pin_low(TS_CART_DDIR_CONTROLLER, TS_CART_DDIR_PIO);
	configure_pin_as_pio_input(TS_CART_DATA0_CONTROLLER, TS_CART_DATA0_PIO);
	configure_pin_as_pio_input(TS_CART_DATA1_CONTROLLER, TS_CART_DATA1_PIO);
	configure_pin_as_pio_input(TS_CART_DATA2_CONTROLLER, TS_CART_DATA2_PIO);
	configure_pin_as_pio_input(TS_CART_DATA3_CONTROLLER, TS_CART_DATA3_PIO);
	configure_pin_as_pio_input(TS_CART_DATA4_CONTROLLER, TS_CART_DATA4_PIO);
	configure_pin_as_pio_input(TS_CART_DATA5_CONTROLLER, TS_CART_DATA5_PIO);
	configure_pin_as_pio_input(TS_CART_DATA6_CONTROLLER, TS_CART_DATA6_PIO);
	configure_pin_as_pio_input(TS_CART_DATA7_CONTROLLER, TS_CART_DATA7_PIO);
	
			NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
}

char readAddress(unsigned short address, unsigned char ramAddr)
{
	char data;
	
	set_pin_high(TS_CART_NWE_CONTROLLER, TS_CART_NWE_PIO);
	set_pin_low(TS_CART_NRD_CONTROLLER, TS_CART_NRD_PIO);
	
	// Pull CS low to access RAM
	if (ramAddr) {
		set_pin_low(TS_CART_NCS0_CONTROLLER, TS_CART_NCS0_PIO);
	}
	else {
		set_pin_high(TS_CART_NCS0_CONTROLLER, TS_CART_NCS0_PIO);
	}
	
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	
	write_to_address(address);
	
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	NOPNOPNOPTOAST
	
	data = read_from_data();
	
	set_pin_high(TS_CART_NCS0_CONTROLLER, TS_CART_NCS0_PIO);
	
	return data;
}

void scopeMe(void) {
	int data = 0;
	
	data |= ((read_pin_level(TS_LINK_SI_CONTROLLER, TS_LINK_SI_PIO) != 0) ? 1 : 0) << 0;
	data |= ((read_pin_level(TS_LINK_SO_CONTROLLER, TS_LINK_SO_PIO) != 0) ? 1 : 0) << 1;
	data |= ((read_pin_level(TS_LINK_SC_CONTROLLER, TS_LINK_SC_PIO) != 0) ? 1 : 0) << 2;
	data |= ((read_pin_level(TS_LINK_SD_CONTROLLER, TS_LINK_SD_PIO) != 0) ? 1 : 0) << 3;
	
	print_hex_byte(data);
}

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
	initialize_cart_pio();
	
	while (1) {
		scopeMe();
	}

    while (1) {
		PKT_UNION pktUnion;
		unsigned short i;
		unsigned int crc;
		
		readPacket(&pktUnion);
		
		crc = 0;
		
		if (pktUnion.read.header.ptype == PTYPE_READ) {
			// Read each byte and send it over the USART
			for (i = 0; i < pktUnion.read.length; i++) {
				unsigned char data = readAddress(pktUnion.read.startAddress + i, pktUnion.read.ramAddr);
				crc = crc32(crc, &data, 1);
				usart0_ftdi_putchar(data);
			}
			
			// Send the CRC32 at the end
			sendCrc32(crc);
		}
		else if (pktUnion.write.header.ptype == PTYPE_WRITE) {
			// Write each byte from the USART
			for (i = 0; i < pktUnion.write.length; i++) {
				unsigned char data = usart0_ftdi_getchar();
				writeAddress(pktUnion.write.startAddress + i, data, pktUnion.write.ramAddr);
			}
		}
    }
}
