#ifndef _MapBoundsHolder_
#define _MapBoundsHolder_

/*
 * Holds boundaries for a map. 
 */

#include "gpubase.h"

/*
 * Struct holding boundaries coordinates for one frame within a map.
 * A grid map consists of 4 frames and each frame has a set of arbitrary
 * boundaries
 */
typedef struct {
    // X index of current frame within grid map
    u_char frameX; 
    // Y index of current frame within grid map
    u_char frameY;
    // The amount of collision blocks within this frame.
    u_char amount;
    // Array of bounds
    RECT* bounds;
} FrameCoords;

/*
 * Struct holding boundaries for an entire map. 
 */
typedef struct {
    // The level we are at
    u_char level;
    // Array of frames in this map (default is 4 frames)
    FrameCoords* frameCoords;
} MapCoords;

// The current level we will be working on. Should be freed and replace with a new level upon switching
MapCoords* mapCoords;

/*
 * Initialize boundaries for a new map
 * @param index of the level we want boundaries for
 */
void mapbounds_init(u_char level);
void mapbounds_initLevelOne();
/*
 * Allocates a new FrameBounds struct.
 * @param bounds the RECT boundaries array for the current frame
 * @param frameX x coords of current map frame
 * @param frameY y coords of current map frame
 * @param amount size of the bounds array.
 * @return the allocated FrameBounds variable.
 */
FrameCoords* mapbounds_addFrameBoundaries(RECT* bounds, u_char frameX, u_char frameY, u_char amount);
RECT mapbounds_getRect(u_short x, u_short y, u_short w, u_short h);
void mapbounds_free(u_char level);


void mapbounds_init(u_char level) {
    mapCoords = (MapCoords*) malloc3(sizeof(MapCoords));
    switch(level) {
        case 1:
            mapbounds_initLevelOne();
            break;
    }
    mapCoords->level = level;
}

void mapbounds_initLevelOne() {
    RECT* tlCoords;
    RECT* tRCoords; 
    RECT* bLCoords; 
    RECT* bRCoords;
    
    // 0-0
    tlCoords = (RECT*) malloc3(sizeof(RECT) * 8); 
    tlCoords[0] = mapbounds_getRect(112,80,48,40);
    tlCoords[1] = mapbounds_getRect(0,32,16,224);
    tlCoords[2] = mapbounds_getRect(0,0,256,32);
    tlCoords[3] = mapbounds_getRect(36,176,8,8);
    tlCoords[4] = mapbounds_getRect(144,176,112,80);
    tlCoords[5] = mapbounds_getRect(176,64,64,24);
    tlCoords[6] = mapbounds_getRect(48,64,48,24);
    tlCoords[7] = mapbounds_getRect(32,208,80,24);

    // 1-0
    tRCoords = (RECT*) malloc3(sizeof(RECT) * 6); 
    tRCoords[0] = mapbounds_getRect(240,32,16,224);
    tRCoords[1] = mapbounds_getRect(160,32,48,56);
    tRCoords[2] = mapbounds_getRect(112,48,48,24);
    tRCoords[3] = mapbounds_getRect(0,176,160,80);
    tRCoords[4] = mapbounds_getRect(48,96,96,24);
    tRCoords[5] = mapbounds_getRect(0,0,256,32);
    
    // 0-1
    bLCoords = (RECT*) malloc3(sizeof(RECT) * 6); 
    bLCoords[0] = mapbounds_getRect(144,0,112,146);
    bLCoords[1] = mapbounds_getRect(0,0,16,240);
    bLCoords[2] = mapbounds_getRect(0,240,128,16);
    bLCoords[3] = mapbounds_getRect(144,240,112,16);
    bLCoords[4] = mapbounds_getRect(36,176,8,12);
    bLCoords[5] = mapbounds_getRect(48,64,48,24);
    // 1-1
    bRCoords = (RECT*) malloc3(sizeof(RECT) * 5); 
    bRCoords[0] = mapbounds_getRect(0,0,160,144);
    bRCoords[1] = mapbounds_getRect(240,0,16,256);
    bRCoords[2] = mapbounds_getRect(192,240,48,16);
    bRCoords[3] = mapbounds_getRect(0,240,176,16);
    bRCoords[4] = mapbounds_getRect(0,176,144,52);

    mapCoords->frameCoords[0] = *mapbounds_addFrameBoundaries(tlCoords, 0, 0, 8);
    mapCoords->frameCoords[1] = *mapbounds_addFrameBoundaries(tRCoords, 1, 0, 6);
    mapCoords->frameCoords[2] = *mapbounds_addFrameBoundaries(bLCoords, 0, 1, 6);
    mapCoords->frameCoords[3] = *mapbounds_addFrameBoundaries(bRCoords, 1, 1, 5);
}

FrameCoords* mapbounds_addFrameBoundaries(RECT* bounds, u_char frameX, u_char frameY, u_char amount) {
    FrameCoords* frameCoords = (FrameCoords*) malloc3(sizeof(FrameCoords));
    frameCoords->bounds = bounds;
    frameCoords->frameX = frameX;
    frameCoords->frameY = frameY;
    frameCoords->amount = amount;
    return frameCoords;
}

RECT mapbounds_getRect(u_short x, u_short y, u_short w, u_short h) {
    RECT r = {x, y, w, h};
    printf("Bounds added: {x:%d, y:%d, w:%d, h:%d}\n", x, y, w, h);
    return r;
}


void mapbounds_free(u_char level) {
    free3(mapCoords->frameCoords->bounds);
}


#endif
