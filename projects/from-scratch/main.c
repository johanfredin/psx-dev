#include <STDLIB.H>
#include <STDIO.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBETC.H>
#include <LIBSPU.H>
#include <LIBCD.H>
#include <LIBDS.H>
#include <STRINGS.H>
#include <SYS/TYPES.H>

// Constants
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 256
#define NUM_BUFFERS 2
#define BUFFERS_LENGHT 1
#define MODE_NTSC 0
#define MODE_PAL 1
#define PACKETMAX 300
#define FRAME_BUFFER_WIDTH 1024
#define FRAME_BUFFER_HEIGHT 512
#define SECTOR 2048
#define SPEED 4

#define __ramsize   0x00200000 // Force 2MB vram
#define __stacksize 0x00004000

// Globals
typedef struct {
    u_short r;
    u_short g;
    u_short b;
} Color;

GsOT orderingTable[NUM_BUFFERS];
GsOT_TAG minorOrderingTable[NUM_BUFFERS][1<<BUFFERS_LENGHT];
PACKET GPUOutputPacket[NUM_BUFFERS][PACKETMAX];
u_char currentBuffer = 0;
Color backgroundColor;

DISPENV dispenv[2];
DRAWENV drawenv[2];
POLY_FT4 polyFt4;

u_char didInitDs = 0;
u_short currentKeyDown = 0;

// Prototypes
void initializeScreen();
void clearVRAM();
void update();
void initializeDebugFont();
void draw();
void display();
void clearDisplay();
void initDisplayAndDrawEnvs();
void initPlayer(u_short x, u_short y, u_short w, u_short h, u_short r, u_short g, u_short b);
void initGameObjects();
void cd_open();
void cd_close();
void cd_read_file(u_char* file_path, u_long** file);

int main() {
    backgroundColor.r = 66;
    backgroundColor.g = 200;
    backgroundColor.b = 100;
    initializeScreen();
    initializeDebugFont();
    initPlayer(100, 50, 28, 48, 50, 10, 200);
    initGameObjects();

    while(1) {
        update();
        draw();
        display();
        clearDisplay();
    }

    return 0;
}

// Definitions -----------------------------------------

void initializeScreen() {
    SetVideoMode(1);
    SetDispMask(1); // 1=MASK on
	ResetGraph(0);  // Initialise drawing engine, 0=Complete reset
    clearVRAM();    

    // Resets libgpu and initializes the graphic system. libpgu settings are maintaned by 
    // the global variables GsDISPENV and GsDRAWENV
    GsInitGraph(SCREEN_WIDTH, SCREEN_HEIGHT, GsINTER|GsOFSGPU, 0, 0); 
    
    // Define double buffers
    GsDefDispBuff(0, 0, 0, SCREEN_HEIGHT);

    // Initialize display and drawing environments
    SetDefDispEnv(&dispenv[0], 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    SetDefDrawEnv(&drawenv[0], 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    SetDefDispEnv(&dispenv[1], 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    SetDefDrawEnv(&drawenv[1], 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    drawenv[0].isbg = 0;
    drawenv[1].isbg = 0;

    // Initialize ordering tables
    GsClearOt(0, 0, &orderingTable[GsGetActiveBuff()]);
    orderingTable[0].length = BUFFERS_LENGHT;
    orderingTable[0].org = minorOrderingTable[0];
    orderingTable[1].length = BUFFERS_LENGHT;
    orderingTable[1].org = minorOrderingTable[1];
    GsClearOt(0, 0, &orderingTable[0]);
    GsClearOt(0, 0, &orderingTable[1]);

    PadInit(MODE_NTSC);
}

void clearVRAM() {
    RECT rect;
    setRECT(&rect, 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
    ClearImage2(&rect, backgroundColor.r, backgroundColor.g, backgroundColor.b);    // Clear Frame Buffer at high speed (interlaced mode)
    DrawSync(0);    // Waits for ClearImage2 function to finish
}

void initializeDebugFont() {
	FntLoad(960, 256);
	SetDumpFnt(FntOpen(5, 20, 320, 240, 0, 512)); //Sets the dumped font for use with FntPrint();
}

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

void display() {
    currentBuffer = GsGetActiveBuff();
    DrawSync(0);        // Wait for drawing to end
    VSync(0);           // Wait for vsync (2 = 30FPS moreless)
    GsSwapDispBuff();   // Swap buffers
    
    // Sets a screen clear command at the start of the OT. Should be called after GsSwapDispBuff.
    // Actual clearing isnt executed unti GsDrawOt() is called
    GsSortClear(backgroundColor.r, backgroundColor.g, backgroundColor.b, &orderingTable[currentBuffer]);
    // Starts execution of GPU commands registered in OT
    GsDrawOt(&orderingTable[currentBuffer]);
}

void clearDisplay() {
    // currentBuffer = GsGetActiveBuff();
    FntFlush(-1);
    // Set the memory adress for storing drawing commands for drawing primitives generated by functions
    // like GsSortObject(), GsSortSprite() and GsSortBg()
    GsSetWorkBase((PACKET*)GPUOutputPacket[currentBuffer]);
    GsClearOt(0, 0, &orderingTable[currentBuffer]);
}

void cd_open() {
	if(!didInitDs) {
		didInitDs = 1;
		DsInit();
	}
}

void cd_close() {
	if(didInitDs) {
		didInitDs = 0;
		DsClose();
    }
}

void cd_read_file(unsigned char* file_path, u_long** file) {

	u_char* file_path_raw;
	int* sectors_size;
	DslFILE* temp_file_info;
	sectors_size = malloc3(sizeof(int));
	temp_file_info = malloc3(sizeof(DslFILE));

	// Exit if libDs isn't initialized
	if(!didInitDs) {
		printf("LIBDS not initialized, run cdOpen() first\n");	
		return;
	}

	// Get raw file path
	file_path_raw = malloc3(4 + strlen(file_path));
	strcpy(file_path_raw, "\\");
	strcat(file_path_raw, file_path);
	strcat(file_path_raw, ";1");
	printf("Loading file from CD: %s\n", file_path_raw);

	// Search for file on disc
	DsSearchFile(temp_file_info, file_path_raw);

	// Read the file if it was found
	if(temp_file_info->size > 0) {
		printf("...file found\n");
		printf("...file size: %lu\n", temp_file_info->size);
		*sectors_size = temp_file_info->size + (SECTOR % temp_file_info->size);
		printf("...file buffer size needed: %d\n", *sectors_size);
		printf("...sectors needed: %d\n", (*sectors_size + SECTOR - 1) / SECTOR);
		*file = malloc3(*sectors_size + SECTOR);
		
		DsRead(&temp_file_info->pos, (*sectors_size + SECTOR - 1) / SECTOR, *file, DslModeSpeed);
		while(DsReadSync(NULL));
		printf("...file loaded!\n");
	} else {
		printf("...file not found");
	}

	// Clean up
	free3(file_path_raw);
	free3(sectors_size);
	free3(temp_file_info);
}