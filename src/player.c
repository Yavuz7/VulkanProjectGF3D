#include "simple_logger.h"
#include "gfc_types.h"

#include "gf3d_camera.h"
#include "player.h"
#include "SDL.h"
#include "gfc_types.h"
#include "gfc_audio.h"
#include "tile.h"
#include "menu.h"

void player_think(Entity *self);
void player_update(Entity *self);
Uint8 activeP;
Uint8 px,py;
Uint8 startx, starty;
int stopper,startMenu;
Card *cardPointer,*defender,*attacker;
Sound *cardMoveSound;
Entity *playerCamera, *menus;
//Todo : Put these in a struct and intialize it so they are not global
Vector3D player1position, player2position;
Vector3D player1rotation, player2rotation;
Uint8 px1, py1, px2, py2;

Uint32 timeEnd;
const int cameraDelay = 240;

int moveCount;
enum movement moveHistory[2];

Entity *player_new()
{
    Entity *ent = NULL;
    
    ent = entity_new();
    if (!ent)
    {
        slog("UGH OHHHH, no player for you!");
        return NULL;
    }
	ent->think = player_think;
	ent->update = player_update;
	px1 = 3;
	py1 = 0;
	px = px1;
	py = py1;
	ent->position.y = 0;
	ent->position.x = 69;
	ent->position.z = 37.0f;

		// ent->rotation.x = -M_PI;
	ent->rotation.x = 10.12f;
	ent->rotation.z = 0.0f;
	
	vector3d_copy(player1position, ent->position);
	vector3d_copy(player1rotation,ent->rotation);
	vector3d_copy(player2rotation, ent->rotation);

	moveCount = 0;
	stopper = 0;
	startMenu = 0;
    return ent;
}

void setPlayers()
{
	playerCamera = player_new();
	menus = menu_new();
	activeP = 1;
	player2position = vector3d(69.0,0,37.0);
	player2rotation.z = GFC_PI;
	px2 = 3;
	py2 = 6;
	return;
}

void changeTurn()
{
	resetCardMoves();
	if (activeP == 1) // Change to Player 2 Turn
	{
		px1 = px;
		py1 = py;
		player2position.x = px2 * XPOSITIONOFFSET;
		player2position.y = (23*4) + py2 * YPOSITIONOFFSET;
		vector3d_copy(playerCamera->position, player2position);
		vector3d_copy(playerCamera->rotation, player2rotation);
		px = px2;
		py = py2;
		slog("player 1 camera y: %f", playerCamera->position.y);
		generateResource(activeP);
		activeP = 2;
		resetMovement();
		return;

	}
	if (activeP == 2) // Change to Player 1 Turn
	{
		px2 = px;
		py2 = py;
		player1position.x = px1 * XPOSITIONOFFSET;
		player1position.y = py1 * YPOSITIONOFFSET;
		vector3d_copy(playerCamera->position, player1position);
		vector3d_copy(playerCamera->rotation, player1rotation);
		px = px1;
		py = py1;
		slog("player 2 camera y: %f", playerCamera->position.y);
		generateResource(activeP);
		activeP = 1;
		resetMovement();
		return;
	}
}

