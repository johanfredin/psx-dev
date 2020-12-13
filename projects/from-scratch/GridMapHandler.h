#ifndef _GridMapHandler_
#define _GridMapHandler_

#include "assetmanager.h"
#include "logger.h"

#define NUM_X_FRAMES 2
#define NUM_Y_FRAMES 2

typedef struct {
    RECT* bounds;
    u_char amount;
    LINE_F4* boundLines;
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


// Prototypes
// ----------
Frame* initFrame(u_long* bgSprite, u_long* fgSprite, u_char xIdx, u_char yIdx);
RECT getBounds(u_short x, u_short y, u_short w, u_short h);
void setCollisionBounds(CollisionBlocks* cbs);
void initMap(u_long* assets[], u_char, u_char, u_char, u_char, u_char, u_char, u_char, u_char);
void tickMap(GsSPRITE*);
void drawMap();
void drawBounds();


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
    blocks[2] = getBounds(32,176,16,16);
    blocks[3] = getBounds(144,176,112,80);
    blocks[4] = getBounds(176,64, 64, 32);
    blocks[5] = getBounds(48,64,48,32);
    blocks[6] = getBounds(32,208,80,32);
    blocks[7] = getBounds(112,96,48,32);
    cbs->amount = 8;
    cbs->bounds = blocks;
     if(DebugMode) {
        int i = 0;
        LINE_F4* boundLines = (LINE_F4*) malloc3(sizeof(LINE_F4)* 8);
        while(i < 8) {
            LINE_F4 lineF4;
            SetLineF4(&lineF4);
            
            // (xo, yo)                                        (x1. y1)
            lineF4.x0 = cbs->bounds[i].x;                      lineF4.x1 = cbs->bounds[i].x + cbs->bounds[i].w;
            lineF4.y0 = cbs->bounds[i].y;                      lineF4.y1 = cbs->bounds[i].y;
            
            // (x2, y2)                                         (x3. y3)
            lineF4.x2 = cbs->bounds[i].x;                      lineF4.x3 = cbs->bounds[i].x + cbs->bounds[i].w;
            lineF4.y2 = cbs->bounds[i].y + cbs->bounds[i].h;   lineF4.y3 = cbs->bounds[i].y + cbs->bounds[i].h;
            
            lineF4.r0 = 255;
            lineF4.g0 = 0;
            lineF4.b0 = 0;
            boundLines[i] = lineF4;
            logger_logVertices(&lineF4);
            i++;
        }
        cbs->boundLines = boundLines;
    }
}

void initMap(u_long* assets[], u_char tLBgIdx, u_char tLFgIdx, u_char tRBgIdx, u_char tRFgIdx, u_char bLBgIdx, u_char bLFgIdx, u_char bRBgIdx, u_char bRFgIdx) {
    CollisionBlocks* cbs1;
    map[0][0] = initFrame(assets[tLBgIdx], assets[tLFgIdx], 0, 0);
    map[0][1] = initFrame(assets[tRBgIdx], assets[tRFgIdx], 0, 1);
    map[1][0] = initFrame(assets[bLBgIdx], assets[bLFgIdx], 1, 0);
    map[1][1] = initFrame(assets[bRBgIdx], assets[bRFgIdx], 1, 1);

    cbs1 = (CollisionBlocks*) malloc3(sizeof(CollisionBlocks));
    setCollisionBounds(cbs1);
    map[0][0]->cbs = cbs1;
}


void drawMap() {
    GsSortFastSprite(map[currXFrame][currYFrame]->fg, currentOT(), 0);
    GsSortFastSprite(map[currXFrame][currYFrame]->bg, currentOT(), 2);
    if(DebugMode) {
        drawBounds();
    }
}

RECT getBounds(u_short x, u_short y, u_short w, u_short h) {
    RECT r = {x, y, w, h};
    printf("Bounds added: {x:%d, y:%d, w:%d, h:%d}\n", x, y, w, h);
    return r;
}

void tickMap(GsSPRITE* player) {
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

void drawBounds() {
    u_short i = 0;
    while(i < map[0][0]->cbs->amount) {
        DrawPrim((LINE_F4*) &map[0][0]->cbs->boundLines[i]);
        i++;
    }
}

#endif