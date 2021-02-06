#ifndef _gameobject_h_
#define _gameobject_h_

#include "base.h"
#include "gpubase.h"

#define TYPE_PLAYER 0
#define TYPE_NPC 1

#define resetPosition(gobj)         \
    gobj->sprite->x = gobj->spawnX; \
    gobj->sprite->y = gobj->spawnY


#define switchDirection(gobj) \
    gobj->xSpeed *= -1;       \
    gobj->ySpeed *= -1

#define switchXDir(gobj) gobj->xSpeed *= -1
#define switchYDir(gobj) gobj->ySpeed *= -1

typedef struct Heading {
    u_int left : 1;
    u_int right : 1;
    u_int up : 1;
    u_int down : 1;
} Heading;

typedef struct {
    GsSPRITE *sprite;
    u_short w, h;
    u_char spawnX, spawnY, type;
    short xSpeed, ySpeed;
    u_char health;
    Heading heading;
} GameObject;

typedef struct {
    u_char keyFrames;
    u_short ticksPerFrame;
    u_short curr_u;
    u_short curr_v;
    u_short accTicks;
} Animation;

typedef struct {
    Animation *anim;
    GameObject *gobj;
    u_char pNum;
    u_long currBtnPressed;
} Player;

/**
 * Create a new animation object on the heap with given properties
 * @param curr_u the starting texture frame x offset of the sprite we will work with
 * @param curr_v the starting texture frame y offset of the sprite we will work with
 * @param keyFrames the amount of frames on the x axis of the spritesheet passed in. Eg. if sprite w=96 and keyFrames=3 then animation object will assume gameobject sprite has a width of 32 pixels and update annimation in that fassion.
 * @param ticksPerFrame how many rendering iterations until its time to update frame. 
 * @return pointer to a new animation object on the heap with given properties
 */
Animation *animation_init(u_short curr_u, u_short curr_v, u_char keyFrames, u_short ticksPerFrame);
/**
 * Update passed in object with current animation state
 * @param anim animation object that will update sprite of passed in game object
 * @param gobj game object we want to animate.
 */
void animation_tick(Animation *anim, GameObject *gobj);

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
 * @param type what type of game object this is (e.g player, NPC, key...
 * @return a pointer to a heap allocated game object with given parameters
 */
GameObject *gameobject_init(GsSPRITE *sprite, short w, short h, char xSpeed, char ySpeed, u_char health, u_char type);
/**
 * Draw the game object
 * @param gameObject the game object sprite to draw 
 */
void gameobject_draw(GameObject *gameObject);

/**
 * Update the game object
 * @param gameObject the game object to update
 * @param player how to interact with player
 */
void gameobject_tick(GameObject *gameObject, Player *player);

Player *player_init(Animation *anim, GameObject *gobj, u_char pNum);
void player_draw(Player *p);
void player_tick(Player *p);

#endif
