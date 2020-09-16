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

POLY_FT3 poly;

int main() {
	initialize();

	FntLoad(960, 256); // load the font from the BIOS into the framebuffer
	SetDumpFnt(FntOpen(5, 20, 320, 240, 0, 512)); // screen X,Y | max text length X,Y | autmatic background clear 0,1 | max characters


	while(1) {
		/*
 		sprintf(positions, "x=%d, y=%d, w=%d, h%d\n", raichu.x, raichu.y, raichu.width,raichu.height);
		FntPrint(positions);
		sprintf(positions_g, "x=%d, y=%d, w=%d, h%d\n", grass.x, grass.y, grass.width,grass.height);
		FntPrint(positions_g);
		
		update();
		clearDisplay();
		draw();
		*/
		DrawPrim(&poly);
		display();
	}
}

void initialize() {
	/*
	typedef struct {
	u_long	tag;
	u_char	r0, g0, b0, code;
	short	x0, 	y0;
	u_char	u0, v0;	u_short	clut;
	short	x1,	y1;
	u_char	u1, v1;	u_short	tpage;
	short	x2,	y2;
	u_char	u2, v2;	u_short	pad1;
} POLY_FT3
	*/
	
	poly.tag = 0;
	poly.r0 = 255;
	poly.g0 = 100;
	poly.b0 = 155;
	poly.tpage = 1;
	poly.x0 = 10;
	poly.x1 = 50;
	poly.y0 = 10;
	poly.y1 = 25;
	poly.x2 = 100;
	poly.y2 = 50;
	SetPolyF4(&poly);

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
	rightCol = (raichu.x + raichu.width >= grass.x) && 	(raichu.x + raichu.width < grass.width);
	leftCol = (raichu.x <= grass.width) && raichu.x > grass.x;
	topCol = (raichu.y + raichu.height) >= grass.y && raichu.y < grass.height;
	bottomCol = (raichu.y <= grass.height) && raichu.y + raichu.height > grass.y;

	if(rightCol && !leftCol && (topCol || bottomCol)) {
		raichu.x = grass.x - raichu.width;
	}
	if(leftCol && !rightCol && (topCol || bottomCol)) {	
		raichu.x = grass.width;
	}
	if(topCol && !bottomCol) {
		raichu.y = grass.y - grass.height;
	}
	if(bottomCol && !topCol) {
		raichu.y = grass.height;
	}


	FntPrint("rightCol=%d\nleftCol=%d\ntopCol=%d\nbottomCol=%d\n", rightCol, leftCol, topCol, bottomCol);
}

