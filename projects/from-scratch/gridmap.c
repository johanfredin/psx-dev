#include "header/gridmap.h"

Frame* map[NUM_X_FRAMES][NUM_Y_FRAMES];
u_char rightCol, leftCol, topCol, bottomCol;

Frame* gridmap_initFrame(u_long* bgSprite, u_long* fgSprite, u_char xIdx, u_char yIdx, u_char blockIndex);
void gridmap_handleEdgeCollision(GsSPRITE* sprite);
void gridmap_handleBlockCollision(GsSPRITE* sprite);


void gridmap_init(u_long** assets, u_char tLBgIdx, u_char tLFgIdx, u_char tRBgIdx, u_char tRFgIdx, u_char bLBgIdx, u_char bLFgIdx, u_char bRBgIdx, u_char bRFgIdx) {
    mapbounds_init(1);
    map[0][0] = gridmap_initFrame(assets[tLBgIdx], assets[tLFgIdx], 0, 0, 0);
    map[1][0] = gridmap_initFrame(assets[bLBgIdx], assets[bLFgIdx], 1, 0, 1);
    map[0][1] = gridmap_initFrame(assets[tRBgIdx], assets[tRFgIdx], 0, 1, 2);
    map[1][1] = gridmap_initFrame(assets[bRBgIdx], assets[bRFgIdx], 1, 1, 3);
}

Frame* gridmap_initFrame(u_long* bgSprite, u_long* fgSprite, u_char xIdx, u_char yIdx, u_char blockIndex) {
    Frame* frame = (Frame*) malloc3(sizeof(Frame));
    CollisionBlocks* cbs = (CollisionBlocks*) malloc3(sizeof(CollisionBlocks));
    frame->bg = assetmanager_loadSprite("bg", bgSprite, 0, 0, 128, COLOR_BITS_8);
    frame->fg = assetmanager_loadSprite("fg", fgSprite, 0, 0, 128, COLOR_BITS_8);
    frame->xIdx = xIdx;
    frame->yIdx = yIdx;

    printf("--------------------------------------------\n");
    printf("Setting collision bounds from bounds holder.\n");
    printf("--------------------------------------------\n");
    cbs->amount = mapCoords->frameCoords[blockIndex].amount;
    cbs->bounds = mapCoords->frameCoords[blockIndex].bounds;
    printf("Bounds amount=%d\n", cbs->amount);
    logger_logBoundsArray(cbs->bounds, cbs->amount);

    // Draw tiles where collision bounds are supposed to be if set in main
    if(DRAW_BOUNDS) {
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
    frame->cbs = cbs;
    return frame;
}

void gridmap_draw() {
    CollisionBlocks* blocks = map[gridmap_currXFrame][gridmap_currYFrame]->cbs;
    GsSortFastSprite(map[gridmap_currXFrame][gridmap_currYFrame]->fg, currentOT(), 0);
    GsSortFastSprite(map[gridmap_currXFrame][gridmap_currYFrame]->bg, currentOT(), 2);
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
    gridmap_handleEdgeCollision(player->gobj->textureFrame);
    gridmap_handleBlockCollision(player->gobj->textureFrame);
}

void gridmap_handleEdgeCollision(GsSPRITE* sprite) {
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

void gridmap_handleBlockCollision(GsSPRITE* sprite) {
    CollisionBlocks* blocks = map[gridmap_currXFrame][gridmap_currYFrame]->cbs;
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
