#include <stdlib.h>
#include <stdio.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>
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
#define left 32768
#define right 8192
#define up 4096
#define down 16384

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
POLY_F3 polyf3;

// Our awesome hero
typedef struct {
    POLY_F3 poly;
    u_short x, y, w, h;
} Player;

u_long currentKeyDown = 0;
Player player;

// Prototypes
void initializeScreen();
void clearVRAM();
void update();
void updatePlayer(short xSpeed, short ySpeed);
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
    SetPolyF3(&polyf3);
    setXY3(&polyf3, x, y, x + w, y + h, x - w, y + h);
    setRGB0(&polyf3, r, g, b);
    player.poly = polyf3;
    player.x = x;
    player.y = y;
    player.w = w;
    player.h = h;
}

void initGameObjects() {
    // No objects yet...
}

void update() {
    currentKeyDown = PadRead(0);
    switch(currentKeyDown) {
        case down:
            FntPrint("DOWN");
            updatePlayer(0, SPEED); 
        case up:
            FntPrint("UP");
            updatePlayer(0, -SPEED);
        case right:
            updatePlayer(SPEED, 0);
            FntPrint("RIGHT");
        case left:
            updatePlayer(-SPEED, 0);
            FntPrint("LEFT");
    }
    updatePlayer(0, 0);
}

void updatePlayer(short xSpeed, short ySpeed) {
    player.x += xSpeed;
    player.y += ySpeed;
    updatePoly(player, &player.poly);
}

void updatePoly(Player player, POLY_F3* poly) {
    setXY3(poly, player.x, player.y, player.x + player.w, player.y + player.h, player.x - player.w, player.y + player.h);
}

void draw() {
    DrawPrim(&player.poly);
    FntPrint("Hello World, padDown=%d", currentKeyDown);
}

void display() {
    currentBuffer = GsGetActiveBuff();
    PutDispEnv(&dispenv[currentBuffer]);
    PutDrawEnv(&drawenv[currentBuffer]);
    DrawSync(0);        // Wait for drawing to end
    VSync(2);           // Wait for vsync (2 = 30FPS moreless)
    GsSwapDispBuff();   // Swap buffers
    
    // Sets a screen clear command at the start of the OT. Should be called after GsSwapDispBuff.
    // Actual clearing isnt executed unti GsDrawOt() is called
    GsSortClear(backgroundColor.r, backgroundColor.g, backgroundColor.b, &orderingTable[currentBuffer]);
    // Starts execution of GPU commands registered in OT
    GsDrawOt(&orderingTable[currentBuffer]);
}

void clearDisplay() {
    currentBuffer = GsGetActiveBuff();
    FntFlush(-1);
    // Set the memory adress for storing drawing commands for drawing primitives generated by functions
    // like GsSortObject(), GsSortSprite() and GsSortBg()
    GsSetWorkBase((PACKET*)GPUOutputPacket[currentBuffer]);
    GsClearOt(0, 0, &orderingTable[currentBuffer]);
}