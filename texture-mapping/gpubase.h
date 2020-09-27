#ifndef _gpubase_
#define _gpubase_

#include <stdlib.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>

#define OT_LENGTH 1 				// the ordertable length
#define PACKETMAX 18 				// the maximum number of objects on the screen
#define SCREEN_WIDTH  320 
#define	SCREEN_HEIGHT 240 

GsOT orderingTable[2];							// ordering table header
GsOT_TAG orderingTable_TAG[2][1<<OT_LENGTH]; 	// ordering table unit
PACKET GPUPacketArea[2][PACKETMAX]; 	// GPU packet data

u_long _ramsize   = 0x00200000; 		// force 2 megabytes of RAM
u_long _stacksize = 0x00004000; 		// force 16 kilobytes of stack

short currentBuffer = 0; 				// holds the current buffer number

typedef struct Color {
    u_short r;
    u_short g;
    u_short b;
} Color;

typedef struct {
	short x;
	short y;
} Vec2;

Color* bgcolor;

void initGraphics(Color* color) {
    bgcolor = color;
	SetVideoMode(0); // 1=PAL, 0=NTSC 
	
	GsInitGraph(SCREEN_WIDTH, SCREEN_HEIGHT, GsINTER|GsOFSGPU, 1, 0); // set the graphics mode resolutions (GsNONINTER for NTSC, and GsINTER for PAL)
	GsDefDispBuff(0, 0, 0, SCREEN_HEIGHT); // tell the GPU to draw from the top left coordinates of the framebuffer
	
	// init the ordertables
	orderingTable[0].length = OT_LENGTH;
	orderingTable[1].length = OT_LENGTH;
	orderingTable[0].org = orderingTable_TAG[0];
	orderingTable[1].org = orderingTable_TAG[1];
	
	// clear the ordertables
	GsClearOt(0,0,&orderingTable[0]);
	GsClearOt(0,0,&orderingTable[1]);
}

void display() {
	FntFlush(-1); 											            // refresh the font
	currentBuffer = GsGetActiveBuff(); 						            // get the current buffer
	GsSetWorkBase((PACKET*)GPUPacketArea[currentBuffer]);	            // setup the packet workbase   
	GsClearOt(0,0, &orderingTable[currentBuffer]); 					            // clear the ordering table
	DrawSync(0);											            // wait for all drawing to finish
	VSync(2); 												            // wait for v_blank interrupt
	GsSwapDispBuff(); 										            // flip the double buffers
	GsSortClear(bgcolor->r,bgcolor->b,bgcolor->b, &orderingTable[currentBuffer]);// clear the ordering table with a background color (R,G,B)
	GsDrawOt(&orderingTable[currentBuffer]); 						            // draw the ordering table
}

void drawSprite(GsSPRITE* sprite) {
	currentBuffer = GsGetActiveBuff();
	GsSortSprite(sprite, &orderingTable[currentBuffer], 0);
}

POLY_F3* createPolyF3(Vec2 xy0, Vec2 xy1, Vec2 xy2, Color rgb0) {
	POLY_F3 polyf3;
	SetPolyF3(&polyf3);
	setRGB0(&polyf3, rgb0.r, rgb0.g, rgb0.b);
	setXY3(&polyf3, xy0.x, xy0.y, xy1.x, xy1.y, xy2.x, xy2.y);
	return &polyf3;
}

Color rgb(u_short r, u_short g, u_short b) {
	Color color = {r,g,b};
	return color;
}

Vec2 vec2(short x, short y) {
	Vec2 vec2 = {x, y};
	return vec2;
}

#endif