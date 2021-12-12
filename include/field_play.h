#ifndef FIELD_PLAY_H__
#define FIELD_PLAY_H__

#include "simple_logger.h"
#include "gfc_types.h"
#include "gfc_text.h"

#include "card.h"
#include "tile.h"

/*
*@Brief Move cards position and entity and sets new coordinates
*@Param X coordinate of new position
*@Param Y coordinate of new position
*@Param Pointer to card being moved, should be data from list
*/
void cardMove(int x, int y, Card *cardP);

/*
*@Brief Sets Card to defense position
*@Param Card being set to defense postion
*/
void setCardDefense(Card *cardpointer);
/*
*@Brief Sets Card to Fight position
*@Param Card being set to Fight position
*/
void setCardFight(Card *cardpointer, int player);

/*
*@Brief Handles Card Battle equations
*@Param The card that intiated the attack
*@Param The card that is being attacked
*/
int cardFight(Card *attacker, Card *defender);

/*
*@Brief Increments Resource by three
*@Param Player whose resource will be incremented
*/
void generateResource(int player);

#endif