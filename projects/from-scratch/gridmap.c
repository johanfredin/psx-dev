#include "header/gridmap.h"

Frame *frames;
u_char assetsCount, currentFrame = 4;
u_long **assets;

void initFrame(Frame *frame, u_long *bgSprite, u_long *fgSprite, char name[], u_long *gameObjectAsset);
void initSmallerFrame(Frame *frame, u_long *bgSprite, u_long *fgSprite, u_char x, u_char y, char name[]);
void handleBlockCollision(GameObject *gobj, Frame *frame);
void handleTeleportCollision(GameObject *gobj, Frame *frame);
u_char setLevelAssets(u_char level);

void gridmap_init(u_char level) {
    assetsCount = setLevelAssets(level);
    frames = CALLOC(7, Frame);
    initFrame(&frames[0], assets[0], assets[4], "00", assets[12]);
    initFrame(&frames[1], assets[1], assets[5], "01", assets[13]);
    initFrame(&frames[2], assets[2], assets[6], "10", assets[14]);
    initFrame(&frames[3], assets[3], assets[7], "11", assets[15]);
    initSmallerFrame(&frames[4], assets[8], assets[9], 256 / 4, 256 / 4, "yolo");
    initSmallerFrame(&frames[5], assets[10], NULL, 0, 256 / 3, "tunnel");
    initSmallerFrame(&frames[6], assets[11], NULL, 256 / 4, 256 / 8, "1_h2");
    mapbounds_init(level, frames);
}

u_char setLevelAssets(u_char level) {
    u_char count = 0;
    log(INFO, "*********************************");
    log(INFO, "* ADDING ASSETS FOR LEVEL NR %d *", level);
    log(INFO, "*********************************");
    CdOpen();
    switch (level) {
    case 1:
        assets = (u_long **)calloc3(16, sizeof(u_long)); //MEM_CALLOC(8, u_long);
        CdReadFile("00BG.TIM", &assets[0], &count);
        CdReadFile("01BG.TIM", &assets[1], &count);
        CdReadFile("10BG.TIM", &assets[2], &count);
        CdReadFile("11BG.TIM", &assets[3], &count);
        CdReadFile("00FG.TIM", &assets[4], &count);
        CdReadFile("01FG.TIM", &assets[5], &count);
        CdReadFile("10FG.TIM", &assets[6], &count);
        CdReadFile("11FG.TIM", &assets[7], &count);
        CdReadFile("YOLOBG.TIM", &assets[8], &count);
        CdReadFile("YOLOFG.TIM", &assets[9], &count);
        CdReadFile("1_TUNNEL.TIM", &assets[10], &count);
        CdReadFile("1_H2.TIM", &assets[11], &count);
        CdReadFile("PSYDUCK.TIM", &assets[12], &count);
        CdReadFile("RAICHU.TIM", &assets[13], &count);
        CdReadFile("RAICHU_2.TIM", &assets[14], &count);
        CdReadFile("ALOLA.TIM", &assets[15], &count);
        break;
    }
    CdClose();
    return count;
}

void initFrame(Frame *frame, u_long *bgSprite, u_long *fgSprite, char name[6], u_long *gameObjectAsset) {
    frame->cbs = MALLOC(CollisionBlocks);

    // Init BG sprite if provided
    if (bgSprite == NULL) {
        frame->bg = NULL;
        log(DEBUG, "BG sprite NULL so no BG for frame");
    } else {
        frame->bg = assetmanager_loadSprite(name, bgSprite, 0, 0, 128, COLOR_BITS_8);
    }

    // Init FG sprite if provided
    if (fgSprite == NULL) {
        frame->fg = NULL;
        log(DEBUG, "FG sprite NULL so no FG for frame\n");
    } else {
        frame->fg = assetmanager_loadSprite(name, fgSprite, 0, 0, 128, COLOR_BITS_8);
    }

    // Init Game object if provided
    if (gameObjectAsset != NULL) {
        frame->gameObject = gameobject_init(assetmanager_loadSprite("Game object", gameObjectAsset, 90, 120, 128, COLOR_BITS_8), 16, 16, 2, 3, 100, TYPE_NPC);
    }
}

void initSmallerFrame(Frame *frame, u_long *bgSprite, u_long *fgSprite, u_char x, u_char y, char name[]) {
    initFrame(frame, bgSprite, fgSprite, name, NULL);
    frame->bg->x = x;
    frame->fg->x = x;
    frame->bg->y = y;
    frame->fg->y = y;
}

