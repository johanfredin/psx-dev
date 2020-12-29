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
#include "gridmap.h"
#include "AnimatedGameObject.h"

// Constants
#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 256
#define SPEED 3
#define __ramsize   0x00200000 // Force 2MB vram
#define __stacksize 0x00004000

Color backgroundColor;
AnimatedGameObject* player;
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
    player = setGameObject("HERO", assets[8], 200, 50, 16, 16, 220, 3, 3, COLOR_BITS_8);
    gridmap_init(assets, 0, 4, 1, 5, 2, 6, 3, 7);
    
    while(1) {
        update();
        gridmap_tick(player->textureFrame);
        draw();
        display(&backgroundColor);
        clearDisplay();
    }
    return 0;
}

// Definitions -----------------------------------------
void loadCDRomAssets() {
    CdOpen();
    CdReadFile((u_char*)"00BG.TIM", &assets[0]);
    CdReadFile((u_char*)"01BG.TIM", &assets[1]);
    CdReadFile((u_char*)"10BG.TIM", &assets[2]);
    CdReadFile((u_char*)"11BG.TIM", &assets[3]);
    CdReadFile((u_char*)"00FG.TIM", &assets[4]);
    CdReadFile((u_char*)"01FG.TIM", &assets[5]);
    CdReadFile((u_char*)"10FG.TIM", &assets[6]);
    CdReadFile((u_char*)"11FG.TIM", &assets[7]);
    CdReadFile((u_char*)"HERO.TIM", &assets[8]);
    CdClose();
}

void update() {
    short xSpeed = 0;
    short ySpeed = 0;
    setHeading(player, 0, 0, 0, 0);
    currentKeyDown = PadRead(0);
    if(currentKeyDown & PADLdown) {
        ySpeed = SPEED;
        setHeading(player, 0, 0, 0, 1);
    } if(currentKeyDown & PADLup) {
        ySpeed = -SPEED;
        setHeading(player, 0, 0, 1, 0);
    } if(currentKeyDown & PADLright) {
        xSpeed = SPEED;
        setHeading(player, 0, 1, 0, 0);
    } if(currentKeyDown & PADLleft) {
        xSpeed = -SPEED;
        setHeading(player, 1, 0, 0, 0);
    } 
    player->textureFrame->x += xSpeed;
    player->textureFrame->y += ySpeed;
    updateGameObject(player); 
}

void draw() {
    currentBuffer = GsGetActiveBuff();
    if(PrintCoords) {
        FntPrint("x=%d, y=%d\ncurrXF=%d, currYF=%d\n", player->textureFrame->x, player->textureFrame->y, currXFrame, currYFrame);
        FntPrint("left=%d, right=%d, up=%d, down=%d\n", player->heading.left, player->heading.right, player->heading.up, player->heading.down);
    }
    gridmap_draw();
    GsSortFastSprite(player->textureFrame, currentOT(), 1);
}
