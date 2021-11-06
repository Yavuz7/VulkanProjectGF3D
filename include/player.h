#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"

/**
 * @brief Create a new player entity
 * @param position where to spawn the aguman at
 * @return NULL on error, or an player entity pointer on success
 */
Entity *player_new(Vector3D position);

enum movement{north,south,west,east,none};

void cardMovement(Entity *self);

int movementHelper(enum movement direction, enum movement opposite);
/*
*@Brief Summon player idols or summoning catalyst or whatever
*/
void startGame();



#endif
