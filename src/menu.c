#include "SDL.h"
#include "simple_logger.h"

#include "gf3d_sprite.h"
#include "gfc_audio.h"
#include "menu.h"

int selectionIndex,menuIndex,menuStateIndex,tempIndex;
Uint32 timeEnd2;
const int menuDelay = 190;
void menu_think();
Sprite *tempMenu,*button1,*button2,*arrows;
Sprite *cardBack, *cardImage0, *cardImage1, *cardImage2, *cardImage3, *cardImage4;
Sound *move2,*select2;


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
	if (!move2)
	{
		move2 = gfc_sound_load("sounds/moveSound.wav", 1.0, 1);
		select2 = gfc_sound_load("sounds/select.wav", 0.7, 1);
	}
	return;
}

void openHand()
{
	if (menuStateIndex == 2)
	{
		freeHandUI();
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

void loadHandUI(int player)
{
	
	cardImage0 = gf3d_sprite_load(getImageFromData(player, 0), -1, -1, 1, 0.4, 0.4);
	cardImage0->position = vector2d(90, 290);

	cardImage1 = gf3d_sprite_load(getImageFromData(player, 1), -1, -1, 1, 0.4, 0.4);
	cardImage1->position = vector2d(280, 290);
	
	cardImage2 = gf3d_sprite_load(getImageFromData(player, 2), -1, -1, 1, 0.4, 0.4);
	cardImage2->position = vector2d(490, 290);

	cardImage3 = gf3d_sprite_load(getImageFromData(player, 3), -1, -1, 1, 0.4, 0.4);
	cardImage3->position = vector2d(700, 290);

	cardImage4 = gf3d_sprite_load(getImageFromData(player, 4), -1, -1, 1, 0.4, 0.4);
	cardImage4->position = vector2d(910, 290);

	cardBack = gf3d_sprite_load("images/cardDefaultFace.png", 2050, -1, 1, 0.5, 0.4);
	cardBack->position = vector2d(80, 250);
}


void freeHandUI()
{

	gf3d_sprite_free(cardImage0);
	gf3d_sprite_free(cardImage1);
	gf3d_sprite_free(cardImage2);
	gf3d_sprite_free(cardImage3);
	gf3d_sprite_free(cardImage4);
	gf3d_sprite_free(cardBack);
	
	//gf3d_texture_delete_unused();
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
		gfc_sound_play(select2, 0, 1, -1, -1);
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
				gfc_sound_play(move2, 0, 1, -1, -1);
			}
			else
			{
				
				selectionIndex = 0;
				timeEnd2 = SDL_GetTicks();
			}
		}
		if (keys[SDL_SCANCODE_S])
		{
			if (selectionIndex < max)
			{
				selectionIndex++;
				slog("Moved index Down 1, Current Index : %i", selectionIndex);
				timeEnd2 = SDL_GetTicks();
				gfc_sound_play(move2, 0, 1, -1, -1);
			}
			else
			{
				
				selectionIndex = max;
				timeEnd2 = SDL_GetTicks();
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
				gfc_sound_play(move2, 0, 1, -1, -1);
			}
			else
			{
				
				selectionIndex = max;
				timeEnd2 = SDL_GetTicks();
			}
		}
		if (keys[SDL_SCANCODE_A])
		{
			if (selectionIndex > 0)
			{
				selectionIndex--;
				slog("Moved index to left 1, Current Index : %i", selectionIndex);
				timeEnd2 = SDL_GetTicks();
				gfc_sound_play(move2, 0, 1, -1, -1);
			}
			else
			{
				
				selectionIndex = 0;
				timeEnd2 = SDL_GetTicks();
			}
		}
		return 1;
	}
	
}