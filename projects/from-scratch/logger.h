#ifndef _logger_
#define _logger_

#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>

void logger_logCoords(RECT* rect, char* source);
void logger_logVertices(LINE_F2* lines);
void logger_logBounds(TILE* bounds);

void logger_logCoords(RECT* rect, char* source) {
    printf("%s coords assigned at {x:%d, y%d, w:%d, h:%d}\n", source, rect->x, rect->y, rect->w, rect->h);
}

void logger_logVertices(LINE_F2* lines) {
    u_char i = 0;
    while(i < 4) {
        printf("Line Coords={(%d,%d, (%d,%d))\n", lines[i].x0, lines[i].y0, lines[i].x1, lines[i].y1);
        i++;
    }
    
}
void logger_logBounds(TILE* bounds) {
    printf("TILE bounds={x:%d, y:%d, w:%d, h:%d}\n", bounds->x0, bounds->y0, bounds->w, bounds->h);
}


#endif