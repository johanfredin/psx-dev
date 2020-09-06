#include <stdlib.h>
#include <LIBGTE.H>
#include <libgpu.h>
#include <LIBGS.H>

#include "constants.h"

void initialize();
void display();


GsOT orderingTable[2];
short currentBuffer;
char fullText[100] = "Current loop: ";
int loopCounter = 0;


int main() {
    initialize();

    while (1) {
        loopCounter++;
        sprintf(fullText, "Current loop: %d", loopCounter);
        FntPrint(fullText);
        display();
    }

    return 0;
}

void initialize() {
    initializeScreen();
    initializeDebugFont();
}

void display() {
    currentBuffer = GsGetActiveBuff();
    FntFlush(-1);
    GsClearOt(0, 0, &orderingTable[currentBuffer]);
    DrawSync(0);
    VSync(0);
    GsSwapDispBuff();
    GsSortClear(0, 0, 255, &orderingTable[currentBuffer]);
    GsDrawOt(&orderingTable[currentBuffer]);
}