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
#define NUM_FRAMES 4
#define __ramsize   0x00200000 // Force 2MB vram
#define __stacksize 0x00004000

Color backgroundColor;
GsSPRITE* hero;
Frame* map[NUM_FRAMES/2][NUM_FRAMES/2];

u_int currentKeyDown = 0;
u_long* assets[9];

// Prototypes
void update();
void draw();
void loadCDRomAssets();
void initMap();

int main() {
    backgroundColor.r = 0;
    backgroundColor.g = 0;
    backgroundColor.b = 0;
    initializeHeap();
    initializeScreen(SCREEN_WIDTH, SCREEN_HEIGHT, &backgroundColor);
    initializeDebugFont();
    loadCDRomAssets();

    initMap();
    setBounds(SCREEN_WIDTH, SCREEN_HEIGHT);
    hero = assetmanager_loadSprite("PSY_8", assets[8], 200, 50, 128, COLOR_BITS_4);

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

void initMap() {
    map[0][0] = initFrame(assets[0], assets[4], 0, 0);
    map[0][1] = initFrame(assets[1], assets[5], 0, 1);
    map[1][0] = initFrame(assets[2], assets[6], 1, 0);
    map[1][1] = initFrame(assets[3], assets[7], 1, 1);
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
    FntPrint("x=%d, y=%d\ncurrXF=%d, currYF=%d", hero->x, hero->y, currXFrame, currYFrame);
    drawMap(map);
    GsSortFastSprite(hero, currentOT(), 1);
}
