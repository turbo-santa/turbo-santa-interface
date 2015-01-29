#include "gbdump.h"

#define RAM_ENABLE_ADDRESS         0x0000
#define RAM_ENABLE_DATA            0x0A
#define RAM_DISABLE_DATA           0x00

#define RAM_BANK_ADDRESS           0xA000
#define MAX_RAM_BANK_LENGTH        0x2000

#define MODE_SELECT_ADDRESS        0x6000

#define RAM_BANK_NUMBER_ADDRESS    0x4000

static int set_ram_enable(unsigned char ramEnable) {
	unsigned char data = ramEnable ? RAM_ENABLE_DATA : RAM_DISABLE_DATA;
	return write_data_to_address(RAM_ENABLE_ADDRESS, &data, 1);
}

static int set_mode_select(unsigned char mode) {
	return write_data_to_address(MODE_SELECT_ADDRESS, &mode, 1);
}

static int toggle_rtc_latch(void) {
	int err = 0;
	unsigned char data;
	
	data = 0;
	err |= write_data_to_address(MODE_SELECT_ADDRESS, &data, 1);

	data = 1;
	err |= write_data_to_address(MODE_SELECT_ADDRESS, &data, 1);

	return err;
}

static int ram_bank_switch(unsigned char bank, unsigned char cartType) {
	switch (cartType) {
	case 0x02:
	case 0x03:
		// MBC1 needs mode select toggled
		if (bank == 0) {
			set_mode_select(0);
		}
		else {
			set_mode_select(1);
		}
		break;

		// MBC3 and MBC5 don't have anything special to toggle
	case 0x10:
	case 0x12:
	case 0x13:

	case 0x1A:
	case 0x1B:
	case 0x1D:
	case 0x1E:
		break;

	default:
		fprintf(stderr, "Cart type not supported: %x\n", cartType);
		return 0;
	}

	return write_data_to_address(RAM_BANK_NUMBER_ADDRESS, &bank, 1);
}

static int write_ram_bank(unsigned char bank, char* bankData, unsigned int bankDataLength, unsigned char cartType) {
	WRITE_PKT writePkt;
	DWORD bytesWritten;
	
	if (!ram_bank_switch(bank, cartType)) {
		fprintf(stderr, "Failed to switch RAM banks\n");
		return 0;
	}

	writePkt.header.ptype = PTYPE_WRITE;
	writePkt.header.checksum = 0; // Calculated in write_packet()
	writePkt.startAddress = RAM_BANK_ADDRESS;
	writePkt.length = bankDataLength;
	writePkt.ramAddr = 1; // RAM
	if (!write_packet(&writePkt.header, sizeof(writePkt))) {
		fprintf(stderr, "Failed to write RAM bank write request\n");
		return 0;
	}

	if (!WriteFile(hSerialPort, bankData, bankDataLength, &bytesWritten, NULL)) {
		fprintf(stderr, "Failed to write to serial port: %d\n", GetLastError());
		return 0;
	}

	return 1;
}

static int read_ram_bank(unsigned char bank, char* bankData, unsigned int bankDataLength, unsigned char cartType) {
	READ_PKT readPkt;
	DWORD bytesRead;
	DWORD offset;
	unsigned int crc;
	int err;

RetryRead:
	if (!ram_bank_switch(bank, cartType)) {
		fprintf(stderr, "Failed to switch RAM banks\n");
		return 0;
	}

	readPkt.header.ptype = PTYPE_READ;
	readPkt.header.checksum = 0; // Calculated in write_packet()
	readPkt.startAddress = RAM_BANK_ADDRESS;
	readPkt.length = bankDataLength;
	readPkt.ramAddr = 1; // RAM
	if (!write_packet(&readPkt.header, sizeof(readPkt))) {
		fprintf(stderr, "Failed to write RAM bank read request\n");
		return 0;
	}

	crc = 0;

	offset = 0;
	while (offset < bankDataLength) {
		if (!ReadFile(hSerialPort, &bankData[offset], bankDataLength - offset, &bytesRead, NULL)) {
			fprintf(stderr, "Failed to read RAM bank: %d\n", GetLastError());
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

int write_ram(FILE *file, CART_HEADER_AREA *header) {
	char *bankBuffer;
	unsigned char bankCount;
	unsigned char i;
	int bankLength;

	// Enable RAM for writing
	set_ram_enable(1);

	switch (header->ramSize)
	{
	case 0x00:
		// No RAM to dump
		return 0;
	case 0x01:
		// 2KB RAM
		bankCount = 1;
		bankLength = 0x800;
		break;
	case 0x02:
		// 8KB RAM
		bankCount = 1;
		bankLength = 0x2000;
		break;
	case 0x03:
		// 32KB RAM in 4 banks
		bankCount = 4;
		bankLength = 0x2000;
		break;
	default:
		fprintf(stderr, "Invalid RAM size\n");
		return -4;
	}

	bankBuffer = (char*)malloc(bankLength);
	if (bankBuffer == NULL) {
		fprintf(stderr, "Failed to allocate bank memory\n");
		return -5;
	}

	for (i = 0; i < bankCount; i++) {
		printf("Writing RAM bank %d...", i);

		fread(bankBuffer, bankLength, 1, file);

		if (!write_ram_bank(i, bankBuffer, bankLength, header->cartType)) {
			fprintf(stderr, "Failed to write RAM bank %d\n", i);
			free(bankBuffer);
			return -6;
		}

		printf("done\n");
	}

	// Disable RAM for safe power off
	set_ram_enable(0);

	// Set mode back to default
	set_mode_select(0);

	free(bankBuffer);

	return 0;
}

int dump_ram(FILE *file, CART_HEADER_AREA *header) {
	char *bankBuffer;
	unsigned char bankCount;
	unsigned char i;
	int bankLength;

	// Enable RAM for reading
	set_ram_enable(1);

	switch (header->ramSize)
	{
	case 0x00:
		// No RAM to dump
		return 0;
	case 0x01:
		// 2KB RAM
		bankCount = 1;
		bankLength = 0x800;
		break;
	case 0x02:
		// 8KB RAM
		bankCount = 1;
		bankLength = 0x2000;
		break;
	case 0x03:
		// 32KB RAM in 4 banks
		bankCount = 4;
		bankLength = 0x2000;
		break;
	default:
		fprintf(stderr, "Invalid RAM size\n");
		return -4;
	}

	bankBuffer = (char*)malloc(bankLength);
	if (bankBuffer == NULL) {
		fprintf(stderr, "Failed to allocate bank memory\n");
		return -5;
	}

	for (i = 0; i < bankCount; i++) {
		printf("Dumping RAM bank %d...", i);

		if (!read_ram_bank(i, bankBuffer, bankLength, header->cartType)) {
			fprintf(stderr, "Failed to read RAM bank %d\n", i);
			free(bankBuffer);
			return -6;
		}

		fwrite(bankBuffer, bankLength, 1, file);

		printf("done\n");
	}

	// Disable RAM for safe power off
	set_ram_enable(0);

	// Set mode back to default
	set_mode_select(0);

	free(bankBuffer);

	return 0;
}