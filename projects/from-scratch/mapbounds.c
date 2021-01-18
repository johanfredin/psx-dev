#include "header/mapbounds.h"
#include "header/gpubase.h"
#include "header/gridmap.h"

RECT** frameCoords;
Teleport** levelTeleports;
u_char levelBoundsAmount[50];      // Reserve elements for level bounds
u_char levelTeleportsAmount[20];    // Reserve elements for level teleports

// Prototypes
// ----------
void initGlobals(u_char isFreeAllocatedMem);
void initArr(u_char size, u_char arr[]);
void addBoundriesToLevel(u_char level, Frame* frames);
void addTeleportsToLevel(u_char level, Frame* frames);
void addTeleportsToFrame(Frame* frame, Teleport* frameTeleports, u_char frameIndex, u_char amount);
void addBoundriesToFrame(CollisionBlocks* cbs, RECT* bounds, u_char amount);
RECT getRect(u_short x, u_short y, u_short w, u_short h);
Teleport getTeleport(RECT origin, short destX, short destY, u_char destFrame);
void printTeleports(Teleport* teleports, u_char amount);
void printTeleport(Teleport t);

void mapbounds_init(u_char level, Frame* frames) {
    initGlobals(level > 1);
    addBoundriesToLevel(level, frames);
    addTeleportsToLevel(level, frames);
}

// Set global variables to 0
void initGlobals(u_char isFreeAllocatedMem) {
    if(isFreeAllocatedMem) {
        // TODO: free allocated memory  
        // frameCoords;
        // levelTeleports;
    }
    initArr(50, levelBoundsAmount);
    initArr(20, levelTeleportsAmount);
}

void initArr(u_char size, u_char arr[]) {
    int i = 0;
    for(i = 0; i < size; i++) {
        arr[i] = 0;
    }
}

void addBoundriesToLevel(u_char level, Frame* frames) {
    printf("*******************************************************\n");
    printf("*         ADDING BOUNDRIES TO LEVEL NR %d             *\n", level);
    printf("*******************************************************\n");
    switch(level) {
        case 1:
            frameCoords[0] = CALLOC(8, RECT);
            frameCoords[1] = CALLOC(6, RECT);
            frameCoords[2] = CALLOC(6, RECT);
            frameCoords[3] = CALLOC(5, RECT);

            frameCoords[0][0] = getRect(112,80,48,40);
            frameCoords[0][1] = getRect(0,32,16,224);
            frameCoords[0][2] = getRect(0,0,256,32);
            frameCoords[0][3] = getRect(36,176,8,8);
            frameCoords[0][4] = getRect(144,176,112,80);
            frameCoords[0][5] = getRect(176,64,64,24);
            frameCoords[0][6] = getRect(48,64,48,24);
            frameCoords[0][7] = getRect(32,208,80,24);
            frameCoords[1][0] = getRect(144,0,112,144);
            frameCoords[1][1] = getRect(0,0,16,240);
            frameCoords[1][2] = getRect(0,240,128,16);
            frameCoords[1][3] = getRect(144,240,112,16);
            frameCoords[1][4] = getRect(36,176,8,12);
            frameCoords[1][5] = getRect(48,64,48,24);
            frameCoords[2][0] = getRect(240,32,16,224);
            frameCoords[2][1] = getRect(160,32,48,56);
            frameCoords[2][2] = getRect(112,48,48,24);
            frameCoords[2][3] = getRect(0,176,160,80);
            frameCoords[2][4] = getRect(48,96,96,24);
            frameCoords[2][5] = getRect(0,0,256,32);
            frameCoords[3][0] = getRect(0,0,160,144);
            frameCoords[3][1] = getRect(240,0,16,256);
            frameCoords[3][2] = getRect(192,240,48,16);
            frameCoords[3][3] = getRect(0,240,176,16);
            frameCoords[3][4] = getRect(0,176,144,52);

                // 0-0-i0 (additional)
            // tlAddCooords = CALLOC(5, RECT);
            // tlAddCooords[0] = getRect(0, 0, 4, 80);
            // tlAddCooords[1] = getRect(4, 64, 28, 16);
            // tlAddCooords[2] = getRect(48, 64, 32, 16);
            // tlAddCooords[3] = getRect(76, 0, 4, 64);
            // tlAddCooords[4] = getRect(4, 0, 72, 4);

            addBoundriesToFrame(frames[0].cbs, frameCoords[0], 8);
            addBoundriesToFrame(frames[1].cbs, frameCoords[1], 6);
            addBoundriesToFrame(frames[2].cbs, frameCoords[2], 6);
            addBoundriesToFrame(frames[3].cbs, frameCoords[3], 5);
        break;
    }
}

