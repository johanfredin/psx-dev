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

Frame* initFrame(u_long* bgSprite, u_long* fgSprite, u_char xIdx, u_char yIdx);

Frame* initFrame(u_long* bgSprite, u_long* fgSprite, u_char xIdx, u_char yIdx) {
    Frame* frame = (Frame*) malloc3(sizeof(Frame));
    
    frame->bg = assetmanager_loadSprite("bg", bgSprite, 0, 0, 128, COLOR_BITS_8);
    frame->fg = assetmanager_loadSprite("fg", fgSprite, 0, 0, 128, COLOR_BITS_8);
    frame->xIdx = xIdx;
    frame->yIdx = yIdx;
    return frame;
}


#endif