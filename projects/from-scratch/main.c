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

void update();
void draw();

int main() {
    u_long** heroTexture;
    AnimatedGameObject* gobj;
    setBounds(SCREEN_WIDTH, SCREEN_HEIGHT);
    backgroundColor.r = 0;
    backgroundColor.g = 0;
    backgroundColor.b = 0;
    initializeHeap();
    initializeScreen(&backgroundColor);
    initializeDebugFont(0);

    // Load hero sprite
    CdOpen();
    CdReadFile("HERO.TIM", heroTexture);
    CdClose();

    gobj = animatedobject_set("HERO", *heroTexture, 120, 150, 16, 16, 220, 3, 3, COLOR_BITS_8);
    player = player_init(gobj, 0, SPEED, SPEED);
    gridmap_init(1);
    
    while(1) {
        update();
        draw();
        display(&backgroundColor);
        clearDisplay();
    }
    return 0;
}

void update() {
    player_tick(player); 
    gridmap_tick(player);
}

void draw() {
    gridmap_draw();
    player_draw(player);
}
