#include "gbdump.h"

void dump_region(void* buffer, unsigned int length) {
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

void dump_string(char* buffer, unsigned int length) {
	char buf[32];

	memcpy(buf, buffer, length);
	buf[length] = 0;

	printf("%s", buf);
}

int write_packet(PPKT_HEADER packet, unsigned int packetSize) {
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

int write_data_to_address(unsigned short address, unsigned char *data, unsigned int length) {
	WRITE_PKT writePkt;
	DWORD bytesWritten;
	
	writePkt.header.ptype = PTYPE_WRITE;
	writePkt.header.checksum = 0; // Calculated in write_packet()
	writePkt.startAddress = address;
	writePkt.length = 1;
	writePkt.ramAddr = 0; // Not RAM
	if (!write_packet(&writePkt.header, sizeof(writePkt))) {
		fprintf(stderr, "Failed to write cart write request\n");
		return 0;
	}

	if (!WriteFile(hSerialPort, data, length, &bytesWritten, NULL)) {
		fprintf(stderr, "Failed to write to serial port: %d\n", GetLastError());
		return 0;
	}

	return 1;
}

int read_header(PCART_HEADER_AREA cartHeader) {
	READ_PKT readPkt;
	DWORD bytesRead;
	DWORD offset;

	readPkt.header.ptype = PTYPE_READ;
	readPkt.header.checksum = 0; // Calculated in write_packet()
	readPkt.startAddress = CART_HEADER_ADDRESS;
	readPkt.length = CART_HEADER_LENGTH;
	readPkt.ramAddr = 0; // ROM
	if (!write_packet(&readPkt.header, sizeof(readPkt))) {
		fprintf(stderr, "Failed to write cart header read request\n");
		return 0;
	}

	offset = 0;
	while (offset < sizeof(*cartHeader)) {
		if (!ReadFile(hSerialPort, &((char*)cartHeader)[offset], sizeof(*cartHeader) - offset, &bytesRead, NULL)) {
			fprintf(stderr, "Failed to read cart header: %d\n", GetLastError());
			return 0;
		}

		offset += bytesRead;
	}

	//dump_region(cartHeader, sizeof(*cartHeader));

	return 1;
}