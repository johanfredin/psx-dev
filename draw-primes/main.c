#include "gpubase.h"



// Declare shapes
POLY_F3 flatTriangle;  
POLY_FT3 flatTexturedTriange;
POLY_G3 gouradTriangle;
POLY_GT3 gouradTexturedTriangle;



u_short polycnt = 0;

int main() {
	Color bgcolor = {25, 50, 255};

	initGraphics(&bgcolor);
	initPolys();
	FntLoad(960, 256); 								// load the font from the BIOS into the framebuffer
	SetDumpFnt(FntOpen(5, 20, 320, 240, 0, 512)); 	// screen X,Y | max text length X,Y | autmatic background clear 0,1 | max characters
	
	while (1) {
		drawPolys();
		display();
	}

	return 0; // this will never be reached because we're in a while loop above
}

void initPolys() {
	initF3();
	initG3();
	initFt3();
}

void initF3() {
	SetPolyF3(&flatTriangle);
	setPolyF3(&flatTriangle);
	setRGB0(&flatTriangle, 255, 132, 45);
	setXY3(&flatTriangle, 10, 5, 50, 5, 10, 45);
}

void initG3() {
	SetPolyG3(&gouradTriangle);
	setRGB0(&gouradTriangle, 0, 0, 0);
	setRGB1(&gouradTriangle, 200, 10, 100);
	setRGB2(&gouradTriangle, 100, 110, 10);
	setXY3(&gouradTriangle, 60, 5, 20, 45, 60, 45);
}

void initFt3() {

}

void drawPolys() {
	DrawPrim(&gouradTriangle);
	DrawPrim(&flatTriangle);
}
