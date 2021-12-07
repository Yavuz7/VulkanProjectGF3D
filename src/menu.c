#include "SDL.h"
#include "simple_logger.h"

#include "menu.h"

int index,menuIndex;
Uint32 timeEnd2;
const int menuDelay = 100;
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
	switch (menuIndex)
	{
	case 0:
		break;
	case 1:
		openHand();
		break;
	}
	return;
}

void openMenu(int i)
{
	if (menuIndex == i)
	{
		menuIndex = 0;
		return;
	}
	menuIndex = i;
	return;
}

void openHand()
{
	if (menuSelection(0, 4, 1) == 0)
	{
		return;
	}
}


int menuSelection(int min, int max, int orientation)
{
	if (timeEnd2 + menuDelay > SDL_GetTicks())
	{
		return;
	}
	const Uint8 * keys;
	keys = SDL_GetKeyboardState(NULL);
	//Vertical Menu
	if (orientation == 0)
	{
	
	}
	//Horizontal Menu
	if (orientation == 1)
	{
		if (keys[SDL_SCANCODE_D])
		{
			if (index < max)
			{
				index++;
				slog("Moved index to right 1, Current Index : %i",index);
				timeEnd2 = SDL_GetTicks();
			}
		}
		if (keys[SDL_SCANCODE_A])
		{
			if (index > min)
			{
				index--;
				slog("Moved index to left 1, Current Index : %i", index);
				timeEnd2 = SDL_GetTicks();
			}
		}
	}
	return 0;
}