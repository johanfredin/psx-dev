#ifndef _DebugRenderer_
#define _DebugRenderer_

#include "gpubase.h"

/**
 * Bounds and Line structs originated from wituz constants.h
 */

typedef struct {
	LINE_F2 lines[4];
} Bounds;

void createLine(int x1, int y1, int x2, int y2, LINE_F2* line);
void createBox(int x1, int y1, int x2, int y2, Bounds* bounds);
void drawBounds(Bounds* bounds);
void logBounds(Bounds* bounds);

void createLine(int x1, int y1, int x2, int y2, LINE_F2* line) {
	SetLineF2(line);
	setRGB0(line, 255, 0, 0);
	setXY2(line, x1, y1, x2, y2);
}

void createBox(int x1, int y1, int x2, int y2, Bounds* bounds) {
	createLine(x1, y1, x2, y1, &bounds -> lines[0]);
	createLine(x1, y2, x2, y2, &bounds -> lines[1]);
	createLine(x1, y1, x1, y2, &bounds -> lines[2]);
	createLine(x2, y1, x2, y2, &bounds -> lines[3]);
}

void logBounds(Bounds* bounds) {
	printf("BoundLines created {x:%d, y:%d, w:%d, h:%d\n",
		bounds->lines[0].x0, 
		bounds->lines[0].y0, 
		bounds->lines[0].x0 + bounds->lines[0].x1, 
		bounds->lines[0].y0 + bounds->lines[0].y1);
}

void drawBounds(Bounds* bounds) {
	int i = 0;
	while(i < 4) {
		DrawPrim(&bounds->lines[i]);
		i++;
	}
}


#endif