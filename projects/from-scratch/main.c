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

// Constants
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 256
#define SPEED 4
#define __ramsize   0x00200000 // Force 2MB vram
#define __stacksize 0x00004000

Color backgroundColor;
POLY_FT4 polyFt4;
u_int currentKeyDown = 0;

// Prototypes
void update();
void draw();
void initPlayer(u_short x, u_short y, u_short w, u_short h, u_short r, u_short g, u_short b);
void initGameObjects();

int main() {
    backgroundColor.r = 66;
    backgroundColor.g = 200;
    backgroundColor.b = 100;
    initializeHeap();
    initializeScreen(SCREEN_WIDTH, SCREEN_HEIGHT, &backgroundColor);
    initializeDebugFont();
    initPlayer(100, 50, 28, 48, 50, 10, 200);
    initGameObjects();

    while(1) {
        update();
        draw();
        display(&backgroundColor);
        clearDisplay();
    }

    return 0;
}

// Definitions -----------------------------------------
void initPlayer(u_short x, u_short y, u_short w, u_short h, u_short r, u_short g, u_short b) {
    SetPolyFT4(&polyFt4);
    polyFt4.x0 = x;
    polyFt4.y0 = y;
    polyFt4.x1 = x + w;
    polyFt4.y1 = y;
    polyFt4.x2 = x;
    polyFt4.y2 = y + h;
    polyFt4.x3 = x + w;
    polyFt4.y3 = y + h;
    setRGB0(&polyFt4, r, g, b);
}

void initGameObjects() {
    // No objects yet...
}

void update() {
    short xSpeed = 0;
    short ySpeed = 0;
    currentKeyDown = PadRead(0);
    if(currentKeyDown & PADLdown) {
        ySpeed = SPEED;
    } if(currentKeyDown & PADLup) {
        ySpeed = -SPEED;
    } if(currentKeyDown & PADLright) {
        xSpeed = SPEED;
    } if(currentKeyDown & PADLleft) {
        xSpeed = -SPEED;
    }
    polyFt4.x0 += xSpeed;
    polyFt4.x1 += xSpeed;
    polyFt4.x2 += xSpeed;
    polyFt4.x3 += xSpeed;
    polyFt4.y0 += ySpeed;
    polyFt4.y1 += ySpeed;
    polyFt4.y2 += ySpeed;
    polyFt4.y3 += ySpeed;
}

void draw() {
    FntPrint("Hello World, padDown=%d", currentKeyDown);
    PutDrawEnv(&drawenv[currentBuffer]);
    PutDispEnv(&dispenv[currentBuffer]);
    DrawPrim(&polyFt4);
}

