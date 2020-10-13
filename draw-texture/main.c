#include "gpubase.h"
#include "images.h"

// Declare shapes
POLY_F4 flatRect;				// Flat shaded rectangle
POLY_G4 gouraudRect;			// Gouraud shaded rectangle

GsSPRITE gsSprite;
GsIMAGE gs_image;
RECT rect;
RECT crect;

int main() {
	Color bgcolor = {25, 0, 50};

	initPolys();
	FntLoad(960, 256); 								// load the font from the BIOS into the framebuffer
	SetDumpFnt(FntOpen(5, 20, 320, 240, 0, 512)); 	// screen X,Y | max text length X,Y | autmatic background clear 0,1 | max characters

	while (1) {
		FntPrint("x:%d, y:%d, w:%d, h:%d", gsSprite.x, gsSprite.y, gsSprite.w, gsSprite.h);
		drawPolys();
		drawSprites();
		display();
		clearDisplay();		
	}

	return 0; 
}

void initPolys() {

	// Load the image into the frame buffer
	rect.x = gs_image.px;
	rect.y = gs_image.py;
	rect.w = gs_image.pw, 
	rect.h = gs_image.ph; 
	GsGetTimInfo((u_long*)img_raichu+4, &gs_image);
	LoadImage(&rect, gs_image.pixel); // Load the gs_image pixel data into the rect

	// Load the clut into the frame buffer
	crect.x = gs_image.cx;
	crect.y = gs_image.cy;
	crect.w = gs_image.cw;
	crect.h = gs_image.ch;
	LoadImage(&crect, gs_image.clut); // Load the gs_image clut data into rect

	// Init out sprite
	gsSprite.attribute = 0x1000000; // 8-bit
	gsSprite.x = 50;
	gsSprite.y = 50;
	gsSprite.w = gs_image.pw * 2;
	gsSprite.h = gs_image.ph;

	// Set texture page of sprite
	gsSprite.tpage = GetTPage(1, 1, gs_image.px, gs_image.py);

	gsSprite.r = 128;
	gsSprite.g = 128;
	gsSprite.b = 128;
	gsSprite.u = (gs_image.px - 320) * 2;
	gsSprite.v = gs_image.py;
	gsSprite.cx = gs_image.cx;
	gsSprite.cy = gs_image.cy;
	gsSprite.mx = 0;
	gsSprite.my = 0;
	gsSprite.scalex = ONE;
	gsSprite.scaley = ONE;
	gsSprite.rotate = 0;

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
	DrawPrim(&flatRect);
	DrawPrim(&gouraudRect);
}

void drawSprites() {
	currentBuffer = GsGetActiveBuff();
	GsSortSprite(&gsSprite, &orderingTable[currentBuffer], 20);
}
