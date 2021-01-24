#include "header/logger.h"

void logger_logCoords(RECT* rect, char* source) {
    printf("%s coords assigned at {x:%d, y%d, w:%d, h:%d}\n", source, rect->x, rect->y, rect->w, rect->h);
}

void logger_logBounds(TILE* bounds) {
    printf("TILE bounds={x:%d, y:%d, w:%d, h:%d}\n", bounds->x0, bounds->y0, bounds->w, bounds->h);
}

void logger_logBoundsArray(RECT* rect, u_char amount) {
    u_char i = 0;
    while(i < amount) {
        logger_logCoords(&rect[i], "Bounds");
        i++;
    }
}

void logger_logGameObject(Animation* anim) {
    printf("-----------------------------\n");
    printf("Animated Game Object created:\n");
    printf("-----------------------------\n");
    printf("ticks_per_frame:%d, curr_u:%d, curr_v:%d\n", anim->ticksPerFrame, anim->curr_u, anim->curr_v);
    printf("key_frames:%d, acc:%d\n", anim->keyFrames, anim->accTicks);
    printf("-----------------------------\n");
}