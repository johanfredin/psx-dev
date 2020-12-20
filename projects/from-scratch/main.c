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
#include "GridMapHandler.h"

// Constants
#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 256
#define SPEED 4
#define __ramsize   0x00200000 // Force 2MB vram
#define __stacksize 0x00004000

Color backgroundColor;
GsSPRITE* hero;
u_int currentKeyDown = 0;
u_long* assets[9];

// Prototypes
void update();
void draw();
void loadCDRomAssets();

int main() {
    DrawBounds = 0;
    PrintCoords = 0;
    setBounds(SCREEN_WIDTH, SCREEN_HEIGHT);
    backgroundColor.r = 0;
    backgroundColor.g = 0;
    backgroundColor.b = 0;
    initializeHeap();
    initializeScreen(&backgroundColor);
    initializeDebugFont(0);
    loadCDRomAssets();

    setBounds(SCREEN_WIDTH, SCREEN_HEIGHT);
    hero = assetmanager_loadSprite("PSY_8", assets[8], 200, 50, 128, COLOR_BITS_4);
    initMap(assets, 0, 4, 1, 5, 2, 6, 3, 7);
    
    while(1) {
        update();
        tickMap(hero);
        draw();
        display(&backgroundColor);
        clearDisplay();
    }
    return 0;
}

// Definitions -----------------------------------------
void loadCDRomAssets() {
    // int count = 0;
    CdOpen();
    CdReadFile((u_char*)"00BG.TIM", &assets[0]);
    CdReadFile((u_char*)"01BG.TIM", &assets[1]);
    CdReadFile((u_char*)"10BG.TIM", &assets[2]);
    CdReadFile((u_char*)"11BG.TIM", &assets[3]);
    CdReadFile((u_char*)"00FG.TIM", &assets[4]);
    CdReadFile((u_char*)"01FG.TIM", &assets[5]);
    CdReadFile((u_char*)"10FG.TIM", &assets[6]);
    CdReadFile((u_char*)"11FG.TIM", &assets[7]);
    CdReadFile((u_char*)"PSY_8.TIM", &assets[8]);
    CdClose();
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
    hero->x += xSpeed;
    hero->y += ySpeed;
}

void draw() {
    currentBuffer = GsGetActiveBuff();
    if(PrintCoords) {
        FntPrint("x=%d, y=%d\ncurrXF=%d, currYF=%d\n", hero->x, hero->y, currXFrame, currYFrame);
    }
    drawMap();
    GsSortFastSprite(hero, currentOT(), 1);
}
