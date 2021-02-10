#include "header/gridmap.h"

RECT **frameCoords;
Teleport **levelTeleports;
u_char levelBoundsAmount[50];    // Reserve elements for level bounds
u_char levelTeleportsAmount[20]; // Reserve elements for level teleports

int f6XOff = SCREEN_WIDTH / 4;
int f6YOff = SCREEN_HEIGHT / 8;

// Prototypes
// ----------
void initGlobals(u_char isFreeAllocatedMem);
void initArr(u_char size, u_char arr[]);
void addBoundriesToLevel(u_char level, Frame *frames);
void addTeleportsToLevel(u_char level, Frame *frames);
void addTeleportsToFrame(Frame *frame, Teleport *frameTeleports, u_char frameIndex, u_char amount);
void addBoundriesToFrame(CollisionBlocks *cbs, RECT *bounds, u_char amount, u_char frameIndex);
RECT getRect(u_short x, u_short y, u_short w, u_short h);
RECT getRectWO(u_short x, u_short y, u_short w, u_short h, short xOffset, short yOffset);
Teleport getTeleport(RECT origin, short destX, short destY, u_char destFrame);

void mapbounds_init(u_char level, Frame *frames) {
    initGlobals(level > 1);
    addBoundriesToLevel(level, frames);
    addTeleportsToLevel(level, frames);
}

// Set global variables to 0
void initGlobals(u_char isFreeAllocatedMem) {
    if (isFreeAllocatedMem) {
        // TODO: free allocated memory
        // frameCoords;
        // levelTeleports;
    }
    initArr(50, levelBoundsAmount);
    initArr(20, levelTeleportsAmount);
}

void initArr(u_char size, u_char arr[]) {
    int i;
    for (i = 0; i < size; i++) {
        arr[i] = 0;
    }
}

void addBoundriesToLevel(u_char level, Frame *frames) {
    log_i("*******************************************************");
    log_i("*         ADDING BOUNDRIES TO LEVEL NR %d             *", level);
    log_i("*******************************************************");
    switch (level) {
    case 1:
        frameCoords[0] = CALLOC(8, RECT);
        frameCoords[1] = CALLOC(6, RECT);
        frameCoords[2] = CALLOC(6, RECT);
        frameCoords[3] = CALLOC(5, RECT);

        frameCoords[0][0] = getRect(112, 80, 48, 40);
        frameCoords[0][1] = getRect(0, 32, 16, 224);
        frameCoords[0][2] = getRect(0, 0, 256, 32);
        frameCoords[0][3] = getRect(36, 176, 8, 8);
        frameCoords[0][4] = getRect(144, 176, 112, 80);
        frameCoords[0][5] = getRect(176, 64, 64, 24);
        frameCoords[0][6] = getRect(48, 64, 48, 24);
        frameCoords[0][7] = getRect(32, 208, 80, 24);
        frameCoords[1][0] = getRect(144, 0, 112, 144);
        frameCoords[1][1] = getRect(0, 0, 16, 240);
        frameCoords[1][2] = getRect(0, 240, 128, 16);
        frameCoords[1][3] = getRect(144, 240, 112, 16);
        frameCoords[1][4] = getRect(36, 176, 8, 12);
        frameCoords[1][5] = getRect(48, 64, 48, 24);
        frameCoords[2][0] = getRect(240, 32, 16, 224);
        frameCoords[2][1] = getRect(160, 32, 48, 56);
        frameCoords[2][2] = getRect(112, 48, 48, 24);
        frameCoords[2][3] = getRect(0, 176, 160, 80);
        frameCoords[2][4] = getRect(48, 96, 96, 24);
        frameCoords[2][5] = getRect(0, 0, 256, 32);
        frameCoords[3][0] = getRect(0, 0, 160, 144);
        frameCoords[3][1] = getRect(240, 0, 16, 256);
        frameCoords[3][2] = getRect(192, 240, 48, 16);
        frameCoords[3][3] = getRect(0, 240, 176, 16);
        frameCoords[3][4] = getRect(0, 176, 144, 52);

        // 0-0-i0 (additional)
        frameCoords[4] = CALLOC(8, RECT);
        frameCoords[4][0] = getRectWO(96, 60, 16, 20, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4);
        frameCoords[4][1] = getRectWO(80, 112, 44, 16, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4);
        frameCoords[4][2] = getRectWO(4, 112, 44, 16, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4);
        frameCoords[4][3] = getRectWO(16, 48, 32, 32, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4);
        frameCoords[4][4] = getRectWO(4, 16, 44, 16, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4);
        frameCoords[4][5] = getRectWO(4, 0, 120, 16, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4);
        frameCoords[4][6] = getRectWO(128, 0, 4, 128, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4);
        frameCoords[4][7] = getRectWO(-4, 0, 4, 128, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4);

        frameCoords[5] = CALLOC(6, RECT);
        frameCoords[5][0] = getRectWO(84, 32, 8, 8, 0, SCREEN_HEIGHT / 3);
        frameCoords[5][1] = getRectWO(180, 32, 8, 8, 0, SCREEN_HEIGHT / 3);
        frameCoords[5][2] = getRectWO(252, 48, 4, 12, 0, SCREEN_HEIGHT / 3);
        frameCoords[5][3] = getRectWO(0, 48, 4, 12, 0, SCREEN_HEIGHT / 3);
        frameCoords[5][4] = getRectWO(0, 60, 256, 4, 0, SCREEN_HEIGHT / 3);
        frameCoords[5][5] = getRectWO(0, 20, 256, 4, 0, SCREEN_HEIGHT / 3);

        frameCoords[6] = CALLOC(8, RECT);
        frameCoords[6][0] = getRectWO(16, 100, 16, 4, f6XOff, f6YOff);
        frameCoords[6][1] = getRectWO(20, 80, 8, 8, f6XOff, f6YOff);
        frameCoords[6][2] = getRectWO(80, 16, 32, 16, f6XOff, f6YOff);
        frameCoords[6][3] = getRectWO(80, 176, 48, 16, f6XOff, f6YOff);
        frameCoords[6][4] = getRectWO(4, 176, 44, 16, f6XOff, f6YOff);
        frameCoords[6][5] = getRectWO(4, 0, 120, 16, f6XOff, f6YOff);
        frameCoords[6][6] = getRectWO(124, 0, 4, 176, f6XOff, f6YOff);
        frameCoords[6][7] = getRectWO(0, 0, 4, 192, f6XOff, f6YOff);

        addBoundriesToFrame(frames[0].cbs, frameCoords[0], 8, 0);
        addBoundriesToFrame(frames[1].cbs, frameCoords[1], 6, 1);
        addBoundriesToFrame(frames[2].cbs, frameCoords[2], 6, 2);
        addBoundriesToFrame(frames[3].cbs, frameCoords[3], 5, 3);
        addBoundriesToFrame(frames[4].cbs, frameCoords[4], 8, 4);
        addBoundriesToFrame(frames[5].cbs, frameCoords[5], 6, 5);
        addBoundriesToFrame(frames[6].cbs, frameCoords[6], 8, 6);
        break;
    }
}

