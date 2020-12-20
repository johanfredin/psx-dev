#ifndef _GridMapHandler_
#define _GridMapHandler_

#include "assetmanager.h"
#include "logger.h"
#include "MapBoundsHolder.h"
#include "DebugRenderer.h"

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

u_char rightCol, leftCol, topCol, bottomCol;

// Prototypes
// ----------
Frame* initFrame(u_long* bgSprite, u_long* fgSprite, u_char xIdx, u_char yIdx);
RECT getBounds(u_short x, u_short y, u_short w, u_short h);
void setCollisionBounds(CollisionBlocks* cbs);
void initMap(u_long* assets[], u_char, u_char, u_char, u_char, u_char, u_char, u_char, u_char);
void tickMap(GsSPRITE*);
void drawMap();
void handleEdgeCollision(GsSPRITE* player);
void handleBlockCollision(GsSPRITE* player);


void initMap(u_long* assets[], u_char tLBgIdx, u_char tLFgIdx, u_char tRBgIdx, u_char tRFgIdx, u_char bLBgIdx, u_char bLFgIdx, u_char bRBgIdx, u_char bRFgIdx) {
    CollisionBlocks* cbs;
    map[0][0] = initFrame(assets[tLBgIdx], assets[tLFgIdx], 0, 0);
    map[0][1] = initFrame(assets[tRBgIdx], assets[tRFgIdx], 0, 1);
    map[1][0] = initFrame(assets[bLBgIdx], assets[bLFgIdx], 1, 0);
    map[1][1] = initFrame(assets[bRBgIdx], assets[bRFgIdx], 1, 1);

    cbs = (CollisionBlocks*) malloc3(sizeof(CollisionBlocks));
    setCollisionBounds(cbs);
    map[0][0]->cbs = cbs;
}

Frame* initFrame(u_long* bgSprite, u_long* fgSprite, u_char xIdx, u_char yIdx) {
    Frame* frame = (Frame*) malloc3(sizeof(Frame));
    frame->bg = assetmanager_loadSprite("bg", bgSprite, 0, 0, 128, COLOR_BITS_8);
    frame->fg = assetmanager_loadSprite("fg", fgSprite, 0, 0, 128, COLOR_BITS_8);
    frame->xIdx = xIdx;
    frame->yIdx = yIdx;
    return frame;
}

void setCollisionBounds(CollisionBlocks* cbs) {
    RECT* blocks = (RECT*) malloc3(sizeof(RECT) * 8); 
    blocks[0] = getBounds(0,32,16,224);
    blocks[1] = getBounds(0,0,256,32);
    blocks[2] = getBounds(36,176,8,16);
    blocks[3] = getBounds(144,176,112,80);
    blocks[4] = getBounds(176,64, 64, 24);
    blocks[5] = getBounds(48,64,48,24);
    blocks[6] = getBounds(32,208,80,24);
    blocks[7] = getBounds(112,80,48,432);
    cbs->amount = 8;
    cbs->bounds = blocks;
     if(DrawBounds) {
        int i = 0;
        TILE* boundLines = (TILE*) malloc3(sizeof(TILE) * cbs->amount);
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

void drawMap() {
    CollisionBlocks* blocks = map[currXFrame][currYFrame]->cbs;
    GsSortFastSprite(map[currXFrame][currYFrame]->fg, currentOT(), 0);
    GsSortFastSprite(map[currXFrame][currYFrame]->bg, currentOT(), 2);
    if(DrawBounds) {
        int blockIdx = 0;
        while(blockIdx < blocks->amount) {
            DrawPrim(&blocks->boundLines[blockIdx]);
            blockIdx++;
        }
    }
    if(PrintCoords) {
        FntPrint("Blocks in frame=%d\n", blocks->amount);
        FntPrint("tc=%d, bc=%d, lc=%d, rc=%d\n", topCol, bottomCol, leftCol, rightCol);
    }
}

RECT getBounds(u_short x, u_short y, u_short w, u_short h) {
    RECT r = {x, y, w, h};
    printf("Bounds added: {x:%d, y:%d, w:%d, h:%d}\n", x, y, w, h);
    return r;
}

void tickMap(GsSPRITE* player) {
    handleEdgeCollision(player);
    handleBlockCollision(player);
}

void handleEdgeCollision(GsSPRITE* player) {
    // X bounds -------------------------------------
    if(player->x < 0) {
         if(currXFrame > 0) {
            player->x = screenWidth - player->w;
            currXFrame--;
         } else {
             player->x = 0;
         }
    } 
    if(player->x + player->w > screenWidth) {
        if(currXFrame < 1) {
            player->x = 0;
            currXFrame++;
        } else {
            player->x = screenWidth - player->w;
        }
    } 

    // Y bounds -------------------------------------
    if(player->y < 0) {
        if(currYFrame > 0) {
            player->y = screenHeight - player->h;
            currYFrame--;
        } else {
            player->y = 0;
        }
    } 
    if(player->y + player->h > screenHeight) {
        if(currYFrame < 1) {
            player->y = 0;
            currYFrame++;
        } else {
            player->y = screenHeight - player->h;
        }
    }
}

void handleBlockCollision(GsSPRITE* player) {
    CollisionBlocks* blocks = map[currXFrame][currYFrame]->cbs;
    int i = 0;
    
    // Player bounds
    short px = player->x;
    short py = player->y;
    u_short pw = player->w;
    u_short ph = player->h;
    u_short pxw = px + pw;
    u_short pyh = py + ph;

    while (i < blocks->amount) {
        // Collision block bounds
        RECT* bounds = &blocks->bounds[i];
        short bx = bounds->x;
        short by = bounds->y;
        u_short bw = bounds->w;
        u_short bh = bounds->h;
        u_short bxw = bx + bw;
        u_short byh = by + bh;

        rightCol = pxw >= bx && px <= bx && pyh > by && py < byh;
        leftCol = px <= bxw && pxw > bxw && pyh > by && py < byh;
        topCol = pyh >= by && py < by && pxw > bx && px < bxw;
        bottomCol = py <= byh && pyh > byh && pxw > bx && px < bxw;

        if(rightCol) {
            player->x = bx - pw;
        } 
        if(leftCol) {
            player->x = bxw;
        }
        if(topCol) {
            player->y = by - ph;
        }
        if(bottomCol) {
            player->y = byh;
        }

        i++;
    }   
}

#endif