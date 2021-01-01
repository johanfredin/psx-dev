#ifndef _gpubase_h_
#define _gpubase_h_

#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBETC.H>
#include <STRINGS.H>

// Constants
#define NUM_BUFFERS 2
#define OT_LENGTH 5
#define BUFFERS_LENGHT 1
#define MODE_NTSC 0
#define MODE_PAL 1
#define ROT_ONE 4096
#define PACKETMAX 3000
#define FRAME_BUFFER_WIDTH 1024
#define FRAME_BUFFER_HEIGHT 512


// Globals
typedef struct {
    u_short r;
    u_short g;
    u_short b;
} Color;

u_short screenWidth; 
u_short screenHeight;

// Prototypes
void setBounds(u_short w, u_short h);
void initializeScreen(Color* bgColor);
void clearVRAM();
void initializeDebugFont(u_char bg);
void display(Color* backgroundColor);
void clearDisplay();
void initDisplayAndDrawEnvs();
void initializeHeap();
GsOT* currentOT();

#endif