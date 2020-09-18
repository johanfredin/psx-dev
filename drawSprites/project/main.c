#include "constants.h"

#define JUMP 20
#define MAX_GRAV 10

typedef struct {
	u_short x;
	u_short y;
	u_short width;
	u_short height;
	Image img;
} Sprite;

typedef struct {
	u_short x;
	u_short y;
	u_short width;
	u_short height;
	Box box;
} GameObject;

Sprite raichu; 
GameObject grass;


u_char leftCol, rightCol, topCol, bottomCol;

char positions[50] = "Positions Raichu";

int main() {
	initialize();

	FntLoad(960, 256); // load the font from the BIOS into the framebuffer
	SetDumpFnt(FntOpen(5, 10, 320, 240, 0, 512)); // screen X,Y | max text length X,Y | autmatic background clear 0,1 | max characters

	while(1) {
		
 		sprintf(positions, "x=%d, y=%d, w=%d, h%d\n", raichu.x, raichu.y, raichu.width,raichu.height);
		FntPrint(positions);
		
		update();
		draw();
		display();
		clearDisplay();
	}
}

void initialize() {
	initializeScreen();
	initializePad();
	setBackgroundColor(createColor(100, 0, 255));
	
	initializeSprite(&raichu, 150, 100, 16, 16, createImage(img_raichu));
	initializeGameObject(&grass, 0, SCREEN_HEIGHT - 10, SCREEN_WIDTH - 1, 10, createColor(0, 255, 0));

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

u_char isJumping = 0;
u_char isFalling = 1;
short maxJump = 32;
short jumpStrength = JUMP;
short speed = 4;
short gravity = 4;

void update() {
	padUpdate();
	raichu.y += gravity;

	if(padCheck(Pad1Down)) {
		raichu.y += speed;
	} 
	if(padCheck(Pad1Up)) {
		raichu.y -= speed;
	}
	if(padCheck(Pad1Left)) {
		raichu.x -= speed;
	}
	if(padCheck(Pad1Right)) {
		raichu.x += speed;
	}
	if(padCheck(Pad1Cross)) {
		if(!isJumping) {
			jump();
		}
	}
	if(padCheck(Pad1Start))	{
		raichu.y = 0;
	}
	handleCollision();
	raichu.img = moveImage(raichu.img, raichu.x, raichu.y);
	logState();
}

void jump() {
	raichu.y -= jumpStrength;
	if(jumpStrength <= 0) {
		jumpStrength = 0;
	} else {
		jumpStrength -= 2;
	}
}


void draw() {
	FntFlush(-1);
	drawImage(raichu.img);
	drawBox(grass.box);
}

void handleCollision() {
	if((raichu.x + raichu.width) >= SCREEN_WIDTH) {
		raichu.x = (SCREEN_WIDTH - raichu.width);
	} 
	if(raichu.x <= 0) {
		raichu.x = 0;
	}
	if(raichu.y <= 0) {
		raichu.y = 0;
	}

	handleBoxCollision();
}

void handleBoxCollision() {
	topCol = (raichu.y + raichu.height >= grass.y) &&
		(raichu.y < grass.y) &&
		(raichu.x > grass.x) &&
		(raichu.x + raichu.width < grass.width);
	   
	if((raichu.x + raichu.width >= grass.x) &&
		(raichu.x <= grass.x) &&
		(raichu.y + raichu.height > grass.y) &&
		(raichu.y < grass.height)) { 
		rightCol = 1;
		raichu.x = grass.x - raichu.width;
	}
	if((raichu.x <= grass.width) &&
	   (raichu.x + raichu.width > grass.width) &&
	   (raichu.y + raichu.height > grass.y) &&
	   (raichu.y < grass.height)) { 
		leftCol = 1;
		raichu.x = grass.width;
	}
	if(topCol) { 
		jumpStrength = JUMP;
		isJumping = 0; 
		raichu.y = grass.y - raichu.height;
	}
	if((raichu.y <= grass.height) &&
	   (raichu.y + raichu.height > grass.height) &&
	   (raichu.x > grass.x) &&
	   (raichu.x + raichu.width < grass.width)) { 
		bottomCol = 1;
		raichu.y = grass.height;
	}

	// FntPrint("top=%d", topCol);
	rightCol=0;
	leftCol=0;
	bottomCol=0;
}

void logState() {
	FntPrint("topCol:%d\njumping:%d\ngravity:%d\njumpStrength:%d", topCol, isJumping, gravity, jumpStrength);
}
