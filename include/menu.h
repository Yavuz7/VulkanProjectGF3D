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
*@brief Plays Card at index 
*/

void handSelectionFinal();

/*
*@brief Menu selection function for menus to use when open
*@Brief Maximum value of menu index , Min is always 0
*@Brief Whether the menu is vertical or horizontal for input controls
*@returns 1 if still selecting something or 2 if done
*/
int menuSelection(int max, int orientation);

/*
*@brief Checks if menu is done
*@returns menuStateIndex, if it's 0 the menu is done
*/
int checkMenuDone();

#endif
