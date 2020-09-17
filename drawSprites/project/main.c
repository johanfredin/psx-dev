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

short leftCol, rightCol, topCol, bottomCol;

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
		
		update();
		draw();
		display();
		clearDisplay();
	}
}

void initialize() {
	initializeScreen();
	initializePad();
	setBackgroundColor(createColor(0, 0, 0));
	
	initializeSprite(&raichu, 100, 100, 16, 16, createImage(img_raichu));
	initializeGameObject(&grass, 110, 120, 70, 50, createColor(0, 255, 0));

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
	logBox("box", gobj);
};

void logBox(char* name, GameObject* gobj) {
	printf("objc coords: { x:%d, y:%d, w:%d, h%d: }\n", gobj->x, gobj->y, gobj->width, gobj->height);
}


short isJumping = 0;
short maxJump = 16;
short isFalling = 1;

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
	if((raichu.y + raichu.height >= grass.y) &&
		(raichu.y < grass.y) &&
		(raichu.x > grass.x) &&
		(raichu.x + raichu.width < grass.width)) { 
		topCol = 1;
		raichu.y = grass.y - raichu.height;
	}
	if((raichu.y <= grass.height) &&
	   (raichu.y + raichu.height > grass.height) &&
	   (raichu.x > grass.x) &&
	   (raichu.x + raichu.width < grass.width)) { 
		raichu.y = grass.height;
	}

	rightCol=0;
	leftCol=0;
	topCol=0;
	bottomCol=0;
	FntPrint("rightCol=%d\nleftCol=%d\ntopCol=%d\nbottomCol=%d\n", rightCol, leftCol, topCol, bottomCol);
}

