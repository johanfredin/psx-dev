#ifndef _logger_h_
#define _logger_h_

#define LOG_NAMED_RECT(rect, rectName) printf("%s Coords assigned at {x:%d, y%d, w:%d, h:%d}\n", rectName, rect->x, rect->y, rect->w, rect->h)
#define LOG_RECT(rect) printf("Coords assigned at {x:%d, y%d, w:%d, h:%d}\n", (rect)->x, (rect)->y, (rect)->w, (rect)->h)
#define LOG_TILE(tile) printf("TILE bounds={x:%d, y:%d, w:%d, h:%d}\n", (tile)->x0, (tile)->y0, (tile)->w, (tile)->h)

#define LOG_RECTS(rects, amount, i) \
    for (i = 0; i < amount; i++)    \
        LOG_RECT(&rects[i]);        \
    i = 0;

#define LOG_ANIM(anim)                                                                                           \
    printf("-----------------------------\n");                                                                   \
    printf("Animated Game Object created:\n");                                                                   \
    printf("ticks_per_frame:%d, curr_u:%d, curr_v:%d\n", (anim)->ticksPerFrame, (anim)->curr_u, (anim)->curr_v); \
    printf("key_frames:%d, acc:%d\n", (anim)->keyFrames, (anim)->accTicks);                                      \
    printf("-----------------------------\n");

#define LOG_TELEPORT(t) printf("Frame teleport: origin={x:%d, y:%d, w:%d, h:%d}, destX=%d, destY=%d\n", (t)->origin.x, (t)->origin.y, (t)->origin.w, (t)->origin.h, (t)->destX, (t)->destY)

#define LOG_TELEPORTS(ts, amount, i) \
    for (i = 0; i < amount; i++)     \
        LOG_TELEPORT(&ts[i]);        \
    i = 0;

#endif