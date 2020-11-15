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

void* shapes[8];

int main() {
	Color bgcolor = {25, 0, 50};

	initGraphics(&bgcolor);
	initPolys();
	FntLoad(960, 256); 								// load the font from the BIOS into the framebuffer
	SetDumpFnt(FntOpen(5, 20, 320, 240, 0, 512)); 	// screen X,Y | max text length X,Y | autmatic background clear 0,1 | max characters
	
	while (1) {
		FntPrint("Tiles, Triangles and Rectangles");
		drawPolys();
		display();
	}

	return 0; // this will never be reached because we're in a while loop above
}

void initPolys() {
	// Init poly f3
	SetPolyF3(&flatTriangle);
	setRGB0(&flatTriangle, 255, 132, 45);
	setXY3(&flatTriangle, 10, 50, 50, 50, 10, 90);
	shapes[0] = &flatTriangle;

	// Init gouraud triangle
	SetPolyG3(&gouradTriangle);
	setRGB0(&gouradTriangle, 0, 0, 0);
	setRGB1(&gouradTriangle, 200, 10, 100);
	setRGB2(&gouradTriangle, 100, 110, 10);
	setXY3(&gouradTriangle, 60, 50, 20, 90, 60, 90);
	shapes[1] = &gouradTriangle;

	// Init 1 pixel tile
	SetTile1(&dotTile);
	setXY0(&dotTile, 70, 90);
	setRGB0(&dotTile, 255, 0, 0);
	shapes[2] = &dotTile;

	// Init solid tile
	SetTile(&solidTile);
	setXY0(&solidTile, 90, 90);
	setRGB0(&solidTile, 255, 255, 255);
	setWH(&solidTile, 50, 50);
	shapes[3] = &solidTile;

	// Init tile 8
	SetTile8(&eightPixelTile);
	setXY0(&eightPixelTile, 120, 70);
	setRGB0(&eightPixelTile, 123, 123, 123);
	shapes[4] = &eightPixelTile;

	// Init tile 16
	SetTile16(&sixteenPixelTile);
	setXY0(&sixteenPixelTile, 150, 70);
	setRGB0(&sixteenPixelTile, 211, 50, 55);
	shapes[5] = &sixteenPixelTile;

	// Init flat rect
	SetPolyF4(&flatRect);
	setSemiTrans(&flatRect, 1);
	setRGB0(&flatRect, 213, 12, 45);
	setXY4(&flatRect, 40, 120, 80, 120, 40, 160, 80, 160);
	shapes[6] = &flatRect;

	// Init gouraud rect
	SetPolyG4(&gouraudRect);
	setRGB0(&gouraudRect, 10, 10, 10);
	setRGB1(&gouraudRect, 0, 255, 30);
	setRGB2(&gouraudRect, 0, 0, 255);
	setRGB3(&gouraudRect, 255, 0, 0);
	setXY4(&gouraudRect, 40, 200, 80, 200, 40, 240, 80, 240);
	shapes[7] = &gouraudRect;
}

VECTOR v;

void drawPolys() {
	u_short i = 0;
	while(i < 7) {
		DrawPrim(shapes[i]);	
		i++;
	}
}
