#ifndef _gridmap_
#define _gridmap_

#include "base.h"
#include "assetmanager.h"
#include "logger.h"
#include "cdrom.h"

#include "player.h"

#define NUM_X_FRAMES 2
#define NUM_Y_FRAMES 2

u_char gridmap_currXFrame;
u_char gridmap_currYFrame;

typedef struct {
    u_char x, y;
} Point;

typedef struct {
    Point* origin;
    Point* destination;
} Teleport;

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
    Teleport* teleports;
} Frame;

/* 
 * Main struct. 
 * Holds the 4 main frames as well as optional additional frames (e.g for houses etc)
 */
typedef struct {
    u_char level;
    Frame* mainFrames[NUM_X_FRAMES][NUM_Y_FRAMES];
    Frame* additionalFrames;
} GridMap;

void gridmap_init(u_char level, u_char tLBgIdx, u_char tLFgIdx, u_char tRBgIdx, u_char tRFgIdx, u_char bLBgIdx, u_char bLFgIdx, u_char bRBgIdx, u_char bRFgIdx);
void gridmap_tick(Player*);
void gridmap_draw();

#endif