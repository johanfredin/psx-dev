#ifndef _gridmap_
#define _gridmap_

#include "assetmanager.h"
#include "cdrom.h"
#include "gameobject.h"

typedef struct Teleport {
    RECT origin;
    short destX, destY;
    // Graphical representation of a block (if DebugMode)
    TILE boundLines;
    u_char destFrame; // frame array index of frame to change to when colliding with this teleport
} Teleport;

typedef struct CollisionBlocks {
    // The actual physical bounds that we will collide with in the frame
    RECT *bounds;
    // The amount of blocks on one frame
    u_char amount;
    // Graphical representation of a block (if DebugMode)
    TILE *boundLines;
} CollisionBlocks;

typedef struct Frame {
    u_char t_amount;
    GsSPRITE *bg;
    GsSPRITE *fg;
    CollisionBlocks *cbs;
    Teleport *teleports;
    GameObject *gameObject;
} Frame;

void gridmap_init(u_char level);
void gridmap_tick(Player *);
void gridmap_draw();
void mapbounds_init(u_char level, Frame *frames);

#endif