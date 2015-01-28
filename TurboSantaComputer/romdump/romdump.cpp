#include <Windows.h>

#include <stdio.h>

#define PTYPE_READ           0x1
#define PTYPE_BANK_SWITCH    0x2

#pragma pack(push, 1)

typedef struct _PKT_HEADER {
	unsigned char ptype;
	unsigned char checksum;
} PKT_HEADER, *PPKT_HEADER;

typedef struct _READ_PKT {
	PKT_HEADER header;
	unsigned short startAddress;
	unsigned short length;
} READ_PKT, *PREAD_PKT;

typedef struct _BANK_SWITCH_PKT {
	PKT_HEADER header;
	unsigned char newBank;
	unsigned char cartType;
} BANK_SWITCH_PKT, *PBANK_SWITCH_PKT;

typedef union _PKT_UNION {
	READ_PKT read;
	BANK_SWITCH_PKT bankSwitch;
} PKT_UNION, *PPKT_UNION;

#define CART_HEADER_ADDRESS 0x100
#define CART_HEADER_LENGTH  0x050

#define PRIMARY_BANK_ADDRESS   0x0000
#define SECONDARY_BANK_ADDRESS 0x4000
#define BANK_LENGTH            0x4000

typedef struct _CART_HEADER_AREA {
	unsigned char nopJump[0x4];
	unsigned char logo[0x30];
	char gameTitle[0xB];
	char manufacturerCode[0x4];
	unsigned char cgbFlag;
	char newLicenseeCode[0x2];
	unsigned char sgbFlag;
	unsigned char cartType;
	unsigned char romSize;
	unsigned char ramSize;
	unsigned char destinationCode;
	unsigned char oldLicenseeCode;
	unsigned char romVersion;
	unsigned char headerChecksum;
	unsigned short globalChecksum;
} CART_HEADER_AREA, *PCART_HEADER_AREA;
C_ASSERT(sizeof(CART_HEADER_AREA) == CART_HEADER_LENGTH);

#pragma pack(pop)

static HANDLE hSerialPort;

static void dump_region(void* buffer, unsigned int length) {
	unsigned int i;
	unsigned char* buf = (unsigned char*)buffer;

	for (i = 0; i < length; i++) {
		if (i != 0) {
			if (i % 16 == 0) {
				printf("\n");
			}
			else if (i % 8 == 0) {
				printf(" ");
			}
		}

		printf("%02x ", buf[i]);
	}
}

static void dump_string(char* buffer, unsigned int length) {
	char buf[32];

	memcpy(buf, buffer, length);
	buf[length] = 0;

	printf("%s", buf);
}

static int write_packet(PPKT_HEADER packet, unsigned int packetSize) {
	DWORD bytesWritten;
	unsigned int i;
	char *pktBuf = (char*)packet;
	char checksum;

	// Compute the checksum of the outgoing packet
	checksum = 0;
	packet->checksum = 0;
	for (i = 0; i < packetSize; i++) {
		checksum ^= pktBuf[i];
	}
	packet->checksum = checksum;

	//dump_region(packet, packetSize);

	if (!WriteFile(hSerialPort, packet, packetSize, &bytesWritten, NULL)) {
		fprintf(stderr, "Failed to write to serial port: %d\n", GetLastError());
		return 0;
	}

	return 1;
}

// bankData must be 0x4000 bytes
static int read_bank(unsigned char bank, char* bankData, unsigned char cartType) {
	READ_PKT readPkt;
	BANK_SWITCH_PKT bankPkt;
	DWORD bytesRead;
	DWORD offset;

	if (bank == 0) {
		readPkt.header.ptype = PTYPE_READ;
		readPkt.header.checksum = 0; // Calculated in write_packet()
		readPkt.startAddress = PRIMARY_BANK_ADDRESS;
		readPkt.length = BANK_LENGTH;
		if (!write_packet(&readPkt.header, sizeof(readPkt))) {
			fprintf(stderr, "Failed to write bank 0 read request\n");
			return 0;
		}
	}
	else {
		// Cart type 0 doesn't have banks to switch
		if (cartType != 0) {
			// All other banks will require a switch
			bankPkt.header.ptype = PTYPE_BANK_SWITCH;
			bankPkt.header.checksum = 0; // Calculated in write_packet()
			bankPkt.newBank = bank;
			bankPkt.cartType = cartType;
			if (!write_packet(&bankPkt.header, sizeof(bankPkt))) {
				fprintf(stderr, "Failed to write bank switch request\n");
				return 0;
			}
		}

		// Read from the secondary bank
		readPkt.header.ptype = PTYPE_READ;
		readPkt.header.checksum = 0; // Calculated in write_packet()
		readPkt.startAddress = SECONDARY_BANK_ADDRESS;
		readPkt.length = BANK_LENGTH;
		if (!write_packet(&readPkt.header, sizeof(readPkt))) {
			fprintf(stderr, "Failed to write secondary bank read request\n");
			return 0;
		}
	}

	offset = 0;
	while (offset < BANK_LENGTH) {
		if (!ReadFile(hSerialPort, &bankData[offset], BANK_LENGTH - offset, &bytesRead, NULL)) {
			fprintf(stderr, "Failed to read bank: %d\n", GetLastError());
			return 0;
		}

		offset += bytesRead;
	}

	return 1;
}

