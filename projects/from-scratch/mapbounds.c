#include "header/mapbounds.h"
#include "header/gpubase.h"
#include "header/gridmap.h"

void addTeleports(Frame* frame, Teleport* frameTeleports, u_char i, u_char amount);
void addCollisionBlocks(CollisionBlocks* cbs, RECT* bounds, u_char amount);
RECT getRect(u_short x, u_short y, u_short w, u_short h);
RECT* getRectPtr(u_short x, u_short y, u_short w, u_short h);
void printTeleports(Teleport* teleports, u_char amount);
Teleport getTeleport(RECT origin, short destX, short destY);
void updateCounts(u_char* frameCount, u_char* levelBoundsAmounts, u_char frameBoundsAmount);

void mapbounds_init(u_char level, Frame* frames) {
    RECT** frameCoords;
    Teleport** levelTeleports;
    u_char levelBoundsAmounts[50];      // Reserve elements for level bounds
    u_char levelTeleportsAmount[20];    // Reserve elements for level teleports
    u_char frameCount = 0;
    int i;

    switch(level) {
        case 1:
            // 0-0

            // ----
            // Teleports
            levelTeleports[0] = CALLOC(30, Teleport);
            levelTeleports[0][0] = getTeleport(getRect(252, 32, 4, 144), 5, -1);
            levelTeleports[0][1] = getTeleport(getRect(16, 252, 128, 4), 5, -1);
            levelTeleports[0][2] = getTeleport(getRect(128, 116, 16, 12), 25, 125);
            levelTeleportsAmount[frameCount] = 3;

            // Frames
            frameCoords[0] = CALLOC(8, RECT);
            frameCoords[0][0] = getRect(112,80,48,40);
            frameCoords[0][1] = getRect(0,32,16,224);
            frameCoords[0][2] = getRect(0,0,256,32);
            frameCoords[0][3] = getRect(36,176,8,8);
            frameCoords[0][4] = getRect(144,176,112,80);
            frameCoords[0][5] = getRect(176,64,64,24);
            frameCoords[0][6] = getRect(48,64,48,24);
            frameCoords[0][7] = getRect(32,208,80,24);
            updateCounts(&frameCount, levelBoundsAmounts, 8);

            // 1-0
            frameCoords[1] = CALLOC(6, RECT);
            frameCoords[1][0] = getRect(240,32,16,224);
            frameCoords[1][1] = getRect(160,32,48,56);
            frameCoords[1][2] = getRect(112,48,48,24);
            frameCoords[1][3] = getRect(0,176,160,80);
            frameCoords[1][4] = getRect(48,96,96,24);
            frameCoords[1][5] = getRect(0,0,256,32);
            updateCounts(&frameCount, levelBoundsAmounts, 6);

            // 0-1
            frameCoords[2] = CALLOC(6, RECT);
            frameCoords[2][0] = getRect(144,0,112,146);
            frameCoords[2][1] = getRect(0,0,16,240);
            frameCoords[2][2] = getRect(0,240,128,16);
            frameCoords[2][3] = getRect(144,240,112,16);
            frameCoords[2][4] = getRect(36,176,8,12);
            frameCoords[2][5] = getRect(48,64,48,24);
            updateCounts(&frameCount, levelBoundsAmounts, 6);

            // 1-1
            frameCoords[3] = CALLOC(5, RECT);
            frameCoords[3][0] = getRect(0,0,160,144);
            frameCoords[3][1] = getRect(240,0,16,256);
            frameCoords[3][2] = getRect(192,240,48,16);
            frameCoords[3][3] = getRect(0,240,176,16);
            frameCoords[3][4] = getRect(0,176,144,52);
            updateCounts(&frameCount, levelBoundsAmounts, 5);

             // 0-0-i0 (additional)
            // tlAddCooords = CALLOC(5, RECT);
            // tlAddCooords[0] = getRect(0, 0, 4, 80);
            // tlAddCooords[1] = getRect(4, 64, 28, 16);
            // tlAddCooords[2] = getRect(48, 64, 32, 16);
            // tlAddCooords[3] = getRect(76, 0, 4, 64);
            // tlAddCooords[4] = getRect(4, 0, 72, 4);
            break;
    }

    for(i = 0; i < frameCount; i++) {
        Frame* f = &frames[i];
        if(levelTeleports[i] != NULL) {
            addTeleports(f, levelTeleports[i], i, levelTeleportsAmount[i]);
        }
        addCollisionBlocks(f->cbs, frameCoords[i], levelBoundsAmounts[i]);
    }
    
}

void printTeleports(Teleport* teleports, u_char amount) {
    int k = 0;
    while(k < amount) {
        Teleport* t = &teleports[k];
        printf("Frame teleport: origin={x:%d, y:%d, w:%d, h:%d}, destX=%d, destY=%d\n", t->origin.x, t->origin.y, t->origin.w, t->origin.h, t->destX, t->destY);
        k++;
    }
}

void printTeleport(Teleport t) {
    printf("Frame teleport: origin={x:%d, y:%d, w:%d, h:%d}, destX=%d, destY=%d\n", t.origin.x, t.origin.y, t.origin.w, t.origin.h, t.destX, t.destY);
}

void addTeleports(Frame* frame, Teleport* frameTeleports, u_char i, u_char amount) {
    if(amount > 0) {
        frame->teleports = frameTeleports;
        if(DRAW_BOUNDS) {
            int i = 0;
            printf("Adding bound lines for teleport at frame nr %d\n", i);
            printf("----------------------------------------------\n");
            while(i < amount) {
                TILE bounds;
                SetTile(&bounds);
                bounds.x0 = frameTeleports[i].origin.x;
                bounds.y0 = frameTeleports[i].origin.y;
                bounds.w = frameTeleports[i].origin.w;
                bounds.h = frameTeleports[i].origin.h;
                setRGB0(&bounds, 0, 0, 255);
                frameTeleports[i].boundLines = bounds;
                logger_logBounds(&frameTeleports[i].boundLines);
                i++;
            }
        }
    }
    frame->t_amount = amount;
    printf("Added %d frameTeleports to frame nr %d\n", amount, i);
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
       TILE* boundLines = CALLOC(cbs->amount, TILE);
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

RECT* getRectPtr(u_short x, u_short y, u_short w, u_short h) {
    RECT* r = MALLOC(RECT);
    *r = getRect(x, y, w, h);
    return r;
}

Teleport getTeleport(RECT origin, short destX, short destY) {
    Teleport t = {origin, destX, destY};
    printf("Teleport added: origin={x:%d, y:%d, w:%d, h:%d}, destX=%d, destY=%d\n", t.origin.x, t.origin.y, t.origin.w, t.origin.h, t.destX, t.destY);
    return t;
}

void updateCounts(u_char* frameCount, u_char* levelBoundsAmounts, u_char frameBoundsAmount) {
    u_char i = *frameCount;
    levelBoundsAmounts[i] = frameBoundsAmount;
    printf("frameCount=%d, levelBoundsAmount[%d]=%d\n", *frameCount, levelBoundsAmounts[i], frameBoundsAmount);
    i++;
    *frameCount = i;
}