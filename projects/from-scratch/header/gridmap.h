#ifndef _gridmap_
#define _gridmap_

#include "assetmanager.h"
#include "logger.h"
#include "mapbounds.h"

#define NUM_X_FRAMES 2
#define NUM_Y_FRAMES 2

typedef struct {
    // The actual physical bounds that we will collide with
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

Frame* map[NUM_X_FRAMES][NUM_Y_FRAMES];
u_char currXFrame;
u_char currYFrame;

Frame* gridmap_initFrame(u_long* bgSprite, u_long* fgSprite, u_char xIdx, u_char yIdx, u_char blockIndex);
void gridmap_init(u_long** assets, u_char, u_char, u_char, u_char, u_char, u_char, u_char, u_char);
void gridmap_tick(GsSPRITE*);
void gridmap_draw();

#endif