#ifndef _MapBoundsHolder_
#define _MapBoundsHolder_

#include "gpubase.h"

typedef struct {
    u_char frameX, frameY, amount;
    RECT* bounds;
} FrameCoords;

typedef struct {
    u_char level;
    FrameCoords* frameCoords;
} MapCoords;

MapCoords* mapCoords;

void setMapCoords(u_char level);
void loadLevelOneFrameBounds();
void addFrameCoordsToMap(RECT* bounds, u_char frameX, u_char frameY, u_char amount, u_char index);
RECT getBounds(u_short x, u_short y, u_short w, u_short h);
void freeMapCoords(u_char level);


void setMapCoords(u_char level) {
    mapCoords = (MapCoords*) malloc3(sizeof(MapCoords));
    switch(level) {
        case 1:
            loadLevelOneFrameBounds();
            break;
    }
    mapCoords->level = level;
}

void loadLevelOneFrameBounds() {
    RECT* tlCoords;
    RECT* tRCoords; 
    RECT* bLCoords; 
    RECT* bRCoords;
    
    // 0-0
    tlCoords = (RECT*) malloc3(sizeof(RECT) * 8); 
    tlCoords[0] = getBounds(112,80,48,40);
    tlCoords[1] = getBounds(0,32,16,224);
    tlCoords[2] = getBounds(0,0,256,32);
    tlCoords[3] = getBounds(36,176,8,8);
    tlCoords[4] = getBounds(144,176,112,80);
    tlCoords[5] = getBounds(176,64,64,24);
    tlCoords[6] = getBounds(48,64,48,24);
    tlCoords[7] = getBounds(32,208,80,24);

    // 1-0
    tRCoords = (RECT*) malloc3(sizeof(RECT) * 6); 
    tRCoords[0] = getBounds(240,32,16,224);
    tRCoords[1] = getBounds(160,32,48,56);
    tRCoords[2] = getBounds(112,48,48,24);
    tRCoords[3] = getBounds(0,176,160,80);
    tRCoords[4] = getBounds(48,96,96,24);
    tRCoords[5] = getBounds(0,0,256,32);
    
    // 0-1
    bLCoords = (RECT*) malloc3(sizeof(RECT) * 6); 
    bLCoords[0] = getBounds(144,0,112,146);
    bLCoords[1] = getBounds(0,0,16,240);
    bLCoords[2] = getBounds(0,240,128,16);
    bLCoords[3] = getBounds(144,240,112,16);
    bLCoords[4] = getBounds(36,176,8,12);
    bLCoords[5] = getBounds(48,64,48,24);
    // 1-1
    bRCoords = (RECT*) malloc3(sizeof(RECT) * 5); 
    bRCoords[0] = getBounds(0,0,160,144);
    bRCoords[1] = getBounds(240,0,16,256);
    bRCoords[2] = getBounds(192,240,48,16);
    bRCoords[3] = getBounds(0,240,176,16);
    bRCoords[4] = getBounds(0,176,144,52);

    addFrameCoordsToMap(tlCoords, 0, 0, 8, 0);
    addFrameCoordsToMap(tRCoords, 1, 0, 6, 1);
    addFrameCoordsToMap(bLCoords, 0, 1, 6, 2);
    addFrameCoordsToMap(bRCoords, 1, 1, 5, 3);
}

void addFrameCoordsToMap(RECT* bounds, u_char frameX, u_char frameY, u_char amount, u_char index) {
    FrameCoords* frameCoords = (FrameCoords*) malloc3(sizeof(FrameCoords));
    frameCoords->bounds = bounds;
    frameCoords->frameX = frameX;
    frameCoords->frameY = frameY;
    frameCoords->amount = amount;
    mapCoords->frameCoords[index] = *frameCoords;
}

RECT getBounds(u_short x, u_short y, u_short w, u_short h) {
    RECT r = {x, y, w, h};
    printf("Bounds added: {x:%d, y:%d, w:%d, h:%d}\n", x, y, w, h);
    return r;
}


void freeMapCoords(u_char level) {

}


#endif
