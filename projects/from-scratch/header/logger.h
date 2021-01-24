#ifndef _logger_h_
#define _logger_h_

#include "gpubase.h"
#include "gridmap.h"
#include "animation.h"

void logger_logCoords(RECT* rect, char* source);
void logger_logBounds(TILE* bounds);
void logger_logBoundsArray(RECT* rect, u_char amount);
void logger_logGameObject(Animation* gobj);
// void logger_logTeleport(Teleport* t);

#endif