#include "header/base.h"
#include "header/cdrom.h"
#include "header/assetmanager.h"
#include "header/gpubase.h"
#include "header/gameobject.h"
#include "header/gridmap.h"

// Constants
#define SPEED 3
#define MAX_PLAYERS 4

Color backgroundColor;
Player *player;
u_int currentKeyDown = 0;

void initPlayers(u_char numPlayers);
void update();
void draw();

int main() {
    setBounds(SCREEN_WIDTH, SCREEN_HEIGHT);
    backgroundColor.r = 0;
    backgroundColor.g = 0;
    backgroundColor.b = 0;
    initializeHeap();
    initializeScreen(&backgroundColor);
    initializeDebugFont(0);
    initPlayers(1);
    gridmap_init(1);

    while (1) {
        update();
        draw();
        display(&backgroundColor);
        clearDisplay();
    }
    return 0;
}

void initPlayers(u_char numPlayers) {
    u_long **heroTexture[MAX_PLAYERS]; // Reserve space for max 4 players
    int i;

    // Load hero sprites from disc
    CdOpen();
    CdReadFile("HERO.TIM", heroTexture[0]);
    CdClose();

    for (i = 0; i < numPlayers; i++) {
        GsSPRITE *sprite;
        GameObject *go;
        Animation *anim;

        sprite = assetmanager_loadSprite("Player", *heroTexture[0], 126, 128, 220, COLOR_BITS_8);
        go = gameobject_init(sprite, 16, 16, SPEED, SPEED, 100);
        anim = animation_init(0, 0, 3, 3);
        player = player_init(anim, go, 0);
    }
}

void update() {
    player_tick(player);
    gridmap_tick(player);
}

void draw() {
    gridmap_draw();
    player_draw(player);
}
