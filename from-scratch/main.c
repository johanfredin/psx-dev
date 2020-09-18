/*
===========================================================
                Sony PlayStation 1 Source Code
===========================================================
                         FONT EXAMPLE
Displays text on the screen using the built in GPU routines
-----------------------------------------------------------

    Developer / Programmer..............: SCEI & PSXDEV.net
    Software Ddevelopment Kit...........: PSY-Q
	Last Updated........................: 04/MAY/2017

    Original code by SCEI | Edited by PSXDEV.net
    
	NOTE: This example uses double buffering.

  Copyright (C) 1994,1995 by Sony Computer Entertainment Inc.
   Sony Computer Entertainment Inc. Development Department.
                   All Rights Reserved.
				   
                    http://psxdev.net/

-----------------------------------------------------------*/

#include <stdlib.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>

#define OT_LENGTH 1 				// the ordertable length
#define PACKETMAX 18 				// the maximum number of objects on the screen
#define SCREEN_WIDTH  320 
#define	SCREEN_HEIGHT 240 

GsOT myOT[2];							// ordering table header
GsOT_TAG myOT_TAG[2][1<<OT_LENGTH]; 	// ordering table unit
PACKET GPUPacketArea[2][PACKETMAX]; 	// GPU packet data

u_long _ramsize   = 0x00200000; 		// force 2 megabytes of RAM
u_long _stacksize = 0x00004000; 		// force 16 kilobytes of stack

short CurrentBuffer = 0; 				// holds the current buffer number

// CUSTOM SHIIIIT ---------
POLY_G3 triangle;
// ------------------------


void initGraphics(); 					// inits the GPU
void display(); 						// updates the GPU (IE: VRAM/SGRAM/framebuffer)

int main() {
	initGraphics();
	initPolys();
	FntLoad(960, 256); 								// load the font from the BIOS into the framebuffer
	SetDumpFnt(FntOpen(5, 20, 320, 240, 0, 512)); 	// screen X,Y | max text length X,Y | autmatic background clear 0,1 | max characters
	
	while (1) {
		FntPrint("Curr buffer=%d", CurrentBuffer);
		drawPolys();
		display();
	}

	return 0; // this will never be reached because we're in a while loop above
}

void initGraphics() {
	SetVideoMode(1); // 1=PAL, 0=NTSC 
	
	GsInitGraph(SCREEN_WIDTH, SCREEN_HEIGHT, GsINTER|GsOFSGPU, 1, 0); // set the graphics mode resolutions (GsNONINTER for NTSC, and GsINTER for PAL)
	GsDefDispBuff(0, 0, 0, SCREEN_HEIGHT); // tell the GPU to draw from the top left coordinates of the framebuffer
	
	// init the ordertables
	myOT[0].length = OT_LENGTH;
	myOT[1].length = OT_LENGTH;
	myOT[0].org = myOT_TAG[0];
	myOT[1].org = myOT_TAG[1];
	
	// clear the ordertables
	GsClearOt(0,0,&myOT[0]);
	GsClearOt(0,0,&myOT[1]);
}

void initPolys() {

	SetPolyG3(&triangle);
	setRGB0(&triangle, 0, 0, 0);
	setRGB1(&triangle, 200, 10, 100);
	setRGB2(&triangle, 100, 110, 10);
}

void display() {
	FntFlush(-1); 											// refresh the font
	CurrentBuffer = GsGetActiveBuff(); 						// get the current buffer
	GsSetWorkBase((PACKET*)GPUPacketArea[CurrentBuffer]);	// setup the packet workbase   
	GsClearOt(0,0, &myOT[CurrentBuffer]); 					// clear the ordering table
	DrawSync(0);											// wait for all drawing to finish
	VSync(0); 												// wait for v_blank interrupt
	GsSwapDispBuff(); 										// flip the double buffers
	GsSortClear(100,50,255, &myOT[CurrentBuffer]); 			// clear the ordering table with a background color (R,G,B)
	GsDrawOt(&myOT[CurrentBuffer]); 						// draw the ordering table
}

short positions[6] = {0,0,0,0,0,0};
void drawPolys() {

	int i;
	for(i = 0; i < 6; i++) {
		if(positions[i] > 255) {
			positions[i] = 50;
		} else {
			positions[i] += 1;
		}
	}
	setXY3(&triangle, positions[0], positions[1], 50, 100, 100, 150);
	FntPrint("%d, %d, %d, %d, %d, %d\n", positions[0], positions[1], positions[2], positions[3], positions[4], positions[5]);
	DrawPrim(&triangle);
}
