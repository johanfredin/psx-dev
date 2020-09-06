#include "constants.h"

void initialize();
void update();

Line line;
Box box;

// Box coords
int box_x, box_y, box_width, box_height;

int main() {
    initialize();
    initializePad();
    
    while (1) {
        update();
        draw();
        display();
    }

    return 0;
}

void initialize() {
    initializeScreen();

    box_x = 128;
    box_y = 128;
    box_width = 36;
    box_height = 36;

    setBackgroundColor(createColor(255, 255, 255));
    line = createLine(createColor(0, 0, 0), 32, 32, 64, 64);
    box = createBox(createColor(0, 0, 255), box_x, box_y, 164, 164);
}

void update() {
    padUpdate();

    if(padCheck(Pad1Up)) {
        box_y -= 2;
    }
    if(padCheck(Pad1Down)) {
        box_y += 2;
    }
    if(padCheck(Pad1Right)) {
        box_x += 2;
    }
    if(padCheck(Pad1Left)) {
        box_x -= 2;
    }

    if((box_y + box_height) >= SCREEN_HEIGHT) {
        box_y = (SCREEN_HEIGHT - box_width);
    }
    if(box_y <= 0) {
        box_y = 0;
    }
    if((box_x + box_width) >= SCREEN_WIDTH) {
        box_x = (SCREEN_WIDTH - box_width);
    }
    if(box_x <= 0) {
        box_x = 0;
    }

    box = moveBox(box, box_x, box_y);
}

void draw() {
    drawLine(line);
    drawBox(box);
}

