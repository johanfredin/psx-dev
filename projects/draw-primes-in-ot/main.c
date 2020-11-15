#include "gpubase.h"

// Declare shapes
POLY_F3 flatTriangle;  			// Flat shaded triangle
POLY_G3 gouradTriangle;			// Gouraud shded triangle

POLY_F4 flatRect;				// Flat shaded rectangle
POLY_G4 gouraudRect;			// Gouraud shaded rectangle

TILE solidTile;					// Tile where width and height can be set
TILE_1 dotTile;					// Fixed 1 pixel size tile
TILE_8 eightPixelTile;			// Tile with fixed w/h of 8 pixels
TILE_16 sixteenPixelTile;		// Tile with fixed w/h of 16 pixels

u_short polycnt = 0;

u_long ot[255];


int main() {
	Color bgcolor = {25, 0, 50};

	initGraphics(&bgcolor);
	initPolys();
	FntLoad(960, 256); 								// load the font from the BIOS into the framebuffer
	SetDumpFnt(FntOpen(5, 20, 320, 240, 0, 512)); 	// screen X,Y | max text length X,Y | autmatic background clear 0,1 | max characters
	

	while (1) {
		FntPrint("Tiles, Triangles and Rectangles\n");
		
		CurrentBuffer = GsGetActiveBuff(); 						            // get the current buffer
		GsSetWorkBase((PACKET*)GPUPacketArea[CurrentBuffer]);	            // setup the packet workbase   
		GsClearOt(0,0, &myOT[CurrentBuffer]); 					            // clear the ordering table
		ClearOTag(ot, 255);
		moveBox();
		addPrimes();	
		DrawOTag(ot);
		drawPolys();
		DrawSync(0);											            // wait for all drawing to finish
		VSync(3); 												            // wait for v_blank interrupt
		GsSwapDispBuff(); 										            // flip the double buffers
		GsSortClear(0,0,0, &myOT[CurrentBuffer]);							// clear the ordering table with a background color (R,G,B)
		GsDrawOt(&myOT[CurrentBuffer]); 						            // draw the ordering table
		FntFlush(-1); 											            // refresh the font
	}

	return 0; 
}

void addPrimes() {
	AddPrim(&ot[0], &flatTriangle);
	AddPrim(&ot[1], &gouradTriangle);
	AddPrim(&ot[2], &dotTile);
	AddPrim(&ot[3], &solidTile);
	AddPrim(&ot[4], &eightPixelTile);
	AddPrim(&ot[5], &sixteenPixelTile);
	AddPrim(&ot[6], &flatRect);
	AddPrim(&ot[7], &gouraudRect);
}


void moveBox() {
	solidTile.x0 += 1;
	solidTile.y0 -= 1;
}

void initPolys() {
	// Init poly f3
	SetPolyF3(&flatTriangle);
	setRGB0(&flatTriangle, 255, 132, 45);
	setXY3(&flatTriangle, 10, 50, 50, 50, 10, 90);
	

	// Init gouraud triangle
	SetPolyG3(&gouradTriangle);
	setRGB0(&gouradTriangle, 0, 0, 0);
	setRGB1(&gouradTriangle, 200, 10, 100);
	setRGB2(&gouradTriangle, 100, 110, 10);
	setXY3(&gouradTriangle, 60, 50, 20, 90, 60, 90);
	

	// Init 1 pixel tile
	SetTile1(&dotTile);
	setXY0(&dotTile, 70, 90);
	setRGB0(&dotTile, 255, 0, 0);
	

	// Init solid tile
	SetTile(&solidTile);
	setXY0(&solidTile, 90, 90);
	setRGB0(&solidTile, 255, 255, 255);
	setWH(&solidTile, 50, 50);

	// Init tile 8
	SetTile8(&eightPixelTile);
	setXY0(&eightPixelTile, 120, 70);
	setRGB0(&eightPixelTile, 123, 123, 123);
	

	// Init tile 16
	SetTile16(&sixteenPixelTile);
	setXY0(&sixteenPixelTile, 150, 70);
	setRGB0(&sixteenPixelTile, 211, 50, 55);


	// Init flat rect
	SetPolyF4(&flatRect);
	setSemiTrans(&flatRect, 1);
	setRGB0(&flatRect, 213, 12, 45);
	setXY4(&flatRect, 40, 120, 80, 120, 40, 160, 80, 160);
	

	// Init gouraud rect
	SetPolyG4(&gouraudRect);
	setRGB0(&gouraudRect, 10, 10, 10);
	setRGB1(&gouraudRect, 0, 255, 30);
	setRGB2(&gouraudRect, 0, 0, 255);
	setRGB3(&gouraudRect, 255, 0, 0);
	setXY4(&gouraudRect, 40, 200, 80, 200, 40, 240, 80, 240);
	
}

void drawPolys() {
	DrawOTag(ot);
}
