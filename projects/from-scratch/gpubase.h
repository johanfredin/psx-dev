#ifndef _gpubase_h_
#define _gpubase_h_

#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBETC.H>
#include <STRINGS.H>
#include <SYS/TYPES.H>

// Constants
#define NUM_BUFFERS 2
#define BUFFERS_LENGHT 1
#define MODE_NTSC 0
#define MODE_PAL 1
#define PACKETMAX 300
#define FRAME_BUFFER_WIDTH 1024
#define FRAME_BUFFER_HEIGHT 512

// Globals
typedef struct {
    u_short r;
    u_short g;
    u_short b;
} Color;

GsOT orderingTable[NUM_BUFFERS];
GsOT_TAG minorOrderingTable[NUM_BUFFERS][1<<BUFFERS_LENGHT];
PACKET GPUOutputPacket[NUM_BUFFERS][PACKETMAX];
u_char currentBuffer = 0;

DISPENV dispenv[2];
DRAWENV drawenv[2];

// Prototypes
void initializeScreen(u_short screenWidth, u_short screenHeight, Color* bgColor);
void clearVRAM();
void initializeDebugFont();
void display();
void clearDisplay();
void initDisplayAndDrawEnvs();
void initializeHeap();

void initializeScreen(u_short screenWidth, u_short screenHeight, Color* bgColor) {
    SetVideoMode(1);
    SetDispMask(1); // 1=MASK on
	ResetGraph(0);  // Initialise drawing engine, 0=Complete reset
    clearVRAM(bgColor);    

    // Resets libgpu and initializes the graphic system. libpgu settings are maintaned by 
    // the global variables GsDISPENV and GsDRAWENV
    GsInitGraph(screenWidth, screenHeight, GsINTER|GsOFSGPU, 0, 0); 
    
    // Define double buffers
    GsDefDispBuff(0, 0, 0, screenHeight);

    // Initialize display and drawing environments
    SetDefDispEnv(&dispenv[0], 0, 0, screenWidth, screenHeight);
    SetDefDrawEnv(&drawenv[0], 0, 0, screenWidth, screenHeight);
    SetDefDispEnv(&dispenv[1], 0, 0, screenWidth, screenHeight);
    SetDefDrawEnv(&drawenv[1], 0, 0, screenWidth, screenHeight);

    drawenv[0].isbg = 0;
    drawenv[1].isbg = 0;

    // Initialize ordering tables
    GsClearOt(0, 0, &orderingTable[GsGetActiveBuff()]);
    orderingTable[0].length = BUFFERS_LENGHT;
    orderingTable[0].org = minorOrderingTable[0];
    orderingTable[1].length = BUFFERS_LENGHT;
    orderingTable[1].org = minorOrderingTable[1];
    GsClearOt(0, 0, &orderingTable[0]);
    GsClearOt(0, 0, &orderingTable[1]);

    PadInit(MODE_NTSC);
}

void clearVRAM(Color* bgColor) {
    RECT rect;
    setRECT(&rect, 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
    ClearImage2(&rect, bgColor->r, bgColor->g, bgColor->b);    // Clear Frame Buffer at high speed (interlaced mode)
    DrawSync(0);    // Waits for ClearImage2 function to finish
}

void initializeDebugFont() {
	FntLoad(960, 256);
	SetDumpFnt(FntOpen(5, 20, 320, 240, 0, 512)); //Sets the dumped font for use with FntPrint();
}

void display(Color* backgroundColor) {
    currentBuffer = GsGetActiveBuff();
    DrawSync(0);        // Wait for drawing to end
    VSync(0);           // Wait for vsync (2 = 30FPS moreless)
    GsSwapDispBuff();   // Swap buffers
    
    // Sets a screen clear command at the start of the OT. Should be called after GsSwapDispBuff.
    // Actual clearing isnt executed unti GsDrawOt() is called
    GsSortClear(backgroundColor->r, backgroundColor->g, backgroundColor->b, &orderingTable[currentBuffer]);
    // Starts execution of GPU commands registered in OT
    GsDrawOt(&orderingTable[currentBuffer]);
}

void clearDisplay() {
    // currentBuffer = GsGetActiveBuff();
    FntFlush(-1);
    // Set the memory adress for storing drawing commands for drawing primitives generated by functions
    // like GsSortObject(), GsSortSprite() and GsSortBg()
    GsSetWorkBase((PACKET*)GPUOutputPacket[currentBuffer]);
    GsClearOt(0, 0, &orderingTable[currentBuffer]);
}

void initializeHeap() {
	printf("\nReserving 1024KB (1,048,576 Bytes) RAM... \n");
    InitHeap3((void*)0x800F8000, 0x00100000);
    printf("Success!\n");
}

#endif