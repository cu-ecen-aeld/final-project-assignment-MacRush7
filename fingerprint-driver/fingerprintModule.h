#include <syslog.h>
#include <stdio.h>
#include <stdint.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include <wiringPiSPI.h>
#include "bufferModule.h"

#define BUF_SIZE	100
#define NUM_IMAGES	6

extern int alarmLock;

struct fingerprintData
{
	int printID;
	char name[30];
}employee;

uint32_t fingerprintBuffer[BUF_SIZE];
uint32_t start = 0xEF;

// fingerprint commands... see reference
uint32_t GetImage[] = {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x03, 0x01, 0x00, 0x05};
uint32_t GenChar[] = {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x04, 0x02, 0x01, 0x00, 0x00};
uint32_t RegModel[] = {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x03, 0x05, 0x00, 0x09};
uint32_t StoreChar[] = {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x06, 0x06, 0x01, 0x00, 0x01, 0x00, 0x0F};
uint32_t Search[] = {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x08, 0x04, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint32_t DeleteChar[] = {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x07, 0x0C, 0x00, 0x05, 0x00, 0x0A, 0x00, 0x23};
uint32_t Empty[] = {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x03, 0x0D, 0x00, 0x11};
	
// cmd lengths
uint32_t GetImageLength = sizeof(GetImage)/sizeof(GetImage[0]);
uint32_t GenCharLength = sizeof(GenChar)/sizeof(GenChar[0]);
uint32_t RegModelLength = sizeof(RegModel)/sizeof(RegModel[0]);
uint32_t StoreCharLength = sizeof(StoreChar)/sizeof(StoreChar[0]);
uint32_t SearchLength = sizeof(Search)/sizeof(Search[0]);
uint32_t DeleteCharLength = sizeof(DeleteChar)/sizeof(DeleteChar[0]);
uint32_t EmptyLength = sizeof(Empty)/sizeof(Empty[0]);

int checkButton();
int checksum(uint32_t cmd[], uint32_t length);
void clearBuffer();
