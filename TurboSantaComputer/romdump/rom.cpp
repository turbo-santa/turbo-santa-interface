#include "gbdump.h"

#define ROM_BANK_SWITCH_ADDRESS_MBC13 0x2000
#define ROM_BANK_SWITCH_ADDRESS_MBC2  0x2100

#define PRIMARY_ROM_BANK_ADDRESS   0x0000
#define SECONDARY_ROM_BANK_ADDRESS 0x4000
#define ROM_BANK_LENGTH            0x4000

static int rom_bank_switch(unsigned char bank, unsigned char cartType) {
	switch (cartType)
	{
		// MBC1
	case 0x1:
	case 0x2:
	case 0x3:

		// MBC3
	case 0xF:
	case 0x10:
	case 0x11:
	case 0x12:
	case 0x13:

		// Guess for others
	default:
		return write_data_to_address(ROM_BANK_SWITCH_ADDRESS_MBC13, &bank, 1);

		// MBC2
	case 0x5:
	case 0x6:
		return write_data_to_address(ROM_BANK_SWITCH_ADDRESS_MBC2, &bank, 1);
	}
}

// bankData must be 0x4000 bytes
static int read_rom_bank(unsigned char bank, char* bankData, unsigned char cartType) {
	READ_PKT readPkt;
	DWORD bytesRead;
	DWORD offset;
	unsigned int crc;
	int err;

RetryRead:
	if (bank == 0) {
		readPkt.header.ptype = PTYPE_READ;
		readPkt.header.checksum = 0; // Calculated in write_packet()
		readPkt.startAddress = PRIMARY_ROM_BANK_ADDRESS;
		readPkt.length = ROM_BANK_LENGTH;
		readPkt.ramAddr = 0; // ROM
		if (!write_packet(&readPkt.header, sizeof(readPkt))) {
			fprintf(stderr, "Failed to write bank 0 read request\n");
			return 0;
		}
	}
	else {
		// Cart type 0 doesn't have banks to switch
		if (cartType != 0) {
			// All banks other than 0 will require a switch
			if (!rom_bank_switch(bank, cartType)) {
				return 0;
			}
		}

		// Read from the secondary bank
		readPkt.header.ptype = PTYPE_READ;
		readPkt.header.checksum = 0; // Calculated in write_packet()
		readPkt.startAddress = SECONDARY_ROM_BANK_ADDRESS;
		readPkt.length = ROM_BANK_LENGTH;
		readPkt.ramAddr = 0; // ROM
		if (!write_packet(&readPkt.header, sizeof(readPkt))) {
			fprintf(stderr, "Failed to write secondary bank read request\n");
			return 0;
		}
	}

	crc = 0;

	offset = 0;
	while (offset < ROM_BANK_LENGTH) {
		if (!ReadFile(hSerialPort, &bankData[offset], ROM_BANK_LENGTH - offset, &bytesRead, NULL)) {
			fprintf(stderr, "Failed to read bank: %d\n", GetLastError());
			return 0;
		}

		crc = crc32(crc, &bankData[offset], bytesRead);
			
		offset += bytesRead;
	}

	err = read_and_verify_crc32(crc);
	if (err < 0) {
		return 0;
	}
	else if (err == 0) {
		fprintf(stderr, "CRC mismatch; rereading...");
		goto RetryRead;
	}

	return 1;
}


int dump_rom(FILE *file, CART_HEADER_AREA *header) {
	char *bankBuffer;
	unsigned int bankCount;
	unsigned char i;

	switch (header->romSize)
	{
	case 0x0:
		bankCount = 2;
		break;
	case 0x1:
		bankCount = 4;
		break;
	case 0x2:
		bankCount = 8;
		break;
	case 0x3:
		bankCount = 16;
		break;
	case 0x4:
		bankCount = 32;
		break;
	case 0x5:
		bankCount = 64;
		break;
	case 0x6:
		bankCount = 128;
		break;
	case 0x7:
		bankCount = 256;
		break;
	case 0x52:
		bankCount = 72;
		break;
	case 0x53:
		bankCount = 80;
		break;
	case 0x54:
		bankCount = 96;
		break;
	default:
		fprintf(stderr, "Invalid ROM size\n");
		return -4;
	}

	bankBuffer = (char*)malloc(ROM_BANK_LENGTH);
	if (bankBuffer == NULL) {
		fprintf(stderr, "Failed to allocate bank memory\n");
		return -5;
	}

	for (i = 0; i < bankCount; i++) {
		printf("Dumping ROM bank %d...", i);

		if (!read_rom_bank(i, bankBuffer, header->cartType)) {
			fprintf(stderr, "Failed to read ROM bank %d\n", i);
			return -6;
		}

		fwrite(bankBuffer, ROM_BANK_LENGTH, 1, file);

		printf("done\n");
	}

	return 0;
}