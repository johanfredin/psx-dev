#include <stdlib.h>
#include <stdio.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>
#include <LIBCD.H>
#include <LIBDS.H>
#include <libetc.h>
#include <sys/types.h>

// Constants
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define NUM_BUFFERS 2
#define BUFFERS_LENGHT 1
#define MODE_NTSC 0
#define PACKETMAX 300
#define FRAME_BUFFER_WIDTH 1024
#define FRAME_BUFFER_HEIGHT 512
#define SPEED 4

#define __ramsize   0x00200000 // Force 2MB vram
#define __stacksize 0x00004000

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
Color backgroundColor;

DISPENV dispenv[2];
DRAWENV drawenv[2];
POLY_FT4 polyFt4;

u_long currentKeyDown = 0;

// Prototypes
void initializeScreen();
void clearVRAM();
void update();
void initializeDebugFont();
void draw();
void display();
void clearDisplay();
void initDisplayAndDrawEnvs();
void initPlayer(u_short x, u_short y, u_short w, u_short h, u_short r, u_short g, u_short b);
void initGameObjects();

int main() {
    backgroundColor.r = 66;
    backgroundColor.g = 200;
    backgroundColor.b = 100;
    initializeScreen();
    initializeDebugFont();
    initPlayer(100, 50, 28, 48, 50, 10, 200);
    initGameObjects();

    while(1) {
        update();
        draw();
        display();
        clearDisplay();
    }

    return 0;
}

// Definitions -----------------------------------------

void initializeScreen() {
    SetVideoMode(MODE_NTSC);
    SetDispMask(1); // 1=MASK on
	ResetGraph(0);  // Initialise drawing engine, 0=Complete reset
    clearVRAM();    

    // Resets libgpu and initializes the graphic system. libpgu settings are maintaned by 
    // the global variables GsDISPENV and GsDRAWENV
    GsInitGraph(SCREEN_WIDTH, SCREEN_HEIGHT, GsINTER|GsOFSGPU, 0, 0); 
    
    // Define double buffers
    GsDefDispBuff(0, 0, 0, SCREEN_HEIGHT);

    // Initialize display and drawing environments
    SetDefDispEnv(&dispenv[0], 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    SetDefDrawEnv(&drawenv[0], 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    SetDefDispEnv(&dispenv[1], 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    SetDefDrawEnv(&drawenv[1], 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

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

void clearVRAM() {
    RECT rect;
    setRECT(&rect, 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
    ClearImage2(&rect, backgroundColor.r, backgroundColor.g, backgroundColor.b);    // Clear Frame Buffer at high speed (interlaced mode)
    DrawSync(0);    // Waits for ClearImage2 function to finish
}

void initializeDebugFont() {
	FntLoad(960, 256);
	SetDumpFnt(FntOpen(5, 20, 320, 240, 0, 512)); //Sets the dumped font for use with FntPrint();
}

void initPlayer(u_short x, u_short y, u_short w, u_short h, u_short r, u_short g, u_short b) {
    SetPolyFT4(&polyFt4);
    polyFt4.x0 = x;
    polyFt4.y0 = y;
    polyFt4.x1 = x + w;
    polyFt4.y1 = y;
    polyFt4.x2 = x;
    polyFt4.y2 = y + h;
    polyFt4.x3 = x + w;
    polyFt4.y3 = y + h;
    setRGB0(&polyFt4, r, g, b);
}

void initGameObjects() {
    // No objects yet...
}

void update() {
    short xSpeed = 0;
    short ySpeed = 0;
    currentKeyDown = PadRead(0);
    if(currentKeyDown & PADLdown) {
        ySpeed = SPEED;
    } if(currentKeyDown & PADLup) {
        ySpeed = -SPEED;
    } if(currentKeyDown & PADLright) {
        xSpeed = SPEED;
    } if(currentKeyDown & PADLleft) {
        xSpeed = -SPEED;
    }
    polyFt4.x0 += xSpeed;
    polyFt4.x1 += xSpeed;
    polyFt4.x2 += xSpeed;
    polyFt4.x3 += xSpeed;
    polyFt4.y0 += ySpeed;
    polyFt4.y1 += ySpeed;
    polyFt4.y2 += ySpeed;
    polyFt4.y3 += ySpeed;
}

void draw() {
    FntPrint("Hello World, padDown=%d", currentKeyDown);
    PutDrawEnv(&drawenv[currentBuffer]);
    PutDispEnv(&dispenv[currentBuffer]);
    DrawPrim(&polyFt4);
}

void display() {
    currentBuffer = GsGetActiveBuff();
    DrawSync(0);        // Wait for drawing to end
    VSync(0);           // Wait for vsync (2 = 30FPS moreless)
    GsSwapDispBuff();   // Swap buffers
    
    // Sets a screen clear command at the start of the OT. Should be called after GsSwapDispBuff.
    // Actual clearing isnt executed unti GsDrawOt() is called
    GsSortClear(backgroundColor.r, backgroundColor.g, backgroundColor.b, &orderingTable[currentBuffer]);
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