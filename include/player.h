#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"

/**
 * @brief Create a new player entity
 * @param position where to spawn the aguman at
 * @return NULL on error, or an player entity pointer on success
 */
Entity *player_new(Vector3D position);

/*
*@Brief Summon player idols or summoning catalyst or whatever
*/
void startGame();

/*
*@Brief Summon card in valid area
*/
void playCard();

#endif
