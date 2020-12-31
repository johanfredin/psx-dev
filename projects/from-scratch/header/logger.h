#ifndef _logger_
#define _logger_

#include "gpubase.h"
#include "animatedobject.h"

void logger_logCoords(RECT* rect, char* source);
void logger_logVertices(LINE_F2* lines);
void logger_logBounds(TILE* bounds);
void logger_logBoundsArray(RECT* rect, u_char amount);
void logger_logGameObject(AnimatedGameObject* gobj);

#endif