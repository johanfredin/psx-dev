#include "gpubase.h"

// Declare shapes
POLY_FT4 polyFT4;			// Gouraud shaded rectangle

u_short polycnt = 0;

u_long ot[255];


int main() {
	Color bgcolor = {25, 50, 50};

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
		addPrimes();	
		DrawOTag(ot);
		drawPolys();
		DrawSync(0);											            // wait for all drawing to finish
		VSync(3); 												            // wait for v_blank interrupt
		GsSwapDispBuff(); 										            // flip the double buffers
		GsSortClear(bgcolor.r,bgcolor.g, bgcolor.b, &myOT[CurrentBuffer]);							// clear the ordering table with a background color (R,G,B)
		GsDrawOt(&myOT[CurrentBuffer]); 						            // draw the ordering table
		FntFlush(-1); 											            // refresh the font
	}

	return 0; 
}

void addPrimes() {
	AddPrim(&ot[7], &polyFT4);
}


void initPolys() {
	// Init gouraud rect
	SetPolyG4(&polyFT4);
	polyFT4.tpage = GetTPage(0, 0, 330, 50);
	polyFT4.clut = GetClut(960, 256);
	setRGB0(&polyFT4, 100, 100, 100);
	// setRGB1(&polyFT4, 0, 255, 30);
	// setRGB2(&polyFT4, 0, 0, 255);
	// setRGB3(&polyFT4, 255, 0, 0);
	setXY4(&polyFT4, 40, 200, 80, 200, 40, 240, 80, 240);
	setUV4(&polyFT4, 40/2, 200/2, 80/2, 200/2, 40/2, 240/2, 80/2, 240/2);
	
}

void drawPolys() {
	DrawOTag(ot);
}
