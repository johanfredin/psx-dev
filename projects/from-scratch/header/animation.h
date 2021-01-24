#ifndef _animation_h_
#define _animation_h_

#include "base.h"
#include "logger.h"
#include "gpubase.h"
#include "gameobject.h"

typedef struct {
    u_char keyFrames;
    u_short ticksPerFrame;
    u_short curr_u;
    u_short curr_v;
    u_short accTicks;
} Animation;

/**
 * Create a new animation object on the heap with given properties
 * @param curr_u the starting texture frame x offset of the sprite we will work with
 * @param curr_v the starting texture frame y offset of the sprite we will work with
 * @param keyFrames the amount of frames on the x axis of the spritesheet passed in. Eg. if sprite w=96 and keyFrames=3 then animation object will assume gameobject sprite has a width of 32 pixels and update annimation in that fassion.
 * @param ticksPerFrame how many rendering iterations until its time to update frame. 
 * @return pointer to a new animation object on the heap with given properties
 */
Animation* animation_init(u_short curr_u, u_short curr_v, u_char keyFrames, u_short ticksPerFrame);
/**
 * Update passed in object with current animation state
 * @param anim animation object that will update sprite of passed in game object
 * @param gobj game object we want to animate.
 */ 
void animation_tick(Animation *anim, GameObject *gobj);

#endif