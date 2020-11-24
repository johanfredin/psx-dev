#ifndef _logger_
#define _logger_

#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>

void logger_logCoords(RECT* rect, char* source);

void logger_logCoords(RECT* rect, char* source) {
    printf("%s coords assigned at {x:%d, y%d, w:%d, h:%d}\n", source, rect->x, rect->y, rect->w, rect->h);
}

#endif