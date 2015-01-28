#include "gbdump.h"

HANDLE hSerialPort;

int main(int argc, char* argv[])
{
	char path[MAX_PATH];
	CART_HEADER_AREA header;
	unsigned char dumpRom;
	FILE *file;
	DCB port;
	int err;

	if (argc != 4) {
		printf("Usage: romdump <ram/rom> <COM #> <target file>\n");
		return -1;
	}

	if (!_stricmp(argv[1], "ram")) {
		dumpRom = 0;
	}
	else if (!_stricmp(argv[1], "rom")) {
		dumpRom = 1;
	}
	else {
		printf("Must specify dump type: ROM or RAM\n");
		return -1;
	}

	file = fopen(argv[3], "wb");
	if (file == NULL) {
		fprintf(stderr, "Failed to open output file: %d\n", errno);
		return -1;
	}

	sprintf(path, "\\\\.\\%s", argv[2]);
	hSerialPort = CreateFile(path,
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

	printf("Press ENTER to start dumping the %s...",
		dumpRom ? "ROM" : "RAM");
	(void) getchar();

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

	if (dumpRom) {
		err = dump_rom(file, &header);
	}
	else {
		err = dump_ram(file, &header);
	}

	fflush(file);
	fclose(file);

	return err;
}

