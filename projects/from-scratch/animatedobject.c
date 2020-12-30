
#include "header/animatedobject.h"

AnimatedGameObject* animatedobject_set(char* name, u_long* spriteData, u_short x, u_short y, u_short w, u_short h, u_short blend, u_char keyFrames, u_short ticksPerFrame, u_short numColorBits) {
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
    animatedobject_setHeading(gameObject, 0, 0, 0, 0);
    logger_logGameObject(gameObject);
    return gameObject;
}   

void animatedobject_tick(AnimatedGameObject* gameObject, short xSpeed, short ySpeed) {
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

    gameObject->acc += 1;
    FntPrint("Acc=%d\ncurr_u=%d\nmoving=%d\n", gameObject->acc, gameObject->curr_u, animatedobject_moving(gameObject));
    if(gameObject->acc >= gameObject->ticksPerFrame) {
        gameObject->acc = 0;
        if(gameObject->curr_u >= ((gameObject->keyFrames * gameObject->textureFrame->w) - gameObject->textureFrame->w)) {
            gameObject->curr_u = 0;
        } else {
            if(animatedobject_moving(gameObject)) {
                gameObject->curr_u += gameObject->textureFrame->w;
            }
        }
    }
    gameObject->textureFrame->u = gameObject->curr_u;
    gameObject->textureFrame->x += xSpeed;
    gameObject->textureFrame->y += ySpeed;
}

void animatedobject_setHeading(AnimatedGameObject* gobj, u_char l, u_char r, u_char u, u_char d) {
    gobj->heading.left = l;
    gobj->heading.right = r;
    gobj->heading.up = u;
    gobj->heading.down = d;
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

char animatedobject_moving(AnimatedGameObject* gobj) {
    return gobj->heading.left == 1 || gobj->heading.right == 1 || gobj->heading.up == 1 || gobj->heading.down == 1;
}
