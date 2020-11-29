#include <STDLIB.H>
#include <STDIO.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBETC.H>
#include <LIBDS.H>
#include <STRINGS.H>
#include <SYS/TYPES.H>

// Custom imports
#include "cdrom.h"
#include "gpubase.h"
#include "assetmanager.h"

// Constants
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 256
#define SPEED 4
#define __ramsize   0x00200000 // Force 2MB vram
#define __stacksize 0x00004000

Color backgroundColor;
GsSPRITE* sprites[4];
u_int currentKeyDown = 0;
u_long* assets[4];

// Prototypes
void update();
void draw();
void initPlayer(u_short x, u_short y, u_short r, u_short g, u_short b, u_short numColorBits);
void loadCDRomAssets();
void logCoords(RECT* rect, char* source);

int main() {
    backgroundColor.r = 0;
    backgroundColor.g = 0;
    backgroundColor.b = 0;
    initializeHeap();
    initializeScreen(SCREEN_WIDTH, SCREEN_HEIGHT, &backgroundColor);
    initializeDebugFont();
    loadCDRomAssets();
    sprites[0] = assetmanager_loadSprite("JULIA_4", assets[0], 200, 50, 128, 128, 128, COLOR_BITS_4);
    // sprites[1] = assetmanager_loadSprite("MAP_8", assets[1], 0, 0, 128, 128, 128, COLOR_BITS_8);
    sprites[1] = assetmanager_loadSprite("PSY_8", assets[1], 170, 0, 128, 128, 128, COLOR_BITS_8);
    sprites[2] = assetmanager_loadSprite("BG_8", assets[2], 0, 0, 128, 128, 128, COLOR_BITS_8);
    sprites[3] = assetmanager_loadSprite("FG_8", assets[3], 0, 0, 128, 128, 128, COLOR_BITS_8);

    while(1) {
        update();
        draw();
        display(&backgroundColor);
        clearDisplay();
    }

    return 0;
}

// Definitions -----------------------------------------
void loadCDRomAssets() {
    CdOpen();
    CdReadFile("JULIA_4.TIM", &assets[0]);
    // CdReadFile("MAP_8.TIM", &assets[1]);
    CdReadFile("PSY_8.TIM", &assets[1]);
    CdReadFile("BG_8.TIM", &assets[2]);
    CdReadFile("FG_8.TIM", &assets[3]);
    CdClose();
}

u_char AMONGO = 0;
// u_char MAP = 1;
u_char PSY = 1;
u_char BG = 2;
u_char FG = 3;

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
    sprites[PSY]->x += xSpeed;
    sprites[PSY]->y += ySpeed;
}

void draw() {
    FntPrint("x=%d, y=%d", sprites[PSY]->x, sprites[PSY]->y);
    currentBuffer = GsGetActiveBuff();
    GsSortSprite(sprites[AMONGO], &orderingTable[currentBuffer],1);
    GsSortSprite(sprites[PSY], &orderingTable[currentBuffer], 1);
    GsSortSprite(sprites[BG], &orderingTable[currentBuffer], 2);
    GsSortSprite(sprites[FG], &orderingTable[currentBuffer],0);
}

