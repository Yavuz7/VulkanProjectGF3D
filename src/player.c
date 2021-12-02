#include "simple_logger.h"
#include "gfc_types.h"

#include "gf3d_camera.h"
#include "player.h"
#include "SDL.h"
#include "gfc_types.h"
#include "gfc_audio.h"
#include "tile.h"

void player_think(Entity *self);
void player_update(Entity *self);
int activeP;
int px,py;
int startx, starty;
int stopper,startCardMovement;
Card *cardPointer,*defender,*attacker;
Sound *cardMoveSound;
Entity *player1, *player2;
Vector3D player1position, player2position;

Uint32 timeStart, timeEnd;
const int cameraDelay = 240;

int moveCount;
enum movement moveHistory[2];

Entity *player_new(int active)
{
    Entity *ent = NULL;
    
    ent = entity_new();
    if (!ent)
    {
        slog("UGH OHHHH, no player for you!");
        return NULL;
    }
	if (active == 1)
	{
		px = 3;
		py = 0;
		
		ent->position.y = -46;
		ent->position.x = 69;
		// ent->rotation.x = -M_PI;
		ent->rotation.x = 10.12f;
		ent->rotation.z = -0.001f;
		ent->position.z = 37.0f;
		vector3d_copy(player1position, ent->position);
	}
	if (active == 0)
	{
		px = 3;
		py = 6;

		ent->position.y = 23*5;
		ent->position.x = 69;
		ent->rotation.x = 10.12f;
		ent->rotation.z = -0.001f;
		ent->position.z = -37.0f;
		vector3d_copy(player2position, ent->position);

	}
	//ent->think = player_think;
//	ent->update = player_update;
	ent->_inuse = active;
	moveCount = 0;
	stopper = 0;
	startCardMovement = 0;
    return ent;
}

void setPlayers()
{
	player2 = player_new(0);
	player2->update = NULL;
	player2->think = NULL;
	player1 = player_new(1);
	player1->think = player_think;
	player1->update = player_update;
	activeP = 1;	
	return;
}

void changeTurn()
{
	if (activeP == 1)
	{
		
		gf3d_camera_set_position(player2->position);
		gf3d_camera_set_rotation(player2->rotation);
		
		slog("player 1 camera y: %f", player1->position.y);
	
		activeP = 2;
		return;

	}
	if (activeP == 2)
	{

		gf3d_camera_set_position(player1->position);
		gf3d_camera_set_rotation(player1->rotation);

		slog("player 2 camera y: %f", player2->position.y);

		activeP = 1;
		return;
	}
}

