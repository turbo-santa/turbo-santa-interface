#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

extern HANDLE hSerialPort;

#define PTYPE_READ           0x1
#define PTYPE_WRITE          0x2

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

#define CART_HEADER_ADDRESS 0x100
#define CART_HEADER_LENGTH  0x050

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

int dump_rom(FILE *file, CART_HEADER_AREA *header);

int dump_ram(FILE *file, CART_HEADER_AREA *header);

int read_header(PCART_HEADER_AREA cartHeader);
int write_data_to_address(unsigned short address, unsigned char *data, unsigned int length);
int write_packet(PPKT_HEADER packet, unsigned int packetSize);
void dump_region(void* buffer, unsigned int length);
void dump_string(char* buffer, unsigned int length);
int read_and_verify_crc32(unsigned int expectedCrc32);

unsigned int crc32(unsigned int crc, const void *buf, unsigned int size);