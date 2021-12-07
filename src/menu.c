#include "SDL.h"
#include "simple_logger.h"

#include "menu.h"

int selectionIndex,menuIndex,menuStateIndex;
Uint32 timeEnd2;
const int menuDelay = 190;
void menu_think();

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
	//Indexs at 1 because in player.c startmenu 1 is for cardMovement
	switch (menuIndex)
	{
	case 1:
		break;
	case 2:
		openHand();
		break;
	}
	return;
}

void openMenu(int i)
{
	selectionIndex = 0;
	menuStateIndex = 1;
	menuIndex = i;
	timeEnd2 = SDL_GetTicks() + 500;
	return;
}

void openHand()
{
	if (menuStateIndex == 2)
	{
		return;
	}
	menuStateIndex = menuSelection(4, 1);
	return;

}

int checkMenuDone()
{
	return menuStateIndex;
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
			if (selectionIndex < max)
			{
				selectionIndex++;
				slog("Moved index Up 1, Current Index : %i", selectionIndex);
				timeEnd2 = SDL_GetTicks();
			}
			else
			{
				selectionIndex = max;
			}
		}
		if (keys[SDL_SCANCODE_S])
		{
			if (selectionIndex > 0)
			{
				selectionIndex--;
				slog("Moved index Down 1, Current Index : %i", selectionIndex);
				timeEnd2 = SDL_GetTicks();
			}
			else
			{
				selectionIndex = 0;
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