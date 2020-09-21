#include "../gpubase.h"


// Declare shapes
POLY_G4 rect;

typedef struct {
	unsigned short x;
	unsigned short y;
} Vec2;

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
	Vec2 topLeft = {.x = 100, .y=100};
	Vec2 topRight = {.x=200, .y=100};
	Vec2 bottomLeft = {.x=10, .y = 240};
	Vec2 bottomRight = {.x=250, .y = 240};
	initRect(topRight, topRight, bottomRight, bottomRight);
	
}

void initRect(Vec2 topLeft, Vec2 topRight, Vec2 bottomRight, Vec2 bottomLeft) {
	SetPolyG4(&rect);
	setRGB0(&rect, 0, 0, 0);
	setRGB1(&rect, 100, 10, 100);
	setRGB2(&rect, 200, 110, 10);
	setRGB3(&rect, 255, 110, 10);
	setXY4(&rect, topLeft.x, topLeft.y, topRight.x, topRight.y, bottomLeft.x, bottomLeft.y, bottomRight.x, bottomRight.y);
	logVec(topLeft);
}

void logVec(Vec2 vec) {
	FntPrint("x:%d, y%d\n", vec.x, vec.y);
}


void drawPolys() {
	DrawPrim(&rect);
}
