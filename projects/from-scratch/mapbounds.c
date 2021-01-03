#include "header/mapbounds.h"
#include "header/logger.h"
#include "header/cdrom.h"

void mapbounds_initLevelOne();
RECT mapbounds_getRect(u_short x, u_short y, u_short w, u_short h);

void mapbounds_init(u_char level) {
    mapCoords = MEM_ALLOC(MapCoords);
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
    tlCoords = MEM_CALLOC(8, RECT);
    tlCoords[0] = mapbounds_getRect(112,80,48,40);
    tlCoords[1] = mapbounds_getRect(0,32,16,224);
    tlCoords[2] = mapbounds_getRect(0,0,256,32);
    tlCoords[3] = mapbounds_getRect(36,176,8,8);
    tlCoords[4] = mapbounds_getRect(144,176,112,80);
    tlCoords[5] = mapbounds_getRect(176,64,64,24);
    tlCoords[6] = mapbounds_getRect(48,64,48,24);
    tlCoords[7] = mapbounds_getRect(32,208,80,24);

    // 1-0
    tRCoords = MEM_CALLOC(6, RECT);
    tRCoords[0] = mapbounds_getRect(240,32,16,224);
    tRCoords[1] = mapbounds_getRect(160,32,48,56);
    tRCoords[2] = mapbounds_getRect(112,48,48,24);
    tRCoords[3] = mapbounds_getRect(0,176,160,80);
    tRCoords[4] = mapbounds_getRect(48,96,96,24);
    tRCoords[5] = mapbounds_getRect(0,0,256,32);
    
    // 0-1
    bLCoords = MEM_CALLOC(6, RECT);
    bLCoords[0] = mapbounds_getRect(144,0,112,146);
    bLCoords[1] = mapbounds_getRect(0,0,16,240);
    bLCoords[2] = mapbounds_getRect(0,240,128,16);
    bLCoords[3] = mapbounds_getRect(144,240,112,16);
    bLCoords[4] = mapbounds_getRect(36,176,8,12);
    bLCoords[5] = mapbounds_getRect(48,64,48,24);
    // 1-1
    bRCoords = MEM_CALLOC(5, RECT);
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
    FrameCoords* frameCoords = MEM_ALLOC(FrameCoords);
    frameCoords->bounds = bounds;
    frameCoords->frameX = frameX;
    frameCoords->frameY = frameY;
    frameCoords->amount = amount;
    return frameCoords;
}

RECT mapbounds_getRect(u_short x, u_short y, u_short w, u_short h) {
    RECT r = {x, y, w, h};
    logger_logCoords(&r, "bounds");
    return r;
}


void mapbounds_free(u_char level) {
    MEM_FREE(mapCoords->frameCoords->bounds);
}
