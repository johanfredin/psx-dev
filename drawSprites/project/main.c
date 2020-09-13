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

short collide = 0;
short currDir = DIR_NONE;
short lastKnowDir = DIR_NONE;

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
	initializeGameObject(&grass, 30, 150, 70, 50, createColor(0, 255, 0));

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
	//raichu.y += gravity;

	
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
		if(isFalling) {

		} else {
			raichu.y += 50;
		}
	}
	FntPrint("Current dir: %d", lastKnowDir);
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

short collision = 0;
void handleBoxCollision() {
	if (raichu.x < grass.width &&
		raichu.x + raichu.width > grass.x &&
		raichu.y < grass.height &&
		raichu.y + raichu.height > grass.y) {
			collision = 1;
			switch (lastKnowDir) {
			case DIR_RIGHT:
				raichu.x = grass.x - raichu.width;
				break;
			case DIR_LEFT:
				raichu.x = grass.width;
				break;
			case DIR_DOWN:
				raichu.y = grass.y - raichu.height;
				break;
			case DIR_UP:
				raichu.y = grass.height;
				break;
			default:
				break;
			}
		} else {
			collision = 0;
		}
    // collision detected!
	FntPrint("Collision: %d", collision);


	//FntPrint("rightCol=%d\nleftCol=%d\ntopCol=%d\nbottomCol=%d\n", rightCol, leftCol, topCol, bottomCol);
}

