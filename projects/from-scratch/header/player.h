#ifndef _player_h_
#define _player_h_

#include "gameobject.h"
#include "animation.h"
#include <LIBETC.H>

typedef struct {
    Animation* anim;
    GameObject* gobj;
    u_char pNum;
    u_long currBtnPressed;    
} Player;

Player* player_init(Animation* anim, GameObject* gobj, u_char pNum);
void player_draw(Player* p);
void player_tick(Player* p);


#endif