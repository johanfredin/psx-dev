#include "constants.h"


Image crash;


int x = 0;
int y = 0;
int speed = 4;
char positions[100] = "Positions";

int main() {
	initialize();

	FntLoad(960, 256); // load the font from the BIOS into the framebuffer
	SetDumpFnt(FntOpen(5, 20, 320, 240, 0, 512)); // screen X,Y | max text length X,Y | autmatic background clear 0,1 | max characters


	while(1) {
 		sprintf(positions, "x=%d, y=%d, w=%d, h%d", x, y, img_raichu_width,img_raichu_height);
        FntPrint(positions);

		update();
		clearDisplay();
		draw();
		display();
	}
}

void initialize() {
	initializeScreen();
	initializePad();
	setBackgroundColor(createColor(0, 0, 0));
	
	crash = createImage(img_raichu);
}

void update() {
	padUpdate();
	if(padCheck(Pad1Up)) {
		y -= speed;
	}

	if(padCheck(Pad1Down)) {
		y += speed;
	}

	if(padCheck(Pad1Left)) {
		x -= speed;
	}

	if(padCheck(Pad1Right)) {
		x += speed;
	}

	if((x + img_raichu_width) >= SCREEN_WIDTH) {
		x = (SCREEN_WIDTH - img_raichu_width);
	} 
	if(x <= 0) {
		x = 0;
	}
	if((y + img_raichu_height) >= SCREEN_HEIGHT) {
		y = (SCREEN_HEIGHT - img_raichu_height);
	}
	if(y <= 0) {
		y = 0;
	}
	

	crash = moveImage(crash, x, y);
}

void draw() {
	FntFlush(-1);
	drawImage(crash);
	
}
