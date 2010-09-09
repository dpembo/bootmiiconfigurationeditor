/****************************************************************************
 * WiiMC
 * Tantric 2009-2010
 *
 * fileop.h
 *
 * File operations
 ****************************************************************************/


#include <gccore.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX_DEVICES 10

typedef struct
{
	char name[100];
	char mount[10];
	int type;
	DISC_INTERFACE* interface;
	sec_t sector;
} DEVICE_STRUCT;

extern DEVICE_STRUCT part[2][MAX_DEVICES];

void MountAllDevices();
void UnmountAllDevices();
