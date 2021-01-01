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
#include "header/base.h"
#include "header/cdrom.h"
#include "header/gpubase.h"
#include "header/assetmanager.h"
#include "header/gridmap.h"
#include "header/animatedobject.h"

// Constants
#define SPEED 3

Color backgroundColor;
AnimatedGameObject* player;
u_int currentKeyDown = 0;
u_long* assets[9];

void update();
void draw();
void loadCDRomAssets();

int main() {
    setBounds(SCREEN_WIDTH, SCREEN_HEIGHT);
    backgroundColor.r = 0;
    backgroundColor.g = 0;
    backgroundColor.b = 0;
    initializeHeap();
    initializeScreen(&backgroundColor);
    initializeDebugFont(0);
    loadCDRomAssets();

    player = animatedobject_set("HERO", assets[8], 200, 50, 16, 16, 220, 3, 3, COLOR_BITS_8);
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
    CdReadFile("00BG.TIM", &assets[0]);
    CdReadFile("01BG.TIM", &assets[1]);
    CdReadFile("10BG.TIM", &assets[2]);
    CdReadFile("11BG.TIM", &assets[3]);
    CdReadFile("00FG.TIM", &assets[4]);
    CdReadFile("01FG.TIM", &assets[5]);
    CdReadFile("10FG.TIM", &assets[6]);
    CdReadFile("11FG.TIM", &assets[7]);
    CdReadFile("HERO.TIM", &assets[8]);
    CdClose();
}

void update() {
    short xSpeed = 0;
    short ySpeed = 0;
    animatedobject_setHeading(player, 0, 0, 0, 0);
    currentKeyDown = PadRead(0);
    if(currentKeyDown & PADLdown) {
        ySpeed = SPEED;
        animatedobject_setHeading(player, 0, 0, 0, 1);
    } if(currentKeyDown & PADLup) {
        ySpeed = -SPEED;
        animatedobject_setHeading(player, 0, 0, 1, 0);
    } if(currentKeyDown & PADLright) {
        xSpeed = SPEED;
        animatedobject_setHeading(player, 0, 1, 0, 0);
    } if(currentKeyDown & PADLleft) {
        xSpeed = -SPEED;
        animatedobject_setHeading(player, 1, 0, 0, 0);
    } 
    animatedobject_tick(player, xSpeed, ySpeed); 
}

void draw() {
    if(PRINT_COORDS) {
        FntPrint("x=%d, y=%d\ncurrXF=%d, currYF=%d\n", player->textureFrame->x, player->textureFrame->y, gridmap_currXFrame, gridmap_currYFrame);
        FntPrint("left=%d, right=%d, up=%d, down=%d\n", player->heading.left, player->heading.right, player->heading.up, player->heading.down);
    }
    gridmap_draw();
    GsSortFastSprite(player->textureFrame, currentOT(), 1);
}
