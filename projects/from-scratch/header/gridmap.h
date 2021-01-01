#ifndef _gridmap_
#define _gridmap_

#include "assetmanager.h"
#include "logger.h"
#include "mapbounds.h"
#include "base.h"

#define NUM_X_FRAMES 2
#define NUM_Y_FRAMES 2

u_char gridmap_currXFrame;
u_char gridmap_currYFrame;

typedef struct {
    // The actual physical bounds that we will collide with in the frame
    RECT* bounds;
    // The amount of blocks on one frame
    u_char amount;
    // Graphical representation of a block (if DebugMode)
    TILE* boundLines;
} CollisionBlocks;

typedef struct {
    u_char xIdx, yIdx;
    GsSPRITE* bg;
    GsSPRITE* fg;
    CollisionBlocks* cbs;

} Frame;

void gridmap_init(u_long** assets, u_char, u_char, u_char, u_char, u_char, u_char, u_char, u_char);
void gridmap_tick(GsSPRITE*);
void gridmap_draw();

#endif