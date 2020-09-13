#include "constants.h"

#define DIR_NONE 	0
#define DIR_LEFT 	1
#define DIR_RIGHT 	2
#define DIR_UP 		3
#define DIR_DOWN 	4

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

Sprite raichu; 
GameObject grass;

short currDir = DIR_DOWN;
short lastKnowDir = DIR_DOWN;

short leftCol, rightCol, topCol, bottomCol;

short speed = 4;
short gravity = 3;
short jumpVel = 0;
short collision = 0;
short isOnGround = 0;

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
	initializeGameObject(&grass, 50, 150, 70, 50, createColor(0, 255, 0));

}

void initializeSprite(Sprite* sprite, int x, int y, int width, int height, Image img) {
	sprite->x = x;
	sprite->y = y;
	sprite->width = width;
	sprite->height = height;
	sprite->img = img;
}

void initializeGameObject(GameObject* gobj, int x, int y, int width, int height, Color color) {
	int coordWitdth = x + width;
	int coordHeight = y + height;
	
	gobj->x = x;
	gobj->y = y;
	gobj->width = coordWitdth;
	gobj->height = coordHeight;
	gobj->box = createBox(color, x, y, coordWitdth, coordHeight);
};

void update() {
	if(!isOnGround)
		raichu.y += gravity;
		
	checkControllerState();
	handleLevelBoundsCollision();
	handleBoxCollision();

	raichu.img = moveImage(raichu.img, raichu.x, raichu.y);

	FntPrint("Current dir: %d\n", lastKnowDir);
}

void checkControllerState() {
	padUpdate();
	
	if(padCheck(Pad1Down)) {
		raichu.y += speed;
		currDir = DIR_DOWN;
	} 
	if(padCheck(Pad1Up)) {
		raichu.y -= speed;
		currDir = DIR_UP;
	}
	if(padCheck(Pad1Left)) {
		raichu.x -= speed;
		currDir = DIR_LEFT;
	}
	if(padCheck(Pad1Right)) {
		raichu.x += speed;
		currDir = DIR_RIGHT;
	}
	lastKnowDir = currDir;

	if(padCheck(Pad1Cross)) {
		jump();
	}

	// Dirty reset check
	if(padCheck(Pad1Start)) {
		raichu.y = 0;
	}
}


void jump() {
	if(isOnGround) {
		jumpVel = 20;
		raichu.y -= jumpVel;
		jumpVel -= gravity;
	} 

}

void handleLevelBoundsCollision() {
	if((raichu.x + raichu.width) >= SCREEN_WIDTH) {
		raichu.x = (SCREEN_WIDTH - raichu.width);
	} 
	if(raichu.x <= 0) {
		raichu.x = 0;
	}
	if(raichu.y <= 0) {
		raichu.y = 0;
	}

	if(raichu.y + raichu.height >= SCREEN_HEIGHT) {
		raichu.y = SCREEN_HEIGHT - raichu.height;
		isOnGround = 1;
	} else {
		jumpVel = 0;
		isOnGround = 0;
	}
}

void handleBoxCollision() {
	collision = 0;


}



void draw() {
	FntFlush(-1);
	drawImage(raichu.img);
	drawBox(grass.box);
}