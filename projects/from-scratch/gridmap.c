#include "header/gridmap.h"
#include "header/mapbounds.h"

#include <LIBETC.H> // Temp import

Frame* frames;
u_char assetsCount, currentFrame=0;
u_long** assets;

void initFrame(Frame* frame, u_long* bgSprite, u_long* fgSprite);
void handleBlockCollision(GsSPRITE* sprite);
void handleTeleportCollision(GsSPRITE* sprite);
u_char setLevelAssets(u_char level);


void gridmap_init(u_char level) {

    assetsCount = setLevelAssets(level);
    frames = CALLOC(assetsCount, Frame);
    initFrame(&frames[0], assets[0], assets[4]);
    initFrame(&frames[1], assets[1], assets[5]);
    initFrame(&frames[2], assets[2], assets[6]);
    initFrame(&frames[3], assets[3], assets[7]);
    initFrame(&frames[4], assets[8], assets[9]);
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
    CollisionBlocks* cbs = MALLOC(CollisionBlocks);
    frame->bg = assetmanager_loadSprite("bg", bgSprite, 0, 0, 128, COLOR_BITS_8);
    frame->fg = assetmanager_loadSprite("fg", fgSprite, 0, 0, 128, COLOR_BITS_8);
    frame->cbs = cbs;
}

void gridmap_draw() {
    Frame* frame = &frames[currentFrame];
    CollisionBlocks* blocks = frame->cbs;
    Teleport* teleports = frame->teleports;
    GsSortFastSprite(frame->fg, currentOT(), 0);
    GsSortFastSprite(frame->bg, currentOT(), 2);
    if(PRINT_COORDS) {
        FntPrint("Current framee=%d\n", currentFrame);
        FntPrint("Blocks in frame=%d\n", blocks->amount);
        FntPrint("Teleports in frame=%d\n", frame->t_amount);
    }

    if(DRAW_BOUNDS) {
        int blockIdx = 0, t_idx = 0;
        while(blockIdx < blocks->amount) {
            DrawPrim(&blocks->boundLines[blockIdx]);
            blockIdx++;
        }
        while(t_idx < frame->t_amount) {
            DrawPrim(&teleports[t_idx].boundLines);
            t_idx++;
        }
    }
}

void gridmap_tick(Player* player) {
    // TEMP
    u_long btn = PadRead(1);
    if(btn & PADselect) {
        if(currentFrame >= 3) {
            currentFrame = 0;
        } else {
            currentFrame++;
        }
    }

    handleTeleportCollision(player->gobj->textureFrame);
    handleBlockCollision(player->gobj->textureFrame);
}

void handleBlockCollision(GsSPRITE* sprite) {
    u_char rightCol, leftCol, topCol, bottomCol;
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
    u_char rightCol, leftCol, topCol, bottomCol;
    Teleport* teleports = frames[currentFrame].teleports;
    int i = 0;

     // Player bounds
    short px = sprite->x;
    short py = sprite->y;
    u_short pw = sprite->w;
    u_short ph = sprite->h;
    u_short pxw = px + pw;
    u_short pyh = py + ph;
    while(i < frames[currentFrame].t_amount) {
        RECT bounds = teleports[i].origin;
        short bx = bounds.x;
        short by = bounds.y;
        u_short bw = bounds.w;
        u_short bh = bounds.h;
        u_short bxw = bx + bw;
        u_short byh = by + bh;

        rightCol = pxw >= bx && px <= bx && pyh > by && py < byh;
        leftCol = px <= bxw && pxw > bxw && pyh > by && py < byh;
        topCol = pyh >= by && py < by && pxw > bx && px < bxw;
        bottomCol = py <= byh && pyh > byh && pxw > bx && px < bxw;
        
        if(rightCol || leftCol || topCol || bottomCol) {
            Teleport t = teleports[i];
            currentFrame = t.destFrame;
            if(t.destX > -1) {
                sprite->x = t.destX;
            }
            if(t.destY > -1) {
                sprite->y = t.destY;
            }
            FntPrint("COOOOL with teleport nr %d\n", i);
        }
        i++;
    }
}


