#include "header/animation.h"

Animation* animation_init(u_short curr_u, u_short curr_v, u_char keyFrames, u_short ticksPerFrame) {
    Animation *anim = MALLOC(Animation);
    anim->curr_u = curr_u;
    anim->curr_v = curr_v;
    anim->keyFrames = keyFrames;
    anim->ticksPerFrame = ticksPerFrame;
    anim->accTicks = 0;
    logger_logGameObject(anim);
    return anim;
}

void animation_tick(Animation *anim, GameObject *gobj) {
    short u = anim->curr_v;
    short v = anim->curr_v;
    if (gobj->heading.left) {
        v = 16;
    } else if (gobj->heading.right) {
        v = 32;
    } else if (gobj->heading.up) {
        v = 48;
    } else if (gobj->heading.down) {
        v = 0;
    }
    gobj->sprite->v = v;
    anim->curr_v = v;

    anim->accTicks += 1;

    if (PRINT_ANIMATION) {
        FntPrint("Acc=%d, curr_u=%d, moving=%d\n", anim->accTicks, anim->curr_u, gameobject_isMoving(gobj));
    }

    if (anim->accTicks >= anim->ticksPerFrame) {
        anim->accTicks = 0;
        if (anim->curr_u >= ((anim->keyFrames * gobj->sprite->w) - gobj->sprite->w)) {
            anim->curr_u = 0;
        } else {
            if (gameobject_isMoving(gobj)) {
                anim->curr_u += gobj->sprite->w;
            }
        }
    }
    gobj->sprite->u = anim->curr_u;
}
