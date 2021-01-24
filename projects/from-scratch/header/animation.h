#ifndef _animation_h_
#define _animation_h_

#include "base.h"
#include "mem.h"
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
} Animation;

Animation* animation_init(GsSPRITE* textureFrame, u_short w, u_short h, u_char keyFrames, u_short ticksPerFrame)
void animation_tick(Animation* animation);
void animatedobject_setHeading(Animation* animation, u_char l, u_char r, u_char u, u_char d);


#endif