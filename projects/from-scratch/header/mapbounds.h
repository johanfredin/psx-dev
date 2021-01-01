#ifndef _MapBoundsHolder_
#define _MapBoundsHolder_

/*
 * Holds boundaries for a map. 
 */

#include "gpubase.h"

/*
 * Struct holding boundaries coordinates for one frame within a map.
 * A grid map consists of 4 frames and each frame has a set of arbitrary
 * boundaries
 */
typedef struct {
    // X index of current frame within grid map
    u_char frameX; 
    // Y index of current frame within grid map
    u_char frameY;
    // The amount of collision blocks within this frame.
    u_char amount;
    // Array of bounds
    RECT* bounds;
} FrameCoords;

/*
 * Struct holding boundaries for an entire map. 
 */
typedef struct {
    // The level we are at
    u_char level;
    // Array of frames in this map (default is 4 frames)
    FrameCoords* frameCoords;
} MapCoords;

/*
 * Initialize boundaries for a new map
 * @param index of the level we want boundaries for
 */
void mapbounds_init(u_char level);

/*
 * Allocates a new FrameBounds struct.
 * @param bounds the RECT boundaries array for the current frame
 * @param frameX x coords of current map frame
 * @param frameY y coords of current map frame
 * @param amount size of the bounds array.
 * @return the allocated FrameBounds variable.
 */
FrameCoords* mapbounds_addFrameBoundaries(RECT* bounds, u_char frameX, u_char frameY, u_char amount);
void mapbounds_free(u_char level);


#endif
