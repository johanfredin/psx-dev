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
#include "header/player.h"

// Constants
#define SPEED 3

Color backgroundColor;
Player* player;
u_int currentKeyDown = 0;
u_long* assets[9];

void update();
void draw();
void loadCDRomAssets();

int main() {
    AnimatedGameObject* gobj;
    setBounds(SCREEN_WIDTH, SCREEN_HEIGHT);
    backgroundColor.r = 0;
    backgroundColor.g = 0;
    backgroundColor.b = 0;
    initializeHeap();
    initializeScreen(&backgroundColor);
    initializeDebugFont(0);
    loadCDRomAssets();

    gobj = animatedobject_set("HERO", assets[8], 200, 50, 16, 16, 220, 3, 3, COLOR_BITS_8);
    player = player_init(gobj, 0, SPEED, SPEED);
    gridmap_init(assets, 0, 4, 1, 5, 2, 6, 3, 7);
    
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
    player_tick(player); 
    gridmap_tick(player);
}

void draw() {
    gridmap_draw();
    player_draw(player);
}