void player_think(Entity *self)
{
	if (timeEnd + cameraDelay > SDL_GetTicks())
	{
		return;
	}
	if (startMenu != 0)
	{
		if (startMenu == 1)
		{
			cardMovement(self, px, py, cardPointer, faceUp);
			return;
		}
		if (startMenu == 3)
		{
			cardMovement(self, px, py, cardPointer, summoning);
			return;
		}
		int check = checkMenuDone();

		if (check == 0)
		{
			startMenu = 0;
			slog("menu is done!");
			return;
		}
		if (check == 2)
		{
			startMenu = 3;
		}
		slog("checking if menu is done..., state: %i", check);
		timeEnd = SDL_GetTicks() + 250;
		return;
	}

    Vector3D forward;
    Vector3D right;
    Vector3D up;
	int multiple;
    const Uint8 * keys;
	

    keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
	

    vector3d_angle_vectors(self->rotation, &forward, &right, &up);
    vector3d_set_magnitude(&forward,1.0f);
    vector3d_set_magnitude(&right,1.0f);
    vector3d_set_magnitude(&up,0.1f);

	if (keys[SDL_SCANCODE_SPACE])
	{
		if (startMenu == 0)
		{			
			cardPointer = getCardPointer(getTileOccupation(px, py));
			
			if (cardPointer && cardPointer->_cardType == leader && cardPointer->_cardOwner == activeP && cardPointer->_cardMoved == 0)
			{
				startMenu = 2;
				openMenu(startMenu);
				startx = px;
				starty = py;
				slog("card Movement set to 2");
				return;
				
			}
			if (cardPointer && cardPointer->_cardMoved == 0 && cardPointer->_cardType != leader && cardPointer->_cardOwner == activeP && cardPointer->_cardMoved == 0)
			{
				slog("Start Card Movement");
				slog("%s (AP%i ,DP %i,HP %i)", cardPointer->cardName, cardPointer->cardAP, cardPointer->cardDP, cardPointer->cardHPcurrent);
				startMenu = 1;
				startx = px;
				starty = py;
				timeEnd = SDL_GetTicks();
				return;
			}
			if (cardPointer->_cardMoved == 1)
			{
				slog("Card Acted This Turn");
				slog("%s (AP%i ,DP %i,HP %i)", cardPointer->cardName, cardPointer->cardAP, cardPointer->cardDP, cardPointer->cardHPcurrent);
				timeEnd = SDL_GetTicks();
				return;
			}
			if (cardPointer->_cardOwner != activeP)
			{
				slog("Opponent Card");
				slog("%s (AP%i ,DP %i,HP %i)", cardPointer->cardName, cardPointer->cardAP, cardPointer->cardDP, cardPointer->cardHPcurrent);
				timeEnd = SDL_GetTicks();
				return;
			}
		}		
	}


	if (keys[SDL_SCANCODE_BACKSPACE])
	{
		timeEnd = SDL_GetTicks() + 500;
		changeTurn();
		slog("end turn");
		return;
	}
	if (keys[SDL_SCANCODE_P])
	{
		self->rotation.x = 10.12f;
		self->rotation.z = -0.001f;
		self->position.z = 37.0f;
		timeEnd = SDL_GetTicks();
		return;
	}

    if (keys[SDL_SCANCODE_W])
    {   
		cameraMovement(&py, 1, activeP, &self->position.y, YPOSITIONOFFSET);
    }
    if (keys[SDL_SCANCODE_S])
    {
		cameraMovement(&py, -1, activeP, &self->position.y, YPOSITIONOFFSET);
		
    }
    if (keys[SDL_SCANCODE_D])
    {
		cameraMovement(&px, 1, activeP, &self->position.x, XPOSITIONOFFSET);
		
    }
    if (keys[SDL_SCANCODE_A])    
    {
		cameraMovement(&px, -1, activeP, &self->position.x, XPOSITIONOFFSET);
    }

    if (keys[SDL_SCANCODE_X])self->position.z += 0.40f;
    if (keys[SDL_SCANCODE_Z])self->position.z -= 0.40f;

    if (keys[SDL_SCANCODE_UP])self->rotation.x -= 0.0080f;
    if (keys[SDL_SCANCODE_DOWN])self->rotation.x += 0.0080f;
    if (keys[SDL_SCANCODE_LEFT])self->rotation.z += 0.0080f;
    if (keys[SDL_SCANCODE_RIGHT])self->rotation.z -= 0.0080f;

}

void player_update(Entity *self)
{
    if (!self)return;
    gf3d_camera_set_position(self->position);
    gf3d_camera_set_rotation(self->rotation);
}

