#include "header/gameobject.h"

/**
 * Update the Heading struct of the passed in game object
 * @param gobj the game object owning the heading struct
 * @param l are we heading left?
 * @param r are we heading right?
 * @param u are we heading up?
 * @param d are we heading down?
 */
void setHeading(GameObject *gobj, u_char l, u_char r, u_char u, u_char d);

char isMoving(GameObject *gobj);

GameObject *gameobject_init(GsSPRITE *sprite, short w, short h, char xSpeed, char ySpeed, u_char health, u_char type) {
    GameObject *gobj = MALLOC(GameObject);
    gobj->w = w;
    gobj->h = h;
    gobj->sprite = sprite;
    gobj->sprite->w = w; // Set width portion of sprite to render
    gobj->sprite->h = h; // Set height portion of sprite to render
    gobj->xSpeed = xSpeed;
    gobj->ySpeed = ySpeed;
    gobj->health = health;
    gobj->spawnX = sprite->x;
    gobj->spawnY = sprite->y;
    gobj->type = type;
    setHeading(gobj, 0, 0, 0, 0);
    return gobj;
}

void gameobject_draw(GameObject *gameObject) {
    if (PRINT_COORDS) {
        FntPrint("gobjs x=%d, gobj y=%d\n", gameObject->sprite->x, gameObject->sprite->y);
    }
    GsSortFastSprite(gameObject->sprite, currentOT(), 1);
}

void gameobject_tick(GameObject *gameObject, Player *player) {
    gameObject->sprite->x += gameObject->xSpeed;
    gameObject->sprite->y += gameObject->ySpeed;
    return;
}

void setHeading(GameObject *gobj, u_char l, u_char r, u_char u, u_char d) {
    gobj->heading.left = l;
    gobj->heading.right = r;
    gobj->heading.up = u;
    gobj->heading.down = d;
}

char isMoving(GameObject *gobj) {
    return gobj->heading.left == 1 || gobj->heading.right == 1 || gobj->heading.up == 1 || gobj->heading.down == 1;
}

Player *player_init(Animation *anim, GameObject *gobj, u_char pNum) {
    Player *p = MALLOC(Player);
    p->anim = anim;
    p->gobj = gobj;
    p->pNum = pNum;
    p->currBtnPressed = 0;
    return p;
}

void player_draw(Player *p) {
    if (PRINT_COORDS) {
        FntPrint("x=%d, y=%d\n", p->gobj->sprite->x, p->gobj->sprite->y);
        FntPrint("left=%d, right=%d, up=%d, down=%d\n", p->gobj->heading.left, p->gobj->heading.right, p->gobj->heading.up, p->gobj->heading.down);
    }
    GsSortFastSprite(p->gobj->sprite, currentOT(), 1);
}

void player_tick(Player *p) {
    short xSpeed = 0, pxSpeed = p->gobj->xSpeed;
    short ySpeed = 0, pySpeed = p->gobj->ySpeed;
    p->currBtnPressed = PadRead(p->pNum);
     setHeading(p->gobj, 0, 0, 0, 0);
    if (p->currBtnPressed & PADLdown) {
        ySpeed = pySpeed;
        setHeading(p->gobj, 0, 0, 0, 1);
    }
    if (p->currBtnPressed & PADLup) {
        ySpeed = -pySpeed;
        setHeading(p->gobj, 0, 0, 1, 0);
    }
    if (p->currBtnPressed & PADLright) {
        xSpeed = pxSpeed;
        setHeading(p->gobj, 0, 1, 0, 0);
    }
    if (p->currBtnPressed & PADLleft) {
        xSpeed = -pxSpeed;
        setHeading(p->gobj, 1, 0, 0, 0);
    } 
    
    animation_tick(p->anim, p->gobj);
    p->gobj->sprite->x += xSpeed;
    p->gobj->sprite->y += ySpeed;
}

Animation *animation_init(u_short curr_u, u_short curr_v, u_char keyFrames, u_short ticksPerFrame) {
    Animation *anim = MALLOC(Animation);
    anim->curr_u = curr_u;
    anim->curr_v = curr_v;
    anim->keyFrames = keyFrames;
    anim->ticksPerFrame = ticksPerFrame;
    anim->accTicks = 0;
    return anim;
}

void animation_tick(Animation *anim, GameObject *gobj) {
    short u = anim->curr_u;     // Fetch current x offset
    short v = anim->curr_v;     // Fetch current y offset
    short spriteW = gobj->w;
    short spriteH = gobj->h;
    if (gobj->heading.left) {
        v = spriteH;                    // Move y offset down by the height of the objgect to next spritesheet row 
    } else if (gobj->heading.right) {
        v = spriteH * 2;
    } else if (gobj->heading.up) {
        v = spriteH * 3;
    } else if (gobj->heading.down) {
        v = 0;
    }
    gobj->sprite->v = v;
    anim->curr_v = v;
    

    anim->accTicks += 1;

    if (PRINT_ANIMATION) {
        FntPrint("Acc=%d, curr_u=%d, moving=%d\n", anim->accTicks, anim->curr_u, isMoving(gobj));
    }

    if (anim->accTicks >= anim->ticksPerFrame) {
        anim->accTicks = 0;
        if (u >= ((anim->keyFrames * spriteW) - spriteW)) { // If x offset is at last horizontal frame, reset it to 0
            u = 0;
        } else {
            if (isMoving(gobj)) {
                u += spriteW;   // Move to next frame when not at the end of horizontal sheet.
            }
        }
    }
    anim->curr_u = u;
    gobj->sprite->u = u;
}
