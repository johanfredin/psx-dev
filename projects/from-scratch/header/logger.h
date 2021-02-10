#ifndef _logger_h_
#define _logger_h_

#include "STDARG.H"
#include "base.h"

#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_INFO 1
#define LOG_LEVEL_DEBUG 2
#define LOG_LEVEL_TRACE 3

#define LOG_NAMED_RECT(rect, rectName) log_d("%s Coords assigned at {x:%d, y%d, w:%d, h:%d}", rectName, rect->x, rect->y, rect->w, rect->h)
#define LOG_RECT(rect) log_d("Coords assigned at {x:%d, y%d, w:%d, h:%d}", (rect)->x, (rect)->y, (rect)->w, (rect)->h)
#define LOG_TILE(tile) log_d("TILE bounds={x:%d, y:%d, w:%d, h:%d}", (tile)->x0, (tile)->y0, (tile)->w, (tile)->h)

#define LOG_RECTS(rects, amount, i) \
    for (i = 0; i < amount; i++)           \
        LOG_RECT(&rects[i]);        \
    i = 0;

#define LOG_ANIM(anim, level)                                                                                      \
    log(level, "-----------------------------");                                                                   \
    log(level, "Animated Game Object created:");                                                                   \
    log(level, "ticks_per_frame:%d, curr_u:%d, curr_v:%d", (anim)->ticksPerFrame, (anim)->curr_u, (anim)->curr_v); \
    log(level, "key_frames:%d, acc:%d", (anim)->keyFrames, (anim)->accTicks);                                      \
    log(level, "-----------------------------");

#define LOG_TELEPORT(t) log("Frame teleport: origin={x:%d, y:%d, w:%d, h:%d}, destX=%d, destY=%d\n", (t)->origin.x, (t)->origin.y, (t)->origin.w, (t)->origin.h, (t)->destX, (t)->destY)

#define DEBUG_LOG(msg, ...) log_d(msg, va_start)

#define LOG_TELEPORTS(ts, amount, i) \
    for (i = 0; i < amount; i++)            \
        LOG_TELEPORT(&ts[i]);        \
    i = 0;

/*
 * Log a message at specified level
 * @param level log level (0=info, 1=debug, 2=trace)
 * @param msg the message to print
 */
void log(unsigned char level, char *msg, ...);

/*
 * Log a message at debug level
 * @param msg the message to print
 */
void log_d(char *msg, ...);
/*
 * Log a message at info level
 * @param msg the message to print
 */
void log_i(char *msg, ...);
/*
 * Log a message at trace level
 * @param msg the message to print
 */
void log_tr(char *msg, ...);
/*
 * Log a message at error level
 * @param msg the message to print
 */
void log_err(char *msg, ...);

#endif