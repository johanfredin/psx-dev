#include "header/gridmap.h"
#include "header/mapbounds.h"

GridMap* map;
u_char rightCol, leftCol, topCol, bottomCol;
u_long** assets;

Frame* initFrame(u_long* bgSprite, u_long* fgSprite, u_char xIdx, u_char yIdx, u_char blockIndex);
void handleEdgeCollision(GsSPRITE* sprite);
void handleBlockCollision(GsSPRITE* sprite);
void handleTeleportCollision(GsSPRITE* sprite);
void setLevelAssets(u_char level);


void gridmap_init(u_char level, u_char tLBgIdx, u_char tLFgIdx, u_char tRBgIdx, u_char tRFgIdx, u_char bLBgIdx, u_char bLFgIdx, u_char bRBgIdx, u_char bRFgIdx) {
    map = MEM_ALLOC(GridMap);
    map->level = level;
    setLevelAssets(level);
    map->mainFrames[0][0] = initFrame(assets[tLBgIdx], assets[tLFgIdx], 0, 0, 0);
    map->mainFrames[1][0] = initFrame(assets[bLBgIdx], assets[bLFgIdx], 1, 0, 1);
    map->mainFrames[0][1] = initFrame(assets[tRBgIdx], assets[tRFgIdx], 0, 1, 2);
    map->mainFrames[1][1] = initFrame(assets[bRBgIdx], assets[bRFgIdx], 1, 1, 3);
    mapbounds_init(map);
}

void setLevelAssets(u_char level) {
    CdOpen();
    switch(level) {
        case 1:
            assets = (u_long**) calloc3(10, sizeof(u_long));  //MEM_CALLOC(8, u_long);
            CdReadFile("00BG.TIM", &assets[0]);
            CdReadFile("01BG.TIM", &assets[1]);
            CdReadFile("10BG.TIM", &assets[2]);
            CdReadFile("11BG.TIM", &assets[3]);
            CdReadFile("00FG.TIM", &assets[4]);
            CdReadFile("01FG.TIM", &assets[5]);
            CdReadFile("10FG.TIM", &assets[6]);
            CdReadFile("11FG.TIM", &assets[7]);
            CdReadFile("1000iBG.TIM", &assets[8]);
            CdReadFile("1000iFG.TIM", &assets[9]);
            break;
    }
    CdClose();
}

Frame* initFrame(u_long* bgSprite, u_long* fgSprite, u_char xIdx, u_char yIdx, u_char blockIndex) {
    Frame* frame = MEM_ALLOC(Frame);
    CollisionBlocks* cbs = MEM_ALLOC(CollisionBlocks);
    frame->bg = assetmanager_loadSprite("bg", bgSprite, 0, 0, 128, COLOR_BITS_8);
    frame->fg = assetmanager_loadSprite("fg", fgSprite, 0, 0, 128, COLOR_BITS_8);
    frame->xIdx = xIdx;
    frame->yIdx = yIdx;
    frame->cbs = cbs;
    return frame;
}

void gridmap_draw() {
    CollisionBlocks* blocks = map->mainFrames[gridmap_currXFrame][gridmap_currYFrame]->cbs;
    GsSortFastSprite(map->mainFrames[gridmap_currXFrame][gridmap_currYFrame]->fg, currentOT(), 0);
    GsSortFastSprite(map->mainFrames[gridmap_currXFrame][gridmap_currYFrame]->bg, currentOT(), 2);
    if(DRAW_BOUNDS) {
        int blockIdx = 0;
        while(blockIdx < blocks->amount) {
            DrawPrim(&blocks->boundLines[blockIdx]);
            blockIdx++;
        }
    }
    if(PRINT_COORDS) {
        FntPrint("Blocks in frame=%d\n", blocks->amount);
        FntPrint("tc=%d, bc=%d, lc=%d, rc=%d\n", topCol, bottomCol, leftCol, rightCol);
    }
}

void gridmap_tick(Player* player) {
    handleEdgeCollision(player->gobj->textureFrame);
    handleBlockCollision(player->gobj->textureFrame);
}

void handleEdgeCollision(GsSPRITE* sprite) {
    // X bounds -------------------------------------
    if(sprite->x < 0) {
         if(gridmap_currXFrame > 0) {
            sprite->x = screenWidth - sprite->w;
            gridmap_currXFrame--;
         } else {
             sprite->x = 0;
         }
    } 
    if(sprite->x + sprite->w > screenWidth) {
        if(gridmap_currXFrame < 1) {
            sprite->x = 0;
            gridmap_currXFrame++;
        } else {
            sprite->x = screenWidth - sprite->w;
        }
    } 

    // Y bounds -------------------------------------
    if(sprite->y < 0) {
        if(gridmap_currYFrame > 0) {
            sprite->y = screenHeight - sprite->h;
            gridmap_currYFrame--;
        } else {
            sprite->y = 0;
        }
    } 
    if(sprite->y + sprite->h > screenHeight) {
        if(gridmap_currYFrame < 1) {
            sprite->y = 0;
            gridmap_currYFrame++;
        } else {
            sprite->y = screenHeight - sprite->h;
        }
    }
}

void handleBlockCollision(GsSPRITE* sprite) {
    CollisionBlocks* blocks = map->mainFrames[gridmap_currXFrame][gridmap_currYFrame]->cbs;
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

void handleTeleportCollision(GsSPRITE* sprite) {
    Teleports* teleports = map->mainFrames[gridmap_currXFrame][gridmap_currYFrame]->teleports;
    int i = 0;
    while(i < teleports->amount) {
        
        i++;
    }
}


