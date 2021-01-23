#include "header/gameobject.h"

GameObject* gameobject_init(GsSPRITE* sprite, u_char speed, u_char health) {
    GameObject* gobj = MALLOC(GameObject);
    gobj->speed = speed;
    gobj->sprite = sprite;
    gobj->health = health;
    return gobj;
}

void gameobject_draw(GameObject* gameObject) {
    GsSortFastSprite(gameObject->sprite, currentOT(), 1);
}

void gameobject_tick(GameObject* gameObject, Player* player) {

}