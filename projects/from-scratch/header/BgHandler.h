#ifndef _BgHandler_
#define _BgHandler_

/*
Havent yet figured scolling out, so keeping what i have so far as reference
*/

#include "assetmanager.h"

GsBG* gsBG;
u_long* assets[4];

void loadBG();

void loadBG() {
    GsCELL gsCells[10*3];
    GsMAP gsMap;
    Asset asset;
    u_short xCoord;
    u_short yCoord;
    u_short tileW = 16;
    u_short tileH = 16;
    u_short mapW;
    u_short mapH;
    u_short index;
    gsBG = (GsBG*) malloc3(sizeof(GsBG));
    assetmanager_loadAsset(&asset, "TILES_8", assets[0], COLOR_BITS_8);
    
    mapW = asset.frameBuffer->w;
    mapH = asset.frameBuffer->h;

    // Fetch cells
    index = 0;
    printf("\n------------------------------\nStarting retrieval of map cells\n------------------------------\n");
    for(yCoord = 0; yCoord < mapH; yCoord += tileH) {
        for(xCoord = 0; xCoord < mapW; xCoord += tileW) {
            GsCELL cell;
            cell.u = xCoord / 2;
            cell.v = yCoord;
            cell.cba = GetClut(asset.clut->x, asset.clut->y);
            cell.flag = 0;
            cell.tpage = GetTPage(asset.colorMode, 1, asset.frameBuffer->x, asset.frameBuffer->y);
            gsCells[index] = cell;
            index++;
            printf("Cell added at xCoord=%d, yCoord=%d: u=%d, v=%d, cx=%d, cy=%d, index=%d\n", xCoord, yCoord, cell.u, cell.v, asset.clut->x, asset.clut->y, index);
        }    
    }

    // Setup map
    gsMap.cellw = 16;
    gsMap.cellh = 16;
    gsMap.ncellw = 10;
    gsMap.ncellh = 3;
    gsMap.base = gsCells;
    gsMap.index = &gsCells[0];
    printf("\n------------------------------\nMap stats: {cellW:%d, cellH:%d, nCellW:%d, nCellH:%d, base:%p, index:%d}\n", gsMap.cellw, gsMap.cellh, gsMap.ncellw, gsMap.ncellh, gsMap.base, gsMap.index);

    // Setup BG
    gsBG->attribute = asset.spriteAttr;
    gsBG->x = 0;
    gsBG->y = 0;
    gsBG->w = asset.frameBuffer->w * asset.twidthMultiplier;
    gsBG->h = asset.frameBuffer->h;
    gsBG->mx = 0;
    gsBG->my = 0;
    gsBG->scalex = ONE * 1;
    gsBG->scaley = ONE * 1;
    gsBG->rotate = ROT_ONE * 0;
    gsBG->r = gsBG->g = gsBG->b = 128;
    gsBG->scrollx = 0;
    gsBG->scrolly = 0;
    gsBG->map = &gsMap;
}

#endif