void addTeleportsToLevel(u_char level, Frame *frames) {
    log_i("*******************************************************");
    log_i("*         ADDING TELEPORTS TO LEVEL NR %d             *", level);
    log_i("*******************************************************");
    switch (level) {
    case 1:
        levelTeleports[0] = CALLOC(3, Teleport);
        levelTeleports[1] = CALLOC(2, Teleport);
        levelTeleports[2] = CALLOC(3, Teleport);
        levelTeleports[3] = CALLOC(2, Teleport);
        levelTeleports[4] = CALLOC(2, Teleport);
        levelTeleports[5] = CALLOC(2, Teleport);
        levelTeleports[6] = CALLOC(2, Teleport);

        levelTeleports[0][0] = getTeleport(getRect(SCREEN_WIDTH - 1, 32, 1, 144), 2, -1, 2);
        levelTeleports[0][1] = getTeleport(getRect(16, SCREEN_HEIGHT - 1, 128, 1), -1, 2, 1);
        levelTeleports[0][2] = getTeleport(getRect(128, 116, 16, 8), -1, (120 - 18) + (SCREEN_HEIGHT / 4), 4);
        levelTeleports[1][0] = getTeleport(getRect(SCREEN_WIDTH - 1, 144, 1, 96), 2, -1, 3);
        levelTeleports[1][1] = getTeleport(getRect(16, 0, 128, 1), -1, (SCREEN_HEIGHT - 2) - 16, 0);
        levelTeleports[2][0] = getTeleport(getRect(160, SCREEN_HEIGHT - 1, 80, 1), -1, 2, 3);
        levelTeleports[2][1] = getTeleport(getRect(0, 32, 1, 144), (SCREEN_WIDTH - 2) - 16, -1, 0);
        levelTeleports[2][2] = getTeleport(getRect(176, 84, 16, 8), 48 + f6XOff, 156 + f6YOff, 6);

        levelTeleports[3][0] = getTeleport(getRect(160, 0, 80, 1), -1, (SCREEN_HEIGHT - 2) - 16, 2);
        levelTeleports[3][1] = getTeleport(getRect(0, 144, 1, 96), (SCREEN_WIDTH - 2) - 16, -1, 1);
        levelTeleports[4][0] = getTeleport(getRectWO(48, 120, 32, 8, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4), 128, 126, 0);
        levelTeleports[4][1] = getTeleport(getRectWO(112, 16, 12, 16, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4), 15, 24 + (SCREEN_HEIGHT / 3), 5);
        levelTeleports[5][0] = getTeleport(getRectWO(252, 24, 4, 24, 0, SCREEN_HEIGHT / 3), 32 + f6XOff, 16 + f6YOff, 6);
        levelTeleports[5][1] = getTeleport(getRectWO(0, 24, 4, 24, 0, SCREEN_HEIGHT / 3), 92 + SCREEN_WIDTH / 4, 16 + SCREEN_HEIGHT / 4, 4);

        levelTeleports[6][0] = getTeleport(getRectWO(4, 16, 12, 16, f6XOff, f6YOff), 222, 24 + SCREEN_HEIGHT / 3, 5);
        levelTeleports[6][1] = getTeleport(getRectWO(48, 184, 32, 1, f6XOff, f6YOff), 178, 106, 2);

        addTeleportsToFrame(&frames[0], levelTeleports[0], 0, 3);
        addTeleportsToFrame(&frames[1], levelTeleports[1], 1, 2);
        addTeleportsToFrame(&frames[2], levelTeleports[2], 2, 3);
        addTeleportsToFrame(&frames[3], levelTeleports[3], 3, 2);
        addTeleportsToFrame(&frames[4], levelTeleports[4], 4, 2);
        addTeleportsToFrame(&frames[5], levelTeleports[5], 5, 2);
        addTeleportsToFrame(&frames[6], levelTeleports[6], 6, 2);
        break;
    }
}

