#ifndef _gameobject_h_
#define _gameobject_h_

#include "gpubase.h"

typedef struct Heading {
    u_int left : 1;
    u_int right : 1;
    u_int up : 1;
    u_int down : 1;
} Heading;

typedef struct {
    GsSPRITE *sprite;
    u_short w, h;
    char xSpeed, ySpeed;
    u_char health;
    Heading heading;
} GameObject;

/**
 * Allocates heap memory for new game object
 * @param sprite the sprite
 * @param w width of sprite. If passing in a number smaller than full 
 * width then only a portion of sprite will be displayed. Use this when 
 * object takes in a sprite sheet.
 * @param h width of sprite. If passing in a number smaller than full 
 * height then only a portion of sprite will be displayed. Use this when 
 * object takes in a sprite sheet.
 * @param xSpeed movement speed of the game object on the x axis
 * @param ySpeed movement speed of the game object on the y axis
 * @param health health of the game object
 * @return a pointer to a heap allocated game object with given parameters
 */
GameObject *gameobject_init(GsSPRITE* sprite, short w, short h, char xSpeed, char ySpeed, u_char health);
/**
 * Draw the game object
 * @param gameObject the game object sprite to draw 
 */
void gameobject_draw(GameObject *gameObject);

/**
 * Update the game object
 * @param gameObject the game object to update
 */
void gameobject_tick(GameObject* gameObject);

/**
 * Update the Heading struct of the passed in game object
 * @param gobj the game object owning the heading struct
 * @param l are we heading left?
 * @param r are we heading right?
 * @param u are we heading up?
 * @param d are we heading down?
 */
void gameobject_setHeading(GameObject *gobj, u_char l, u_char r, u_char u, u_char d);


char gameobject_isMoving(GameObject *gobj);

#endif