#ifndef _GridMapHandler_
#define _GridMapHandler_

#include "assetmanager.h"

typedef struct {
    u_char xIdx, yIdx;
    GsSPRITE* bg;
    GsSPRITE* fg;
} Frame;

u_char currXFrame;
u_char currYFrame;
u_short SCREEN_WIDTH; 
u_short SCREEN_HEIGHT;

Frame* initFrame(u_long* bgSprite, u_long* fgSprite, u_char xIdx, u_char yIdx);
void setBounds(u_short w, u_short h);
void tickMap(GsSPRITE* player);
void drawMap(Frame* map[2][2]);

Frame* initFrame(u_long* bgSprite, u_long* fgSprite, u_char xIdx, u_char yIdx) {
    Frame* frame = (Frame*) malloc3(sizeof(Frame));

    frame->bg = assetmanager_loadSprite("bg", bgSprite, 0, 0, 128, COLOR_BITS_8);
    frame->fg = assetmanager_loadSprite("fg", fgSprite, 0, 0, 128, COLOR_BITS_8);
    frame->xIdx = xIdx;
    frame->yIdx = yIdx;
    return frame;
}

void setBounds(u_short w, u_short h) {
    SCREEN_WIDTH = w;
    SCREEN_HEIGHT = h;
}

void drawMap(Frame* map[2][2]) {
    GsSortFastSprite(map[currXFrame][currYFrame]->fg, currentOT(), 0);
    GsSortFastSprite(map[currXFrame][currYFrame]->bg, currentOT(), 2);
}

void tickMap(GsSPRITE* player) {
    if(hero->x < 0) {
        hero->x = SCREEN_WIDTH - hero->w;
        currXFrame--;
    } if(hero->x + hero->w > SCREEN_WIDTH) {
        hero->x = 0;
        currXFrame++;
    } if(hero->y < 0) {
        hero->y = SCREEN_HEIGHT - hero->h;
        currYFrame--;
    } if(hero->y + hero->h > SCREEN_HEIGHT) {
        hero->y = 0;
        currYFrame++;
    }
}

#endif