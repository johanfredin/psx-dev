#ifndef _constanst_h_
#define _constanst_h_

#include <KERNEL.H>
#include <STDIO.H>

/*
 * Constant values used throughout game
 */

// Width of screen
#define SCREEN_WIDTH 256
// Height of screen
#define SCREEN_HEIGHT 256
#define __ramsize   0x00200000 // Force 2MB vram
#define __stacksize 0x00004000
// Set to 1 to draw map bounds as red rectangles 
#define DRAW_BOUNDS 0
// Set to 1 to FntPrint player coordinates to screen
#define PRINT_COORDS 0
// Set to 1 to FntPrint player animation to screen
#define PRINT_ANIMATION 0

#endif