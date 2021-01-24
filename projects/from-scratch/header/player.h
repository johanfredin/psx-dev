#ifndef _player_h_
#define _player_h_

#include "animation.h"
#include "gameobject.h"
#include "base.h"
#include "mem.h"
#include <LIBETC.H>

typedef struct {
    Animation* ao;
    GameObject* go;
    u_char pNum;
    u_long currBtnPressed;    
} Player;

Player* player_init(Animation* ao, GameObject* go, u_char pNum);
void player_draw(Player* p);
void player_tick(Player* p);


#endif