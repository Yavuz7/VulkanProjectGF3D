#ifndef __PLAYER_H__
#define __PLAYER_H__


#include "entity.h"
#include "card.h"
#include "field_play.h"

/**
 * @brief Create a new player entity
 * @int to set if player is active or not
 * @return NULL on error, or an player entity pointer on success
 */
Entity *player_new();

/**
* @brief Creates 2 player entities, only one will be active at a time
*/

void setPlayers();

/**
* @brief Changes inuse player entity 
*/
void changeTurn();

/*Enumeration for card Movement system*/
enum movement{north,south,west,east,none,blocked};
/*
*@Brief Moves selected card up to 2 spaces
*@Param Entity of player for camera movement
*@Param x position of movement
*@Param y position of movement
*@Param Pointer to card being moved
*/
void cardMovement(Entity *self, int x, int y, Card *cardPointer);

/*
*@Brief Helper function of cardMovement, keeps track of places cards moved 
*@Param Enum direction card is moving too
*@Param Enum direction opposite of card moving, to check if card is moving somewhere it was
*@Return int to check whether or not to move
*/
int movementHelperDouble(enum movement direction, enum movement opposite);

/*
*@Brief Open Hand Menu
*/
void openHand();

/*
*@Brief Set x and y values to match the last move made
*/
void movementHelperFight(Card *cardPointer);

/*
*@Brief resets movement data for reuse
*/
void resetMovement();

void cameraMovement(Uint8 * pPointer, int pChange, int currentPlayer, float * pPosition, float offset);
#endif
