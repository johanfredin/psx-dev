#ifndef _AnimatedGameObject_
#define _AnimatedGameObject_

#include "assetmanager.h"
#include "logger.h"

typedef struct {
    u_int left: 1;
    u_int right: 1;
    u_int up: 1;
    u_int down: 1;
} Heading;

typedef struct {
    GsSPRITE* textureFrame;
    Heading heading;
    u_char keyFrames;
    u_short ticksPerFrame;
    u_short curr_u, curr_v;
    u_short acc;
} AnimatedGameObject;

//Prototypes----

AnimatedGameObject* setGameObject(char* name, u_long* spriteData, u_short x, u_short y, u_short w, u_short h, u_short blend, u_char keyFrames, u_short ticksPerFrame, u_short numColorBits);
void updateGameObject(AnimatedGameObject* gameObject);
void setHeading(AnimatedGameObject* gobj, u_char l, u_char r, u_char u, u_char d);
void setXOffset(AnimatedGameObject* gobj);
void logger_logGameObject(AnimatedGameObject* gobj);
char isMoving(AnimatedGameObject* gobj);

// Definitions ------

AnimatedGameObject* setGameObject(char* name, u_long* spriteData, u_short x, u_short y, u_short w, u_short h, u_short blend, u_char keyFrames, u_short ticksPerFrame, u_short numColorBits) {
    AnimatedGameObject* gameObject;
    Heading heading;
    GsSPRITE* textureFrame = assetmanager_loadSprite(name, spriteData, x, y, blend, numColorBits);
    textureFrame->w = w;
    textureFrame->h = h;
    gameObject = (AnimatedGameObject*) malloc3(sizeof(AnimatedGameObject));
    gameObject->textureFrame = textureFrame;
    gameObject->heading = heading;
    gameObject->curr_u = textureFrame->u;
    gameObject->curr_v = textureFrame->v;
    gameObject->keyFrames = keyFrames;
    gameObject->ticksPerFrame = ticksPerFrame;
    gameObject->acc = 0;
    setHeading(gameObject, 0, 0, 0, 0);
    logger_logGameObject(gameObject);
    return gameObject;
}   

void updateGameObject(AnimatedGameObject* gameObject) {
    short u=gameObject->curr_v;
    short v=gameObject->curr_v;
    if(gameObject->heading.left) {
        v = 16;
    } else if(gameObject->heading.right) {
        v = 32;
    } else if(gameObject->heading.up) {
        v = 48;
    } else if(gameObject->heading.down) {
        v = 0;
    } 
    gameObject->textureFrame->v = v;
    gameObject->curr_v = v;
    setXOffset(gameObject);
}

void setHeading(AnimatedGameObject* gobj, u_char l, u_char r, u_char u, u_char d) {
    gobj->heading.left = l;
    gobj->heading.right = r;
    gobj->heading.up = u;
    gobj->heading.down = d;
}

void setXOffset(AnimatedGameObject* gobj) {
    gobj->acc += 1;
    FntPrint("Acc=%d\ncurr_u=%d\nmoving=%d\n", gobj->acc, gobj->curr_u, isMoving(gobj));
    if(gobj->acc >= gobj->ticksPerFrame) {
        gobj->acc = 0;
        if(gobj->curr_u >= ((gobj->keyFrames * gobj->textureFrame->w) - gobj->textureFrame->w)) {
            gobj->curr_u = 0;
        } else {
            if(isMoving(gobj)) {
                gobj->curr_u += gobj->textureFrame->w;
            }
        }
    }
    gobj->textureFrame->u = gobj->curr_u;
}

void logger_logGameObject(AnimatedGameObject* gobj) {
    printf("-----------------------------\n");
    printf("Animated Game Object created:\n");
    printf("-----------------------------\n");
    printf("x:%d, y:%d, w%d, h%d\n", gobj->textureFrame->x, gobj->textureFrame->y, gobj->textureFrame->w, gobj->textureFrame->h);
    printf("ticks_per_frame:%d, curr_u:%d, curr_v:%d\n", gobj->ticksPerFrame, gobj->curr_u, gobj->curr_v);
    printf("key_frames:%d, acc:%d\n", gobj->keyFrames, gobj->acc);
    printf("-----------------------------\n");
}

char isMoving(AnimatedGameObject* gobj) {
    return gobj->heading.left == 1 || gobj->heading.right == 1 || gobj->heading.up == 1 || gobj->heading.down == 1;
}

#endif