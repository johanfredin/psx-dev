#include "header/player.h"

Player* player_init(Animation *anim, GameObject *gobj, u_char pNum) {
    Player *p = MALLOC(Player);
    p->anim = anim;
    p->gobj = gobj;
    p->pNum = pNum;
    p->currBtnPressed = 0;
    return p;
}

void player_draw(Player *p) {
    if (PRINT_COORDS) {
        FntPrint("x=%d, y=%d\n", p->gobj->sprite->x, p->gobj->sprite->y);
        FntPrint("left=%d, right=%d, up=%d, down=%d\n", p->gobj->heading.left, p->gobj->heading.right, p->gobj->heading.up, p->gobj->heading.down);
    }
    GsSortFastSprite(p->gobj->sprite, currentOT(), 1);
}

void player_tick(Player *p) {
    short xSpeed = 0, pxSpeed = p->gobj->xSpeed;
    short ySpeed = 0, pySpeed = p->gobj->ySpeed;
    animatedobject_setHeading(p->gobj, 0, 0, 0, 0);
    p->currBtnPressed = PadRead(p->pNum);
    if (p->currBtnPressed & PADLdown) {
        ySpeed = pySpeed;
        animatedobject_setHeading(p->gobj, 0, 0, 0, 1);
    }
    if (p->currBtnPressed & PADLup) {
        ySpeed = -pySpeed;
        animatedobject_setHeading(p->gobj, 0, 0, 1, 0);
    }
    if (p->currBtnPressed & PADLright) {
        xSpeed = pxSpeed;
        animatedobject_setHeading(p->gobj, 0, 1, 0, 0);
    }
    if (p->currBtnPressed & PADLleft) {
        xSpeed = -pxSpeed;
        animatedobject_setHeading(p->gobj, 1, 0, 0, 0);
    }
    animation_tick(p->anim, p->gobj);
    p->gobj->sprite->x += xSpeed;
    p->gobj->sprite->y += ySpeed;
}