static int read_header(PCART_HEADER_AREA cartHeader) {
	READ_PKT readPkt;
	DWORD bytesRead;
	DWORD offset;

	readPkt.header.ptype = PTYPE_READ;
	readPkt.header.checksum = 0; // Calculated in write_packet()
	readPkt.startAddress = CART_HEADER_ADDRESS;
	readPkt.length = CART_HEADER_LENGTH;
	if (!write_packet(&readPkt.header, sizeof(readPkt))) {
		fprintf(stderr, "Failed to write cart header read request\n");
		return 0;
	}

	// FIXME FIXME FIMXE
	offset = 0;
	while (offset < sizeof(*cartHeader) - 1) {
		if (!ReadFile(hSerialPort, &((char*)cartHeader)[offset], sizeof(*cartHeader) - offset, &bytesRead, NULL)) {
			fprintf(stderr, "Failed to read cart header: %d\n", GetLastError());
			return 0;
		}

		offset += bytesRead;
	}

	//dump_region(cartHeader, sizeof(*cartHeader));

	return 1;
}



int main(int argc, char* argv[])
{
	char path[MAX_PATH];
	CART_HEADER_AREA header;
	unsigned char i;
	unsigned char bankCount;
	char *bankBuffer;
	FILE *file;
	DCB port;

	if (argc != 3) {
		//printf("Usage: romdump <COM #> <target file>\n");
		//return -1;
	}

	file = fopen("C:\\ROMs\\pokemonsilvercap.gb"/*argv[2]*/, "wb");
	if (file == NULL) {
		fprintf(stderr, "Failed to open output file: %d\n", errno);
		return -1;
	}

	sprintf(path, "\\\\.\\%s", argv[1]);
	hSerialPort = CreateFile("\\\\.\\COM11",//path,
		GENERIC_READ | GENERIC_WRITE | SYNCHRONIZE,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	if (hSerialPort == INVALID_HANDLE_VALUE) {
		fprintf(stderr, "Failed to open serial port: %d\n", GetLastError());
		return -2;
	}

	memset(&port, 0, sizeof(port));
	port.DCBlength = sizeof(port);
	if (!GetCommState(hSerialPort, &port)) {
		fprintf(stderr, "Failed to get comm state: %d\n", GetLastError());
		return -8;
	}
	if (!BuildCommDCB("baud=38400 parity=n data=8 stop=1", &port)) {
		fprintf(stderr, "Failed to build comm DCB: %d\n", GetLastError());
		return -8;
	}
	if (!SetCommState(hSerialPort, &port)) {
		fprintf(stderr, "Failed to set comm state: %d\n", GetLastError());
		return -8;
	}

	printf("Press ENTER to start dumping the ROM...");
	getchar();

	if (!read_header(&header)) {
		return -3;
	}

	printf("NOP-JUMP Area: ");
	dump_region(&header.nopJump[0], sizeof(header.nopJump));
	printf("\nLogo Area: \n");
	dump_region(&header.logo[0], sizeof(header.logo));
	printf("\nTitle: ");
	dump_string(&header.gameTitle[0], sizeof(header.gameTitle));
	printf("\nManufacturer Code: ");
	dump_string(&header.manufacturerCode[0], sizeof(header.manufacturerCode));
	printf("\nCGB Flag: %02x", header.cgbFlag);
	printf("\nNew Licensee Code: ");
	dump_string(&header.newLicenseeCode[0], sizeof(header.newLicenseeCode));
	printf("\nSGB Flag: %02x", header.sgbFlag);
	printf("\nCart Type: %02x", header.cartType);
	printf("\nROM Size: %02x", header.romSize);
	printf("\nRAM Size: %02x", header.ramSize);
	printf("\nDestination Code: %02x", header.destinationCode);
	printf("\nOld Licensee Code: %02x", header.oldLicenseeCode);
	printf("\nROM Version: %02x", header.romVersion);
	printf("\nHeader Checksum: %02x", header.headerChecksum);
	printf("\nGlobal Checksum: %04x\n", header.globalChecksum);

	switch (header.romSize)
	{
	case 0:
		bankCount = 2;
		break;
	case 1:
		bankCount = 4;
		break;
	case 2:
		bankCount = 8;
		break;
	case 3:
		bankCount = 16;
		break;
	case 4:
		bankCount = 32;
		break;
	case 5:
		bankCount = 64;
		break;
	case 6:
		bankCount = 128;
		break;
	default:
		fprintf(stderr, "Invalid ROM size\n");
		return -4;
	}

	bankBuffer = (char*)malloc(BANK_LENGTH);
	if (bankBuffer == NULL) {
		fprintf(stderr, "Failed to allocate bank memory\n");
		return -5;
	}

	for (i = 0; i < bankCount; i++) {
		printf("Dumping bank %d...", i);

		if (!read_bank(i, bankBuffer, header.cartType)) {
			fprintf(stderr, "Failed to read bank %d\n", i);
			return -6;
		}

		fwrite(bankBuffer, BANK_LENGTH, 1, file);

		printf("done\n");
	}

	fflush(file);
	fclose(file);

	return 0;
}

