#ifndef _logger_
#define _logger_

#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>

void logger_logCoords(RECT* rect, char* source);
void logger_logVertices(LINE_F4* lineF4);

void logger_logCoords(RECT* rect, char* source) {
    printf("%s coords assigned at {x:%d, y%d, w:%d, h:%d}\n", source, rect->x, rect->y, rect->w, rect->h);
}

void logger_logVertices(LINE_F4* lineF4) {
    printf("Line Coords={(%d,%d, (%d,%d), (%d,%d), (%d,%d))\n", lineF4->x0, lineF4->y0, lineF4->x1, lineF4->y1, lineF4->x2, lineF4->y2, lineF4->x3, lineF4->y3);
}


#endif