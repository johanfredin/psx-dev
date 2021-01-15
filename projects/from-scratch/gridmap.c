#include "header/gridmap.h"
#include "header/mapbounds.h"

Frame* frames;
u_char assetsCount, currentFrame=0;
u_char rightCol, leftCol, topCol, bottomCol;
u_long** assets;

void initFrame(Frame* frame, u_long* bgSprite, u_long* fgSprite);
void handleBlockCollision(GsSPRITE* sprite);
void handleTeleportCollision(GsSPRITE* sprite);
u_char setLevelAssets(u_char level);


void gridmap_init(u_char level) {
    assetsCount = setLevelAssets(level);
    frames = (Frame*) calloc3(assetsCount, sizeof(Frame)); //MEM_CALLOC(assetsCount, Frame);
    initFrame(&frames[0], assets[0], assets[4]);
    initFrame(&frames[1], assets[1], assets[5]);
    initFrame(&frames[2], assets[2], assets[6]);
    initFrame(&frames[3], assets[3], assets[7]);
    mapbounds_init(level, frames);
}

u_char setLevelAssets(u_char level) {
    u_char count = 0;
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
            count = 10;
            break;
    }
    CdClose();
    return count;
}

void initFrame(Frame* frame, u_long* bgSprite, u_long* fgSprite) {
    CollisionBlocks* cbs = MEM_ALLOC(CollisionBlocks);
    frame->bg = assetmanager_loadSprite("bg", bgSprite, 0, 0, 128, COLOR_BITS_8);
    frame->fg = assetmanager_loadSprite("fg", fgSprite, 0, 0, 128, COLOR_BITS_8);
    frame->cbs = cbs;
}

void gridmap_draw() {
    CollisionBlocks* blocks = frames[currentFrame].cbs;
    GsSortFastSprite(frames[currentFrame].fg, currentOT(), 0);
    GsSortFastSprite(frames[currentFrame].bg, currentOT(), 2);
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
    handleTeleportCollision(player->gobj->textureFrame);
    handleBlockCollision(player->gobj->textureFrame);
}

void handleBlockCollision(GsSPRITE* sprite) {
    CollisionBlocks* blocks = frames[currentFrame].cbs;
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
    // Teleports* teleports = map->mainFrames[gridmap_currXFrame][gridmap_currYFrame]->teleports;
    // int i = 0;
    // while(i < teleports->amount) {
        
    //     i++;
    // }
    // X bounds -------------------------------------
    // if(sprite->x < 0) {
    //      if(gridmap_currXFrame > 0) {
    //         sprite->x = screenWidth - sprite->w;
    //         gridmap_currXFrame--;
    //      } else {
    //          sprite->x = 0;
    //      }
    // } 
    // if(sprite->x + sprite->w > screenWidth) {
    //     if(gridmap_currXFrame < 1) {
    //         sprite->x = 0;
    //         gridmap_currXFrame++;
    //     } else {
    //         sprite->x = screenWidth - sprite->w;
    //     }
    // } 

    // // Y bounds -------------------------------------
    // if(sprite->y < 0) {
    //     if(gridmap_currYFrame > 0) {
    //         sprite->y = screenHeight - sprite->h;
    //         gridmap_currYFrame--;
    //     } else {
    //         sprite->y = 0;
    //     }
    // } 
    // if(sprite->y + sprite->h > screenHeight) {
    //     if(gridmap_currYFrame < 1) {
    //         sprite->y = 0;
    //         gridmap_currYFrame++;
    //     } else {
    //         sprite->y = screenHeight - sprite->h;
    //     }
    // }
}


