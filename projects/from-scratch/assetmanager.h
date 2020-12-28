#ifndef _assetmanager_
#define _assetmanager_

#include <STDLIB.H>
#include <STDIO.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBETC.H>
#include <LIBDS.H>
#include <STRINGS.H>
#include <SYS/TYPES.H>

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
u_char getTPageColorBitMode(u_short numColorBits);
u_char getWidthByColorBitsMode(u_short numColorBits);
u_long getAttributeByColorBitsMode(u_short numColorBits);

void assetmanager_loadAsset(Asset* asset, char* name, u_long* spriteData, u_short numColorBits) {
    // Declarations
    GsIMAGE* tim_data;
    RECT* frameBuffer;
    RECT* clut;
    u_char* data;
    
    // Definitions
    u_short twidthMultiplier = getWidthByColorBitsMode(numColorBits);
    u_short colorMode = getTPageColorBitMode(numColorBits);
    u_char isCLUTMode = numColorBits < COLOR_BITS_16;
    u_long spriteAttr = getAttributeByColorBitsMode(numColorBits);

    printf("======================================================\n");
    printf("Started fetching asset %s:\n-----------------------------\n", name);
    printf("Color bits=%d, CLUT mode=%d\n", numColorBits, isCLUTMode);

    // Load image data
    data = (u_char*) spriteData;

    tim_data = malloc3(sizeof(GsIMAGE));
    GsGetTimInfo((u_long*)(data
     + 4), tim_data);
        
    // malloc resources
    frameBuffer = malloc3(sizeof(RECT));
    clut = malloc3(sizeof(RECT));

    // Load image into gpu memory
    frameBuffer->x = tim_data->px;
    frameBuffer->y = tim_data->py;
    frameBuffer->w = tim_data->pw;
    frameBuffer->h = tim_data->ph;
    LoadImage(frameBuffer, tim_data->pixel);
    logger_logCoords(frameBuffer, "Framebuffer");
   
   if(isCLUTMode) {
        // load clut into gpu memory
        clut->x = tim_data->cx;
        clut->y = tim_data->cy;
        clut->w = tim_data->cw;
        clut->h = tim_data->ch;
        LoadImage(clut, tim_data->clut);
        logger_logCoords(clut, "CLUT");
    } else {
        printf("16 bit mode so no CLUT\n");
    }

    free3(tim_data);
    asset->clut=clut;
    asset->frameBuffer=frameBuffer;
    asset->isCLUTMode=isCLUTMode;
    asset->colorMode=colorMode;
    asset->spriteAttr=spriteAttr;
    asset->twidthMultiplier=twidthMultiplier;
}

GsSPRITE* assetmanager_loadSprite(char* name, u_long* spriteData, u_short x, u_short y, u_short blend, u_short numColorBits) {
    Asset* asset;
    GsSPRITE* sprite;

    asset = (Asset*) malloc3(sizeof(Asset));
    assetmanager_loadAsset(asset, name, spriteData, numColorBits);
    sprite = (GsSPRITE*) malloc3(sizeof(GsSPRITE));
    sprite->attribute = asset->spriteAttr;
    sprite->x = x;
    sprite->y = y;
    sprite->w = asset->frameBuffer->w *  asset->twidthMultiplier;
    sprite->h = asset->frameBuffer->h;


    sprite->tpage = GetTPage(asset->colorMode, 1, asset->frameBuffer->x, asset->frameBuffer->y);
    sprite->u = 0;//(frameBuffer->x * twidthMultiplier) % 256;
    sprite->v = (asset->frameBuffer->y) % 256;
    if(asset->isCLUTMode) {
        sprite->cx = asset->clut->x;
        sprite->cy = asset->clut->y;
    }
    sprite->r = sprite->g = sprite->b = blend;
    sprite->rotate = ROT_ONE * 0;            // Rotation, ROT_ONE * (0 to 360) 
	sprite->mx = 0;                          // Rotation x coord
	sprite->my = 0;                          // Rotation y coord
    sprite->scalex = ONE * 1;
    sprite->scaley = ONE * 1;

    printf("w=%d, h=%d\n", sprite->w, sprite->h);
    printf("cx=%d, cy=%d\n", sprite->cx, sprite->cy);
    printf("u=%d, v=%d\n", sprite->u, sprite->v);

    free3(asset->frameBuffer);
    free3(asset->clut);
    free3(asset);
    // DrawSync(0);
    return sprite;
}

u_char getTPageColorBitMode(u_short numColorBits) {
    switch (numColorBits) {
    case COLOR_BITS_4:
        return 0;
    case COLOR_BITS_8:
        return 1;
    case COLOR_BITS_16:
        return 2;
    }
    return -1;
}

u_char getWidthByColorBitsMode(u_short numColorBits) {
    switch (numColorBits) {
    case COLOR_BITS_8:
        return 2;
    case COLOR_BITS_16:
        return 1;
    }
    return numColorBits;
}

u_long getAttributeByColorBitsMode(u_short numColorBits) {
    switch (numColorBits) {
    case COLOR_BITS_4:
        return 0x0000000;
    case COLOR_BITS_8:
        return 0x1000000;
    case COLOR_BITS_16:
        return 0x2000000;
    }
    return -1;
}

#endif