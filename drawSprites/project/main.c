#include "constants.h"

typedef struct {
	int x;
	int y;
	int width;
	int height;
	Image img;
} Sprite;

typedef struct {
	int x;
	int y;
	int width;
	int height;
	Box box;
} GameObject;

Sprite raichu; //{ 100, 100, 16, 16, NULL };
GameObject grass; //;
short collide = 0;

int speed = 4;
int gravity = 4;

char positions[50] = "Positions Raichu";
char positions_g[50] = "Positions grass";

int main() {
	initialize();

	FntLoad(960, 256); // load the font from the BIOS into the framebuffer
	SetDumpFnt(FntOpen(5, 20, 320, 240, 0, 512)); // screen X,Y | max text length X,Y | autmatic background clear 0,1 | max characters


	while(1) {
 		sprintf(positions, "x=%d, y=%d, w=%d, h%d\n", raichu.x, raichu.y, raichu.width,raichu.height);
		FntPrint(positions);
		sprintf(positions_g, "x=%d, y=%d, w=%d, h%d\n", grass.x, grass.y, grass.width,grass.height);
		FntPrint(positions_g);
		if(collide) {
			FntPrint("Collide");
		}

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
	
	initializeSprite(&raichu, 100, 100, 16, 16, createImage(img_raichu));
	initializeGameObject(&grass, 0, SCREEN_HEIGHT - 50, SCREEN_WIDTH - 70, SCREEN_HEIGHT, createColor(0, 255, 0));

}

void initializeSprite(Sprite* sprite, int x, int y, int width, int height, Image img) {
	sprite->x = x;
	sprite->y = y;
	sprite->width = width;
	sprite->height = height;
	sprite->img = img;
}

void initializeGameObject(GameObject* gobj, int x, int y, int width, int height, Color color) {
	gobj->x = x;
	gobj->y = y;
	gobj->width = width;
	gobj->height = height;
	gobj->box = createBox(color, x, y, width, height);
};

short isJumping = 0;
short maxJump = 16;
short isFalling = 1;

void update() {
	padUpdate();
	raichu.y += gravity;

	if(padCheck(Pad1Down)) {
		raichu.y += speed;
	}

	if(padCheck(Pad1Left)) {
		raichu.x -= speed;
	}

	if(padCheck(Pad1Right)) {
		raichu.x += speed;
	}
	if(padCheck(Pad1Cross)) {
		if(isFalling) {

		} else {
			raichu.y += 50;
		}
	}	
	handleCollision();
	raichu.img = moveImage(raichu.img, raichu.x, raichu.y);
}

void draw() {
	FntFlush(-1);
	drawImage(raichu.img);
	drawBox(grass.box);
}

void handleCollision() {
	if((raichu.x + img_raichu_width) >= SCREEN_WIDTH) {
		raichu.x = (SCREEN_WIDTH - img_raichu_width);
	} 
	if(raichu.x <= 0) {
		raichu.x = 0;
	}
	
	if(raichu.y <= 0) {
		raichu.y = 0;
	}


	if((raichu.y + raichu.height) >= grass.y &&
	raichu.x > (grass.x + grass.width)) {
		collide = 1;
		isFalling = 0;
		raichu.y = (grass.y - raichu.height);
	} else {
		collide = 0;
	}

}

