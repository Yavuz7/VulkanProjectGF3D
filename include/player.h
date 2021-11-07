#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"
#include "card.h"

/**
 * @brief Create a new player entity
 * @param position where to spawn the aguman at
 * @return NULL on error, or an player entity pointer on success
 */
Entity *player_new(Vector3D position);
/*Enumeration for card Movement system*/
enum movement{north,south,west,east,none};
/*
*@Brief Moves selected card up to 2 spaces
*@Param Entity of player for camera movement
*/
void cardMovement(Entity *self, int x, int y, Card *cardPointer);

/*
*@Brief Helper function of cardMovement, keeps track of places cards moved 
*@Param Enum direction card is moving too
*@Param Enum direction opposite of card moving, to check if card is moving somewhere it was
*/
int movementHelperDouble(enum movement direction, enum movement opposite);
/*
*@Brief Summon player idols or summoning catalyst or whatever
*/
void startGame();



#endif
