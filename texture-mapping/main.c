#include "gpubase.h"
#include "images.h"

// Declare shapes
GsSPRITE sprite;
GsIMAGE timData;

int main() {
	Color bgcolor = {25, 50, 50};

	initGraphics(&bgcolor);
	initPolys();
	FntLoad(960, 256); 								// load the font from the BIOS into the framebuffer
	SetDumpFnt(FntOpen(5, 20, 320, 240, 0, 512)); 	// screen X,Y | max text length X,Y | autmatic background clear 0,1 | max characters
	

	while (1) {
		FntPrint("tim.px=%d\ntim.cy=%d", sprite.x, sprite.y);
		display();
		drawSprite(&sprite);
								
	}

	return 0; 
}

/*
typedef struct {
	unsigned long pmode;
	short   px, py;
	unsigned short pw, ph;
	unsigned long *pixel;
	short   cx, cy;
	unsigned short cw, ch;
	unsigned long *clut;
}       GsIMAGE;
*/

void initPolys() {
	// Declare data
	RECT tex;
	RECT clut;	
	

	// Get tim info from image array
	GsGetTimInfo((u_long*)(img_trees), &timData);

	// Load the image into the frame buffer
	tex.x = timData.px;
	tex.y = timData.py;
	tex.w = timData.pw;
	tex.h = timData.ph;
	LoadImage(&tex, timData.pixel);	// Load the pixel data from the image into the rectangle (ish)

	// load the CLUT into the frame buffer
	clut.x = timData.cx;
	clut.y = timData.cy;
	clut.w = timData.cw;
	clut.h = timData.ch;
	LoadImage(&clut, timData.clut);

	// Init sprite
	sprite.attribute = 0x1000000;
	sprite.x = 0;
	sprite.y = 0;
	sprite.w = timData.pw * 2;
	sprite.h = timData.ph;
	sprite.tpage = GetTPage(1, 1, timData.px, timData.py);

	sprite.r = 128;
	sprite.g = 128;
	sprite.b = 128;
	sprite.u = (timData.px - 320) * 2;
	sprite.v = timData.py;
	sprite.cx = timData.cx;
	sprite.cy = timData.cy;

}