void cameraMovement(Uint8 * pPointer, int pChange, int currentPlayer, float * pPosition, float offset)
{
	if (!pPointer)return;
	if (!pPosition) return;
	timeEnd = SDL_GetTicks();

	if (currentPlayer == 1)
	{
		if (pChange > 0)
		{
			if (*pPointer >= 6)return;
			*pPosition += offset;
		}
		if (pChange < 0)
		{
			if (*pPointer <= 0)return;	
			*pPosition -= offset;
		}
		*pPointer += pChange;
		slog("px : %i , py : %i", px, py);
		return;
	}
	if (currentPlayer == 2)
	{
		if (pChange < 0)
		{
			if (*pPointer >= 6)return;
			*pPosition += offset;
		}
		if (pChange > 0)
		{
			if (*pPointer <= 0)return;
			*pPosition -= offset;
			
		}
		*pPointer -= pChange;
		slog("px : %i , py : %i", px, py);
		return;		
	}
	return;
}

void cardMovement(Entity *self, int x, int y, Card *cardPointer, enum cardSelectionState state)
{
	if (timeEnd + cameraDelay > SDL_GetTicks())
	{
		return;
	}

	const Uint8 * keys;


	keys = SDL_GetKeyboardState(NULL);

	if (keys[SDL_SCANCODE_R])
	{
		if (state == summoning)
		{
			timeEnd = SDL_GetTicks();
			return;
		}
		if (cardPointer->_cardPosition == Fight)
		{
			setCardDefense(cardPointer);
			timeEnd = SDL_GetTicks();
			return;
		}
		if (cardPointer->_cardPosition == Defense)
		{
			setCardFight(cardPointer);
			timeEnd = SDL_GetTicks();
			return;
		}
	}

	if (keys[SDL_SCANCODE_SPACE]) //Select Card
	{
		if (state == summoning)
		{
			if (checkTileOccupation(px, py) == 1)
			{
				slog("Can't summon on occupied square");
				return;
			}
			playCard(px, py, getMenuIndex(),activeP);
			cardPointer->_cardMoved = 1;
			timeEnd = SDL_GetTicks() + 500;
			startMenu = 0;
			openMenu(startMenu);
			resetMovement();
			return;
		}
		if (px == startx && py == starty)
		{
			slog("Card set in same place");
			//cardPointer->_cardMoved = 1;
			resetMovement();
			cardPointer->_cardMoved = 1;
			return;
		}
		if (checkTileOccupation(px, py) == 1)
		{
			attacker = getCardPointer(getTileOccupation(startx, starty));
			defender = getCardPointer(getTileOccupation(px, py));
			if (defender->_cardOwner == attacker->_cardOwner)
			{
				slog("no friendly fire");
				resetMovement();
				return;
			}
			setCardFight(cardPointer);
			int result = cardFight(attacker,defender);
			if (result == 0)
			{
				destroyCard(defender->listReference);
				removeTileOccupation(px, py);
				cardMove(px, py, cardPointer);				
				resetMovement();
				timeEnd = SDL_GetTicks();
				cardPointer->_cardMoved = 1;
				return;
			}
			if (result == 1)
			{
				destroyCard(attacker->listReference);
				removeTileOccupation(startx, starty);				
				resetMovement();
				timeEnd = SDL_GetTicks();
				cardPointer->_cardMoved = 1;
				return;
			}
			if (result == 2)
			{
				movementHelperFight(cardPointer);
				resetMovement();
				timeEnd = SDL_GetTicks();
				cardPointer->_cardMoved = 1;
				return;
			}
		}
		cardMove(px, py, cardPointer);
		cardPointer->_cardMoved = 1;
		//cardPointer = NULL;
		resetMovement();
		return;
	}

	if (keys[SDL_SCANCODE_P]) //Reset Camera
	{
		self->rotation.x = 10.12f;
		self->rotation.z = -0.001f;
		self->position.z = 37.0f;
	}


	if (keys[SDL_SCANCODE_W])
	{
		if (py > 6)
		{
			py = 6;
			return;
		}

		stopper = movementHelperDouble(north, south);
		if (stopper == 1)return;

		if (state == summoning)
		{
			if (summoningMovementHelper() == 1)
			{
				cameraMovement(&py, 1, activeP, &self->position.y, YPOSITIONOFFSET);
				return;
			}
			return;
		}
		setCardFight(cardPointer);

		cameraMovement(&py, 1, activeP, &self->position.y, YPOSITIONOFFSET);

	}
	if (keys[SDL_SCANCODE_S])
	{
		if (py < 0)
		{
			py = 0;
			return;
		}

		stopper = movementHelperDouble(south, north);
		if (stopper == 1)return;

		if (state == summoning)
		{
			if (summoningMovementHelper() == 1)
			{
				cameraMovement(&py, -1, activeP, &self->position.y, YPOSITIONOFFSET);
				return;
			}
			return;
		}
		setCardFight(cardPointer);

		cameraMovement(&py, -1, activeP, &self->position.y, YPOSITIONOFFSET);
		
	}
	if (keys[SDL_SCANCODE_D])
	{
		if (px > 6)
		{
			px = 6;
			return;
		}

		stopper = movementHelperDouble(east,west);
		if (stopper == 1)return;

		if (state == summoning)
		{
			if (summoningMovementHelper() == 1)
			{
				cameraMovement(&px, 1, activeP, &self->position.x, XPOSITIONOFFSET);
				return;
			}
			return;
		}

		setCardFight(cardPointer);
		
		cameraMovement(&px, 1, activeP, &self->position.x, XPOSITIONOFFSET);
		
	}
	if (keys[SDL_SCANCODE_A])
	{
		if (px < 0)
		{
			px = 0;
			return;
		}

		stopper = movementHelperDouble(west, east);
		if (stopper == 1)return;

		if (state == summoning)
		{
			if (summoningMovementHelper() == 1)
			{
				cameraMovement(&px, -1, activeP, &self->position.x, XPOSITIONOFFSET);
				return;
			}
			return;
		}
		setCardFight(cardPointer);

		cameraMovement(&px, -1, activeP, &self->position.x, XPOSITIONOFFSET);	
	}
}