void gridmap_draw() {
    Frame *frame = &frames[currentFrame];
    CollisionBlocks *blocks = frame->cbs;
    Teleport *teleports = frame->teleports;

    if (frame->fg != NULL) {
        GsSortFastSprite(frame->fg, currentOT(), 0);
    }
    if (frame->bg != NULL) {
        GsSortFastSprite(frame->bg, currentOT(), 2);
    }
    if (frame->gameObject != NULL) {
        gameobject_draw(frame->gameObject);
    }

    if (PRINT_COORDS) {
        FntPrint("Current framee=%d\n", currentFrame);
        FntPrint("Blocks in frame=%d\n", blocks->amount);
        FntPrint("Teleports in frame=%d\n", frame->t_amount);
    }

    if (DRAW_BOUNDS) {
        int blockIdx = 0, t_idx = 0;
        while (blockIdx < blocks->amount) {
            DrawPrim(&blocks->boundLines[blockIdx]);
            blockIdx++;
        }
        while (t_idx < frame->t_amount) {
            DrawPrim(&teleports[t_idx].boundLines);
            t_idx++;
        }
    }
}

void gridmap_tick(Player *player) {
    Frame *frame = &frames[currentFrame];
    // TEMP
    u_long btn = PadRead(1);
    if (btn & PADselect) {
        if (currentFrame >= 3) {
            currentFrame = 0;
        } else {
            currentFrame++;
        }
    }

    handleTeleportCollision(player->gobj, frame);
    handleBlockCollision(player->gobj, frame);
    if (frame->gameObject != NULL) {
        gameobject_tick(frame->gameObject, player);
        handleBlockCollision(frame->gameObject, frame);
        handleTeleportCollision(frame->gameObject, frame);
    }
}

void handleBlockCollision(GameObject *gobj, Frame *frame) {
    u_char rightCol, leftCol, topCol, bottomCol;
    CollisionBlocks *blocks = frame->cbs;
    int i = 0;

    // Sprite  bounds
    short px = gobj->sprite->x;
    short py = gobj->sprite->y;
    u_short pw = gobj->sprite->w;
    u_short ph = gobj->sprite->h;
    u_short pxw = px + pw;
    u_short pyh = py + ph;

    while (i < blocks->amount) {
        // Collision block bounds
        RECT *bounds = &blocks->bounds[i];
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

        switch (gobj->type) {
        case TYPE_PLAYER:
            if (rightCol) {
                gobj->sprite->x = bx - pw;
            }
            if (leftCol) {
                gobj->sprite->x = bxw;
            }
            if (topCol) {
                gobj->sprite->y = by - ph;
            }
            if (bottomCol) {
                gobj->sprite->y = byh;
            }
            break;
        case TYPE_NPC:
            if (rightCol || leftCol) {
                switchXDir(gobj);
            }
            if (topCol || bottomCol) {
                switchYDir(gobj);
            }
            break;
        }
        i++;
    }
}

void handleTeleportCollision(GameObject *gobj, Frame *frame) {
    u_char rightCol, leftCol, topCol, bottomCol;
    Teleport *teleports = frame->teleports;
    int i = 0;

    // Player bounds
    short px = gobj->sprite->x;
    short py = gobj->sprite->y;
    u_short pw = gobj->sprite->w;
    u_short ph = gobj->sprite->h;
    u_short pxw = px + pw;
    u_short pyh = py + ph;
    while (i < frame->t_amount) {
        Teleport t = teleports[i];
        RECT bounds = t.origin;
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

        if (rightCol || leftCol || topCol || bottomCol) {
            switch (gobj->type) {
            case TYPE_PLAYER:
                if (t.destX > -1) {
                    gobj->sprite->x = t.destX;
                }
                if (t.destY > -1) {
                    gobj->sprite->y = t.destY;
                }
                if (frame->gameObject != NULL && frame->gameObject->type == TYPE_NPC) {
                    resetPosition(frame->gameObject);
                }
                currentFrame = t.destFrame;
                break;
            case TYPE_NPC:
                if (rightCol || leftCol) {
                    switchXDir(gobj);
                }
                if (topCol || bottomCol) {
                    switchYDir(gobj);
                }
                break;
            }
        }

        i++;
    }
}
