#ifndef _assetmanager_
#define _assetmanager_

#include "gpubase.h"
#include "mem.h"
#include "base.h"
#include "logger.h"

#define ROT_ONE 4096
#define COLOR_BITS_4 4
#define COLOR_BITS_8 8
#define COLOR_BITS_16 16

typedef struct {
    RECT* frameBuffer;
    RECT* clut;
    u_short twidthMultiplier;
    u_short colorMode;
    u_char isCLUTMode;
    u_long spriteAttr;
} Asset;

void assetmanager_loadAsset(Asset* asset, char* name, u_long* spriteData, u_short numColorBits);
GsSPRITE* assetmanager_loadSprite(char* name, u_long* spriteData, u_short x, u_short y, u_short blend, u_short numColorBits);

#endif