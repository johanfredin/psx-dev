#include "header/mapbounds.h"
#include "header/gpubase.h"
#include "header/gridmap.h"

void initLevelOne();
void addCollisionBlocks(CollisionBlocks* cbs, RECT* bounds, u_char amount);
RECT getRect(u_short x, u_short y, u_short w, u_short h);


void mapbounds_init(u_char level, Frame* frames) {
    RECT *tlCoords, *tRCoords, *bLCoords, *bRCoords;    // Main frame bounds
    switch(level) {
        case 1:
             // 0-0
            tlCoords = MEM_CALLOC(8, RECT);
            tlCoords[0] = getRect(112,80,48,40);
            tlCoords[1] = getRect(0,32,16,224);
            tlCoords[2] = getRect(0,0,256,32);
            tlCoords[3] = getRect(36,176,8,8);
            tlCoords[4] = getRect(144,176,112,80);
            tlCoords[5] = getRect(176,64,64,24);
            tlCoords[6] = getRect(48,64,48,24);
            tlCoords[7] = getRect(32,208,80,24);

            // 0-0-i0 (additional)
            // tlAddCooords = MEM_CALLOC(5, RECT);
            // tlAddCooords[0] = getRect(0, 0, 4, 80);
            // tlAddCooords[1] = getRect(4, 64, 28, 16);
            // tlAddCooords[2] = getRect(48, 64, 32, 16);
            // tlAddCooords[3] = getRect(76, 0, 4, 64);
            // tlAddCooords[4] = getRect(4, 0, 72, 4);

             // 0-0 Teleports

            // 1-0
            tRCoords = MEM_CALLOC(6, RECT);
            tRCoords[0] = getRect(240,32,16,224);
            tRCoords[1] = getRect(160,32,48,56);
            tRCoords[2] = getRect(112,48,48,24);
            tRCoords[3] = getRect(0,176,160,80);
            tRCoords[4] = getRect(48,96,96,24);
            tRCoords[5] = getRect(0,0,256,32);
            // 0-1
            bLCoords = MEM_CALLOC(6, RECT);
            bLCoords[0] = getRect(144,0,112,146);
            bLCoords[1] = getRect(0,0,16,240);
            bLCoords[2] = getRect(0,240,128,16);
            bLCoords[3] = getRect(144,240,112,16);
            bLCoords[4] = getRect(36,176,8,12);
            bLCoords[5] = getRect(48,64,48,24);
            // 1-1
            bRCoords = MEM_CALLOC(5, RECT);
            bRCoords[0] = getRect(0,0,160,144);
            bRCoords[1] = getRect(240,0,16,256);
            bRCoords[2] = getRect(192,240,48,16);
            bRCoords[3] = getRect(0,240,176,16);
            bRCoords[4] = getRect(0,176,144,52);
            break;
    }
    addCollisionBlocks(frames[0].cbs, tlCoords, 8);
    addCollisionBlocks(frames[1].cbs, tRCoords, 6);
    addCollisionBlocks(frames[2].cbs, bLCoords, 6);
    addCollisionBlocks(frames[3].cbs, bRCoords, 5);
}

void addCollisionBlocks(CollisionBlocks* cbs, RECT* bounds, u_char amount) {
    printf("--------------------------------------------\n");
    printf("Setting collision bounds from bounds holder.\n");
    printf("--------------------------------------------\n");
    cbs->bounds = bounds;
    cbs->amount = amount;
    printf("Bounds amount=%d\n", cbs->amount);
    logger_logBoundsArray(cbs->bounds, cbs->amount);
    if(DRAW_BOUNDS) {
       TILE* boundLines = MEM_CALLOC(cbs->amount, TILE);
       int i = 0;
       while(i < cbs->amount) {
           TILE bounds;
           SetTile(&bounds);
           bounds.x0 = cbs->bounds[i].x;
           bounds.y0 = cbs->bounds[i].y;
           bounds.w = cbs->bounds[i].w;
           bounds.h = cbs->bounds[i].h;
           setRGB0(&bounds, 255, 0, 0);
           logger_logBounds(&bounds);
           boundLines[i] = bounds;
           i++;
       }
       cbs->boundLines = boundLines;
    }
}

RECT getRect(u_short x, u_short y, u_short w, u_short h) {
    RECT r = {x, y, w, h};
    if(LOG_INDIVIDUAL_BOUNDS) {
        logger_logCoords(&r, "bounds");
    }
    return r;
}


