#include "header/player.h"

Player* player_init(AnimatedGameObject* gobj, u_char pNum, u_char xSpeed, u_char ySpeed) {
    Player* p = MEM_ALLOC(Player);
    p->gobj = gobj;
    p->pNum = pNum;
    p->xSpeed = xSpeed;
    p->ySpeed = ySpeed;
    printf("Player initialized\n");
    return p;
}

void player_draw(Player* p) {
    if(PRINT_COORDS) {
        FntPrint("x=%d, y=%d\n", p->gobj->textureFrame->x, p->gobj->textureFrame->y);
        FntPrint("left=%d, right=%d, up=%d, down=%d\n", p->gobj->heading.left, p->gobj->heading.right, p->gobj->heading.up, p->gobj->heading.down);
    }
    GsSortFastSprite(p->gobj->textureFrame, currentOT(), 1);
}

void player_tick(Player* p) {
    short xSpeed = 0;
    short ySpeed = 0;
    animatedobject_setHeading(p->gobj, 0, 0, 0, 0);
    p->currBtnPressed = PadRead(p->pNum);
    if(p->currBtnPressed & PADLdown) {
        ySpeed = p->ySpeed;
        animatedobject_setHeading(p->gobj, 0, 0, 0, 1);
    } if(p->currBtnPressed & PADLup) {
        ySpeed = -p->ySpeed;
        animatedobject_setHeading(p->gobj, 0, 0, 1, 0);
    } if(p->currBtnPressed & PADLright) {
        xSpeed = p->xSpeed;
        animatedobject_setHeading(p->gobj, 0, 1, 0, 0);
    } if(p->currBtnPressed & PADLleft) {
        xSpeed = -p->xSpeed;
        animatedobject_setHeading(p->gobj, 1, 0, 0, 0);
    } 
    animatedobject_tick(p->gobj);
    p->gobj->textureFrame->x += xSpeed;
    p->gobj->textureFrame->y += ySpeed;
}