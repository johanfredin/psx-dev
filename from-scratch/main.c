#include <stdlib.h>
#include <stdio.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>
#include <libetc.h>
#include "sys/types.h"

// Constants
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define BUFFERS 2
#define MODE_NTSC 0

// Globals
DISPENV dispenv[2];
DRAWENV drawenv[2];
POLY_F3 polyf3;
u_char currentBuffer = 0;

// Prototypes
void initGraphics();
void draw();
void display();
void initDisplayAndDrawEnvs();

int main() {
    initGraphics();

    while(1) {
        draw();
        display();
    }

    return 0;
}

// Definitions

void initGraphics() {
    SetDispMask(1);
	ResetGraph(0);
    FntLoad(960, 512);
    SetDumpFnt(FntOpen(5, 5, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 512));
    GsInitGraph(SCREEN_WIDTH, SCREEN_HEIGHT, GsINTER|GsOFSGPU, 0, 0); //Set up interlation..
    initDisplayAndDrawEnvs();
}

void initDisplayAndDrawEnvs() {
    

    u_short i;
    for(i = 0; i < BUFFERS; i++) {
        SetDefDispEnv(&dispenv[i], 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        SetDefDrawEnv(&drawenv[i], 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    }
}

void draw() {
    FntPrint("Hello Wolrd");
    VSync(0);
    DrawSync(0);
    currentBuffer = (currentBuffer + 1) % 2;
    PutDispEnv(&dispenv[currentBuffer]);
    PutDrawEnv(&drawenv[currentBuffer]);
}

void display() {
    // FntFlush(-1);
}