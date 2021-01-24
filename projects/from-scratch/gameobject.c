#include "header/gameobject.h"

GameObject* gameobject_init(GsSPRITE* sprite, u_char xSpeed, u_char ySpeed, u_char health) {
    GameObject* gobj = MALLOC(GameObject);
    gobj->xSpeed = xSpeed;
    gobj->ySpeed = ySpeed;
    gobj->sprite = sprite;
    gobj->health = health;
    return gobj;
}

void gameobject_draw(GameObject* gameObject) {
    GsSortFastSprite(gameObject->sprite, currentOT(), 1);
}

void gameobject_tick(GameObject* gameObject, Player* player) {


}