#ifndef _gameobject_h_
#define _gameobject_h_

#include "player.h"

typedef struct {
    GsSPRITE* sprite;
    char speed;
    u_char health;
} GameObject;

/**
 * Allocates heap memory for new game object
 * @param sprite the sprite
 * @param speed movement speed of the game object
 * @param health health of the game object
 * @return a pointer to a heap allocated game object with given parameters
 */
GameObject* gameobject_init(GsSPRITE* sprite, u_char speed, u_char health);
/**
 * Draw the game object
 * @param gameObject the game object sprite to draw 
 */
void gameobject_draw(GameObject* gameObject);
/**
 * Update the game object
 * @param gameObject the game object to update
 * @param player how to interact with a player
 */
void gameobject_tick(GameObject* gameObject, Player* player);

#endif