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
Frame* initFrame(u_long* bgSprite, u_long* fgSprite, u_char xIdx, u_char yIdx, u_char blockIndex);
void setCollisionBounds(CollisionBlocks* cbs, u_char blockIndex);
void initMap(u_long* assets[], u_char, u_char, u_char, u_char, u_char, u_char, u_char, u_char);
void tickMap(GsSPRITE*);
void drawMap();
void handleEdgeCollision(GsSPRITE* sprite);
void handleBlockCollision(GsSPRITE* sprite);


void initMap(u_long* assets[], u_char tLBgIdx, u_char tLFgIdx, u_char tRBgIdx, u_char tRFgIdx, u_char bLBgIdx, u_char bLFgIdx, u_char bRBgIdx, u_char bRFgIdx) {
    setMapCoords(1);
    map[0][0] = initFrame(assets[tLBgIdx], assets[tLFgIdx], 0, 0, 0);
    map[1][0] = initFrame(assets[bLBgIdx], assets[bLFgIdx], 1, 0, 1);
    map[0][1] = initFrame(assets[tRBgIdx], assets[tRFgIdx], 0, 1, 2);
    map[1][1] = initFrame(assets[bRBgIdx], assets[bRFgIdx], 1, 1, 3);
}

Frame* initFrame(u_long* bgSprite, u_long* fgSprite, u_char xIdx, u_char yIdx, u_char blockIndex) {
    Frame* frame = (Frame*) malloc3(sizeof(Frame));
    CollisionBlocks* cbs = (CollisionBlocks*) malloc3(sizeof(CollisionBlocks));
    frame->bg = assetmanager_loadSprite("bg", bgSprite, 0, 0, 128, COLOR_BITS_8);
    frame->fg = assetmanager_loadSprite("fg", fgSprite, 0, 0, 128, COLOR_BITS_8);
    frame->xIdx = xIdx;
    frame->yIdx = yIdx;
    setCollisionBounds(cbs, blockIndex);
    frame->cbs = cbs;
    return frame;
}

void setCollisionBounds(CollisionBlocks* cbs, u_char blockIndex) {
    printf("--------------------------------------------\n");
    printf("Setting collision bounds from bounds holder.\n");
    printf("--------------------------------------------\n");
    cbs->amount = mapCoords->frameCoords[blockIndex].amount;
    cbs->bounds = mapCoords->frameCoords[blockIndex].bounds;
    printf("Bounds amount=%d\n", cbs->amount);
    logger_logBoundsArray(cbs->bounds, cbs->amount);

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

void tickMap(GsSPRITE* sprite) {
    handleEdgeCollision(sprite);
    handleBlockCollision(sprite);
}

void handleEdgeCollision(GsSPRITE* sprite) {
    // X bounds -------------------------------------
    if(sprite->x < 0) {
         if(currXFrame > 0) {
            sprite->x = screenWidth - sprite->w;
            currXFrame--;
         } else {
             sprite->x = 0;
         }
    } 
    if(sprite->x + sprite->w > screenWidth) {
        if(currXFrame < 1) {
            sprite->x = 0;
            currXFrame++;
        } else {
            sprite->x = screenWidth - sprite->w;
        }
    } 

    // Y bounds -------------------------------------
    if(sprite->y < 0) {
        if(currYFrame > 0) {
            sprite->y = screenHeight - sprite->h;
            currYFrame--;
        } else {
            sprite->y = 0;
        }
    } 
    if(sprite->y + sprite->h > screenHeight) {
        if(currYFrame < 1) {
            sprite->y = 0;
            currYFrame++;
        } else {
            sprite->y = screenHeight - sprite->h;
        }
    }
}

void handleBlockCollision(GsSPRITE* sprite) {
    CollisionBlocks* blocks = map[currXFrame][currYFrame]->cbs;
    int i = 0;
    
    // Player bounds
    short px = sprite->x;
    short py = sprite->y;
    u_short pw = sprite->w;
    u_short ph = sprite->h;
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
            sprite->x = bx - pw;
        } 
        if(leftCol) {
            sprite->x = bxw;
        }
        if(topCol) {
            sprite->y = by - ph;
        }
        if(bottomCol) {
            sprite->y = byh;
        }

        i++;
    }   
}

#endif