void addTeleportsToLevel(u_char level, Frame* frames) {
    printf("*******************************************************\n");
    printf("*         ADDING TELEPORTS TO LEVEL NR %d             *\n", level);
    printf("*******************************************************\n");
    switch(level) {
        case 1:
            levelTeleports[0] = CALLOC(3, Teleport);
            levelTeleports[1] = CALLOC(2, Teleport);
            levelTeleports[2] = CALLOC(2, Teleport);
            levelTeleports[3] = CALLOC(2, Teleport);
            
            levelTeleports[0][0] = getTeleport(getRect(SCREEN_WIDTH - 1, 32, 1, 144), 2, -1, 2);
            levelTeleports[0][1] = getTeleport(getRect(16, SCREEN_HEIGHT - 1, 128, 1), -1, 2, 1);
            levelTeleports[0][2] = getTeleport(getRect(128, 116, 16, 8), 25, 125, 4);
            levelTeleports[1][0] = getTeleport(getRect(SCREEN_WIDTH - 1, 144, 1, 96), 2, -1, 3);
            levelTeleports[1][1] = getTeleport(getRect(16, 0, 128, 1), -1, (SCREEN_HEIGHT - 2) - 16, 0);
            levelTeleports[2][0] = getTeleport(getRect(160, SCREEN_HEIGHT - 1, 80, 1), -1, 2, 3);
            levelTeleports[2][1] = getTeleport(getRect(0, 32, 1, 144), (SCREEN_WIDTH - 2) - 16, -1, 0);
            levelTeleports[3][0] = getTeleport(getRect(160, 0, 80, 1), -1, (SCREEN_HEIGHT - 2) - 16, 2);
            levelTeleports[3][1] = getTeleport(getRect(0, 144, 1, 96), (SCREEN_WIDTH - 2) - 16, -1, 1);

            addTeleportsToFrame(&frames[0], levelTeleports[0], 0, 3);
            addTeleportsToFrame(&frames[1], levelTeleports[1], 1, 2);
            addTeleportsToFrame(&frames[2], levelTeleports[2], 2, 2);
            addTeleportsToFrame(&frames[3], levelTeleports[3], 3, 2);
        break;
    }
}

void addTeleportsToFrame(Frame* frame, Teleport* frameTeleports, u_char frameIndex, u_char amount) {
    if(amount > 0) {
        printf("----------------------------------------------\n");
        printf("Adding bound lines for teleport at frame nr %d\n", frameIndex);
        printf("----------------------------------------------\n");
        frame->teleports = frameTeleports;
        frame->t_amount = amount;
        printTeleports(frame->teleports, frame->t_amount);
        if(DRAW_BOUNDS) {
            int i = 0;
            while(i < amount) {
                TILE bounds;
                SetTile(&bounds);
                bounds.x0 = frameTeleports[i].origin.x;
                bounds.y0 = frameTeleports[i].origin.y;
                bounds.w = frameTeleports[i].origin.w;
                bounds.h = frameTeleports[i].origin.h;
                setRGB0(&bounds, 0, 0, 255);
                frameTeleports[i].boundLines = bounds;
                printTeleport(frameTeleports[i]);
                logger_logBounds(&frameTeleports[i].boundLines);
                i++;
            }
        }
    }
    printf("Added %d frameTeleports to frame nr %d\n", amount, frameIndex);
}

void addBoundriesToFrame(CollisionBlocks* cbs, RECT* bounds, u_char amount) {
    printf("--------------------------------------------\n");
    printf("Setting collision bounds for frame.\n");
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
    printf("Added %d bounds\n", amount);
}

RECT getRect(u_short x, u_short y, u_short w, u_short h) {
    RECT r = {x, y, w, h};
    if(LOG_INDIVIDUAL_BOUNDS) {
        logger_logCoords(&r, "bounds");
    }
    return r;
}

Teleport getTeleport(RECT origin, short destX, short destY, u_char destFrame) {
    Teleport t = {origin, destX, destY};
    t.destFrame = destFrame;
    if(LOG_INDIVIDUAL_BOUNDS) {
        printf("Teleport added: origin={x:%d, y:%d, w:%d, h:%d}, destX=%d, destY=%d, frameToGoTo=%d\n", t.origin.x, t.origin.y, t.origin.w, t.origin.h, t.destX, t.destY, t.destFrame);
    }
    return t;
}

void printTeleports(Teleport* teleports, u_char amount) {
    int k = 0;
    while(k < amount) {
        Teleport* t = &teleports[k];
        printf("Frame teleport: origin={x:%d, y:%d, w:%d, h:%d}, destX=%d, destY=%d, destFrame=%d\n", t->origin.x, t->origin.y, t->origin.w, t->origin.h, t->destX, t->destY, t->destFrame);
        k++;
    }
}

void printTeleport(Teleport t) {
    printf("Frame teleport: origin={x:%d, y:%d, w:%d, h:%d}, destX=%d, destY=%d\n", t.origin.x, t.origin.y, t.origin.w, t.origin.h, t.destX, t.destY);
}
