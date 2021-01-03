#ifndef _AnimatedGameObject_
#define _AnimatedGameObject_

#include "assetmanager.h"
#include "base.h"
#include "mem.h"

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

AnimatedGameObject* animatedobject_set(char* name, u_long* spriteData, u_short x, u_short y, u_short w, u_short h, u_short blend, u_char keyFrames, u_short ticksPerFrame, u_short numColorBits);
void animatedobject_tick(AnimatedGameObject* gameObject);
void animatedobject_setHeading(AnimatedGameObject* gobj, u_char l, u_char r, u_char u, u_char d);


#endif