void player_think(Entity *self)
{
	if (timeEnd + cameraDelay > SDL_GetTicks())
	{
		return;
	}
	//cardMovement(self,py,px);
	//return;
    Vector3D forward;
    Vector3D right;
    Vector3D up;
    const Uint8 * keys;
	

    keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
	

    vector3d_angle_vectors(self->rotation, &forward, &right, &up);
    vector3d_set_magnitude(&forward,1.0f);
    vector3d_set_magnitude(&right,1.0f);
    vector3d_set_magnitude(&up,0.1f);

	if (keys[SDL_SCANCODE_SPACE])
	{
		if (startCardMovement == 0)
		{			
			cardPointer = getCardPointer(getTileOccupation(px, py));
			
			if (cardPointer && cardPointer->_cardType == leader)
			{
				startCardMovement = 2;
				slog("card Movement set to 2");
				
			}
			if (cardPointer && cardPointer->_cardMoved == 0 && cardPointer->_cardType != leader)
			{
				slog("Start Card Movement");
				slog("%s (AP%i ,DP %i,HP %i)", cardPointer->cardName, cardPointer->cardAP, cardPointer->cardDP, cardPointer->cardHPcurrent);
				startCardMovement = 1;
				startx = px;
				starty = py;
				timeEnd = SDL_GetTicks();
				return;
			}
		}		
	}
	if (startCardMovement == 1)
	{
		cardMovement(self, px, py,cardPointer);
		return;
	}
	if (keys[SDL_SCANCODE_BACKSPACE])
	{
		timeEnd = SDL_GetTicks();
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
		if (py >= 6)
		{
			return;
		}
		self->position.y += 23;
		py += 1;
		timeEnd = SDL_GetTicks();
		
		//vector3d_add(self->position, self->position, -right);
        
    }
    if (keys[SDL_SCANCODE_S])
    {
		if (py <= 0)
		{
			return;
		}
		py -= 1;
		timeEnd = SDL_GetTicks();
		self->position.y -= 23;
		
		//vector3d_add(self->position, self->position, right);
    }
    if (keys[SDL_SCANCODE_D])
    {
		if (px >= 6)
		{
			return;
		}
		px += 1;
		timeEnd = SDL_GetTicks();
		self->position.x += 23;
		
		//vector3d_add(self->position, self->position, forward);
    }
    if (keys[SDL_SCANCODE_A])    
    {
		if (px <= 0)
		{
			return;
		}
		px -= 1;
		timeEnd = SDL_GetTicks();
		self->position.x -= 23;

		
		//vector3d_add(self->position, self->position, -forward);
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

void cardMovement(Entity *self,int x, int y,Card *cardPointer)
{
	if (timeEnd + cameraDelay > SDL_GetTicks())
	{
		return;
	}
	Vector3D forward;
	Vector3D right;
	Vector3D up;
	const Uint8 * keys;


	keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_R])
	{
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
		if (px == startx && py == starty)
		{
			slog("Card set in same place");
			//cardPointer->_cardMoved = 1;
			resetMovement();
			return;
		}
		if (checkTileOccupation(px, py) == 1)
		{
			attacker = getCardPointer(getTileOccupation(startx, starty));
			defender = getCardPointer(getTileOccupation(px, py));
			setCardFight(cardPointer);
			int result = cardFight(attacker,defender);
			if (result == 0)
			{
				destroyCard(defender->listReference);
				removeTileOccupation(px, py);
				cardMove(px, py, cardPointer);				
				resetMovement();
				timeEnd = SDL_GetTicks();
				return;
			}
			if (result == 1)
			{
				destroyCard(attacker->listReference);
				removeTileOccupation(startx, starty);				
				resetMovement();
				timeEnd = SDL_GetTicks();
				return;
			}
			if (result == 2)
			{
				movementHelperFight(cardPointer);
				resetMovement();
				timeEnd = SDL_GetTicks();
				return;
			}
		}
		cardMove(px, py, cardPointer);
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
		if (py >= 6)
		{
			return;
		}

		stopper = movementHelperDouble(north, south);
		if (stopper == 1)return;
		
		setCardFight(cardPointer);
		py += 1;
		self->position.y += 23;
		timeEnd = SDL_GetTicks();
		
		//vector3d_add(self->position, self->position, -right);

	}
	if (keys[SDL_SCANCODE_S])
	{
		if (py <= 0)
		{
			return;
		}

		stopper = movementHelperDouble(south, north);
		if (stopper == 1)return;
		setCardFight(cardPointer);
		py -= 1;
		timeEnd = SDL_GetTicks();
		self->position.y -= 23;
		
		//vector3d_add(self->position, self->position, right);
	}
	if (keys[SDL_SCANCODE_D])
	{
		if (px >= 6)
		{
			return;
		}
		stopper = movementHelperDouble(east,west);
		if (stopper == 1)return;
		setCardFight(cardPointer);
		px += 1;
		timeEnd = SDL_GetTicks();
		self->position.x += 23;
		
		//vector3d_add(self->position, self->position, forward);
	}
	if (keys[SDL_SCANCODE_A])
	{
		if (px <= 0)
		{
			return;
		} 
		stopper = movementHelperDouble(west, east);
		if (stopper == 1)return;
		setCardFight(cardPointer);
		px -= 1;
		timeEnd = SDL_GetTicks();
		self->position.x -= 23;

	
		//vector3d_add(self->position, self->position, -forward);
	}
}



int movementHelperDouble(enum movement direction, enum movement opposite)
{
	if (moveCount == 2)
	{
		if (moveHistory[moveCount - 1] == opposite)
		{
			moveCount -= 1;
			moveHistory[moveCount] = none;
			return 0;
		}
		if (moveHistory[moveCount - 1] == none)
		{
			moveHistory[moveCount-1] = direction;
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
		if (moveHistory[moveCount - 1] == opposite)
		{
			moveCount -= 1;
			moveHistory[moveCount] = none;
			return 0;
		}
	}
	moveHistory[moveCount] = direction;
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
	startCardMovement = 0;
	stopper = 0;
	timeEnd = SDL_GetTicks();
	return;
}
//Based on turn open that player's hand
void openHand()
{

}

/*eol@eof*/
