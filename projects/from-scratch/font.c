#include "header/font.h"

u_long* fntAsset;
GsSPRITE* fntSprite;

void font_load() {

    CdReadFile("FONT.TIM", &fntAsset, NULL);
    assetmanager_loadSprite("Font", fntAsset, 25, 25, 255, COLOR_BITS_8);
}

void font_draw() {
    GsSortFastSprite(fntSprite, currentOT(), 1);
}