/*
 * constants.h
 *
 *  Created on: Oct 8, 2016
 *      Author: Wituz
 */

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>
#include <libetc.h>
#include "sys/types.h"

#define OT_LENGTH 1
#define PACKETMAX 300

#define __ramsize   0x00200000
#define __stacksize 0x00004000

#define TYPE_LINE 0
#define TYPE_BOX 1

#define SCREEN_MODE_PAL 0
#define SCREEN_MODE_NTSC 1

#define DEBUG 1



typedef struct {
	int r;
	int g;
	int b;
} Color;

typedef struct {
	LINE_F2 line;
	int type;
} Line;

typedef struct {
	Line line[4];
	int type;
} Box;

typedef struct {
	RECT rect;
	RECT crect;
	GsIMAGE tim_data;
	GsSPRITE sprite;
} Image;

int 		SCREEN_WIDTH, SCREEN_HEIGHT;
GsOT 		orderingTable[2];
GsOT_TAG  	minorOrderingTable[2][1<<OT_LENGTH];
PACKET 		GPUOutputPacket[2][PACKETMAX];
short 		currentBuffer;
Color 		systemBackgroundColor;

//Initialize screen, 
void initializeScreen() {
	if (*(char *)0xbfc7ff52=='E') setScreenMode(SCREEN_MODE_PAL);
   	else setScreenMode(SCREEN_MODE_NTSC);

	SetDispMask(1);
	ResetGraph(0);
	clearVRAM();
	GsInitGraph(SCREEN_WIDTH, SCREEN_HEIGHT, GsINTER|GsOFSGPU, 0, 0); //Set up interlation..
	GsDefDispBuff(0, 0, 0, SCREEN_HEIGHT);	//..and double buffering.
	initializeOrderingTable();
}

void initializeDebugFont() {
	FntLoad(960, 256);
	SetDumpFnt(FntOpen(5, 20, 320, 240, 0, 512)); //Sets the dumped font for use with FntPrint();
}

void initializeOrderingTable(){
	GsClearOt(0,0,&orderingTable[GsGetActiveBuff()]);

	// initialise the ordering tables
	orderingTable[0].length = OT_LENGTH;
	orderingTable[1].length = OT_LENGTH;
	orderingTable[0].org = minorOrderingTable[0];
	orderingTable[1].org = minorOrderingTable[1];

	GsClearOt(0,0,&orderingTable[0]);
	GsClearOt(0,0,&orderingTable[1]);
}

void clearVRAM() {
	RECT rectTL;
	setRECT(&rectTL, 0, 0, 1024, 512);
	ClearImage2(&rectTL, 0, 0, 0);
    DrawSync(0);
    return;
}

void clearDisplay() {
	currentBuffer = GsGetActiveBuff();
	FntFlush(-1);
	GsSetWorkBase((PACKET*)GPUOutputPacket[currentBuffer]);
	GsClearOt(0, 0, &orderingTable[currentBuffer]);
}

void display() {
	currentBuffer = GsGetActiveBuff();
	DrawSync(0);
	VSync(2);
	GsSwapDispBuff();
	GsSortClear(0,0,0, &orderingTable[currentBuffer]);
	GsDrawOt(&orderingTable[currentBuffer]);
}