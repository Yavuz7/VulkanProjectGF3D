#ifndef __MENU_H__
#define __MENU_H__

#include "entity.h"
#include "card.h"
#include "field_play.h"


/**
* @brief Create a new menu entity
* @return NULL on error, or an menu entity pointer on success
*/

Entity *menu_new();

/**
* @brief Opens or closes menu
* @Brief Index of Menu to Open
*/
void openMenu(int i);

/*
*@brief Hand Menu Functions
*/
void openHand();

/*
*@brief Menu selection function for menus to use when open
*@Brief Minimum value of menu index
*@Brief Maximum value of menu index
*@Brief Whether the menu is vertical or horizontal for input controls
*/
int menuSelection(int min, int max, int orientation);

#endif
