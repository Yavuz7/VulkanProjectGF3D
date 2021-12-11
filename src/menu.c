#include "SDL.h"
#include "simple_logger.h"

#include "gf3d_sprite.h"
#include "menu.h"

int selectionIndex,menuIndex,menuStateIndex,tempIndex;
Uint32 timeEnd2;
const int menuDelay = 190;
void menu_think();
Sprite *tempMenu,*button1,*button2,*arrows;

Entity *menu_new()
{
	Entity *ent = NULL;

	ent = entity_new();

	if (!ent)
	{
		slog("Entity been bleh'd");
		return NULL;
	}

	ent->think = menu_think;

}

void menu_think()
{
	//Indexs at 2 because in player.c startmenu 1 is for cardMovement
	switch (menuIndex)
	{
	case 0:
		break;
	case 2:
		openHand();
		break;
	case 4:
		openMainMenu();
		break;
	}
	return;
}

void openMenu(int i)
{
	selectionIndex = 0;
	tempIndex = 0;
	menuStateIndex = 1;
	menuIndex = i;
	timeEnd2 = SDL_GetTicks() + 500;
	return;
}

void openHand()
{
	if (menuStateIndex == 2)
	{
		menuIndex = 0;
		return;
	}
	menuStateIndex = menuSelection(4, 1);
	return;

}

void openMainMenu()
{
	if (menuStateIndex == 2)
	{
		if (selectionIndex == 1)
		{
			menuStateIndex = 1;
		}
		if (selectionIndex == 0)
		{
			gf3d_sprite_free(tempMenu);
			gf3d_sprite_free(button1);
			gf3d_sprite_free(button2);
			gf3d_sprite_free(arrows);
			menuStateIndex = 0;
			menuIndex = 0;
			return;
		}
	}
	menuStateIndex = menuSelection(1, 0);
	drawMainMenuUI();
	return;
}

void handSelectionFinal()
{
	if (timeEnd2 + menuDelay > SDL_GetTicks())
	{
		return;
	}

}

void loadMainMenuUI()
{
	if (!tempMenu)
	{
		slog("Loading Menu Sprites");
		tempMenu = gf3d_sprite_load("images/mainmenu.png", -1, -1, 1, 1.35, 1.25);
		button1 = gf3d_sprite_load("images/mainmenustart.png", -1, -1, 1, 1, 1);
		button2 = gf3d_sprite_load("images/aibuttong.png", -1, -1, 1, 1, 1);
		arrows = gf3d_sprite_load("images/arrows.png", -1, -1, 1, 1, 1);
		button1->position = vector2d(100,25);
		button2->position = vector2d(100, 240);
		arrows->position = vector2d(100, 25);
	}

}

void drawMainMenuUI()
{
	if (tempIndex == selectionIndex)
	{
		return;
	}
	if (selectionIndex == 0)
	{
		arrows->position = vector2d(100, 25);
		tempIndex = selectionIndex;
	}
	else
	{
		arrows->position = vector2d(100, 240);
		tempIndex = selectionIndex;
	}

}
int checkMenuDone()
{
	return menuStateIndex;
}

int getMenuIndex()
{
	return selectionIndex;
}

int menuSelection(int max, int orientation)
{
	if (timeEnd2 + menuDelay > SDL_GetTicks())
	{
		return;
	}
	const Uint8 * keys;
	keys = SDL_GetKeyboardState(NULL);

	if (keys[SDL_SCANCODE_SPACE])
	{
		slog("Selection Made");
		timeEnd2 = SDL_GetTicks();
		return 2;
		
	}
	//Vertical Menu
	if (orientation == 0)
	{
		if (keys[SDL_SCANCODE_W])
		{
			if (selectionIndex > 0)
			{
				selectionIndex--;
				slog("Moved index Up 1, Current Index : %i", selectionIndex);
				timeEnd2 = SDL_GetTicks();
			}
			else
			{
				selectionIndex = 0;
			}
		}
		if (keys[SDL_SCANCODE_S])
		{
			if (selectionIndex < max)
			{
				selectionIndex++;
				slog("Moved index Down 1, Current Index : %i", selectionIndex);
				timeEnd2 = SDL_GetTicks();
			}
			else
			{
				selectionIndex = max;
			}
		}
		return 1;
	}
	//Horizontal Menu
	if (orientation == 1)
	{
		if (keys[SDL_SCANCODE_D])
		{
			if (selectionIndex < max)
			{
				selectionIndex++;
				slog("Moved index to right 1, Current Index : %i", selectionIndex);
				timeEnd2 = SDL_GetTicks();
			}
			else
			{
				selectionIndex = max;
			}
		}
		if (keys[SDL_SCANCODE_A])
		{
			if (selectionIndex > 0)
			{
				selectionIndex--;
				slog("Moved index to left 1, Current Index : %i", selectionIndex);
				timeEnd2 = SDL_GetTicks();
			}
			else
			{
				selectionIndex = 0;
			}
		}
		return 1;
	}
	
}