int summoningMovementHelper()
{
	return 1;
}

int movementHelperDouble(enum movement direction, enum movement opposite)
{
	enum movement d, o;
	if (activeP == 1)
	{
		d = direction;
		o = opposite;
	}
	else
	{
		d = opposite;
		o = direction;
	}
	if (moveCount == 2)
	{
		if (moveHistory[moveCount - 1] == o)
		{
			moveCount -= 1;
			moveHistory[moveCount] = none;
			return 0;
		}
		if (moveHistory[moveCount - 1] == none)
		{
			moveHistory[moveCount-1] = d;
			moveCount += 1;
			return 0;
		}
		else
		{
			return 1;
		}
	}
	if (moveCount == 1)
	{
		if (moveHistory[moveCount - 1] == o)
		{
			moveCount -= 1;
			moveHistory[moveCount] = none;
			return 0;
		}
	}
	moveHistory[moveCount] = d;
	moveCount += 1;
	return 0;
}

void movementHelperFight(Card *cardPointer)
{
	if (moveCount == 1)
	{
		//cardPointer->_cardMoved = 1;
		slog("Card Attacks, Stand Still");
		return;
	}
	if (moveHistory[moveCount - 1] == north)
	{
		cardMove(px, py - 1, cardPointer);
		slog("Card Attacks north, Card moves");
		return;
	}
	if (moveHistory[moveCount - 1] == south)
	{
		cardMove(px, py + 1, cardPointer);
		slog("Card Attacks south, Card moves");
		return;
	}
	if (moveHistory[moveCount - 1] == east)
	{
		cardMove(px - 1, py, cardPointer);
		slog("Card Attacks east, Card moves");
		return;
	}
	if (moveHistory[moveCount - 1] == west)
	{
		cardMove(px + 1, py, cardPointer);
		slog("Card Attacks west, Card moves");
		return;
	}
	
}
void resetMovement()
{
	moveCount = 0;
	moveHistory[0] = none;
	moveHistory[1] = none;
	startMenu = 0;
	stopper = 0;
	timeEnd = SDL_GetTicks();
	return;
}

int getActivePlayer()
{
	return activeP;
}


/*eol@eof*/