void addTeleportsToFrame(Frame *frame, Teleport *frameTeleports, u_char frameIndex, u_char amount) {
    u_char i;
    if (amount > 0) {
        log_d("-----------------------------------------------");
        log_d("Adding bound lines for teleports at frame nr %d", frameIndex);
        log_d("-----------------------------------------------");
        frame->teleports = frameTeleports;
        frame->t_amount = amount;
        LOG_TELEPORTS(frameTeleports, frame->t_amount, i);
        if (DRAW_BOUNDS) {
            while (i < amount) {
                TILE bounds;
                SetTile(&bounds);
                bounds.x0 = frameTeleports[i].origin.x;
                bounds.y0 = frameTeleports[i].origin.y;
                bounds.w = frameTeleports[i].origin.w;
                bounds.h = frameTeleports[i].origin.h;
                setRGB0(&bounds, 0, 0, 255);
                frameTeleports[i].boundLines = bounds;
                LOG_TELEPORT(&frameTeleports[i]);
                LOG_TILE(&bounds);
                i++;
            }
        }
    }
    log_i("Added %d frameTeleports to frame nr %d", amount, frameIndex);
}

void addBoundriesToFrame(CollisionBlocks *cbs, RECT *bounds, u_char amount, u_char frameIndex) {
    u_char i;
    log_d("--------------------------------------");
    log_d("Setting collision bounds for frame nr %d", frameIndex);
    log_d("--------------------------------------");
    cbs->bounds = bounds;
    cbs->amount = amount;
    log_tr("Bounds amount=%d", cbs->amount);
    LOG_RECTS(cbs->bounds, cbs->amount, i);
    if (DRAW_BOUNDS) {
        TILE *boundLines = CALLOC(cbs->amount, TILE);
        while (i < cbs->amount) {
            TILE bounds;
            SetTile(&bounds);
            bounds.x0 = cbs->bounds[i].x;
            bounds.y0 = cbs->bounds[i].y;
            bounds.w = cbs->bounds[i].w;
            bounds.h = cbs->bounds[i].h;
            setRGB0(&bounds, 255, 0, 0);
            LOG_TILE(&bounds);
            boundLines[i] = bounds;
            i++;
        }
        cbs->boundLines = boundLines;
    }
    log_i("Added %d bounds to frame nr %d", amount, frameIndex);
}

RECT getRect(u_short x, u_short y, u_short w, u_short h) {
    RECT r = {x, y, w, h};
    LOG_RECT(&r);
    return r;
}

RECT getRectWO(u_short x, u_short y, u_short w, u_short h, short xOffset, short yOffset) {
    RECT r = {x + xOffset, y + yOffset, w, h};
    LOG_RECT(&r);
    return r;
}

Teleport getTeleport(RECT origin, short destX, short destY, u_char destFrame) {
    Teleport t = {origin, destX, destY};
    t.destFrame = destFrame;
    LOG_TELEPORT(&t);
    return t;
}
