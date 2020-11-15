#include "constants.h"

#define BIT_4 0
#define BIT_8 1
#define BIT_16 2

void initialize();
void update();
void draw();
void initPlayer(u_short x, u_short y, u_short w, u_short h, u_short r, u_short g, u_short b);

Color* color;
u_long* data[3];
Sprite* sprites[4];

POLY_FT4 polyFt4;

int main() {
    initialize();
    while(1) {
        update();
        clear_display();
        draw();
        display();
    }
}

void initialize() {
    initialize_heap();
    initialize_screen();
    initialize_pad();
    color_create(50, 25, 70, &color);
    set_background_color(color);

    cd_open();
    cd_read_file("HPUP.VAG", &data[0]);
    cd_read_file("PSYDUCK.TIM", &data[1]);
    cd_read_file("TREES.TIM", &data[2]);
    cd_read_file("PSY_8.TIM", &data[3]);
    cd_read_file("PSY_16.TIM", &data[4]);
    cd_close();

    audio_init();
    audio_transfer_vag_to_spu((u_char *)data[0], SECTOR * 21, SPU_00CH);
    audio_play(SPU_00CH);

    sprite_create((u_char *)data[1], 32, 32, &sprites[0], BIT_4);
    sprite_create((u_char *)data[2], 100, 100, &sprites[1], BIT_4);
    sprite_create((u_char *)data[3], 250, 100, &sprites[2], BIT_8);
    sprite_create((u_char *)data[4], 60, 120, &sprites[3], BIT_16);

    free3(data[0]);

    // Init poly
    initPlayer(150, 50, 16, 16, 0, 0, 0);
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
    printf("Player initialized at x=%d, y=%d", x, y);
}

void update() {

    pad_update();
    if(pad_check(Pad1Right)) {
        sprites[0]->rotate += ROT_ONE * 1;
    }

}

void draw() {
    draw_sprite(sprites[0]);
    draw_sprite(sprites[1]);
    draw_sprite(sprites[2]);
    draw_sprite(sprites[3]);
    DrawPrim(&polyFt4);
}