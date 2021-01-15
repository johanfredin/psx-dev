#ifndef _constanst_h_
#define _constanst_h_

#include <KERNEL.H>
#include <STDIO.H>

/*
 * Constant values used throughout game
 */

#define SCREEN_WIDTH 256    // Width of screen
#define SCREEN_HEIGHT 256   // Height of screen

#define __ramsize   0x00200000 // Force 2MB vram
#define __stacksize 0x00004000

#define DRAW_BOUNDS 0           // Set to 1 to draw map bounds as red rectangles 
#define PRINT_COORDS 1          // Set to 1 to FntPrint player coordinates to screen
#define PRINT_ANIMATION 0       // Set to 1 to FntPrint player animation to screen
#define LOG_INDIVIDUAL_BOUNDS 0 // Set to 1 to Log all asigned mab boundarie coordinates


#endif