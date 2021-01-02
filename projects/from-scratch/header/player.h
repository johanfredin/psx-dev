#ifndef _player_h_
#define _player_h_

#include "animatedobject.h"
#include "base.h"
#include <LIBETC.H>

typedef struct {
    AnimatedGameObject* gobj;
    u_char xSpeed, ySpeed, pNum;
    u_long currBtnPressed;    
} Player;

Player* player_init(AnimatedGameObject* gobj, u_char pNum, u_char xSpeed, u_char ySpeed);
void player_draw(Player* p);
void player_tick(Player* p);


#endif