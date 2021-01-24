#include "header/gameobject.h"

GameObject *gameobject_init(GsSPRITE *sprite, short w, short h, char xSpeed, char ySpeed, u_char health) {
    GameObject* gobj = MALLOC(GameObject);
    gobj->w = w;
    gobj->h = h;
    gobj->sprite->w = w;    // Set width portion of sprite to render
    gobj->sprite->h = h;    // Set height portion of sprite to render
    gobj->xSpeed = xSpeed;
    gobj->ySpeed = ySpeed;
    gobj->sprite = sprite;
    gobj->health = health;
    gameobject_setHeading(gobj, 0, 0, 0, 0);
    return gobj;
}

void gameobject_draw(GameObject* gameObject) {
    GsSortFastSprite(gameObject->sprite, currentOT(), 1);
}

void gameobject_tick(GameObject* gameObject, Player* player) {

}

void gamedobject_setHeading(GameObject* gobj, u_char l, u_char r, u_char u, u_char d) {
    gobj->heading.left = l;
    gobj->heading.right = r;
    gobj->heading.up = u;
    gobj->heading.down = d;
}

char gameobject_isMoving(GameObject *gobj) {
    return gobj->heading.left == 1 || gobj->heading.right == 1 || gobj->heading.up == 1 || gobj->heading.down == 1;
}
