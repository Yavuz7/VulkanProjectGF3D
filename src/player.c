#include "simple_logger.h"
#include "gfc_types.h"

#include "gf3d_camera.h"
#include "player.h"
#include "SDL.h"
#include "gfc_types.h"

#include "tile.h"

void player_think(Entity *self);
void player_update(Entity *self);
int px,py;
int startx, starty;
int stopper,startCardMovement;
Card *cardPointer,*defender,*attacker;

Uint32 timeStart, timeEnd;
const int cameraDelay = 240;

int moveCount;
enum movement moveHistory[2];

Entity *player_new(Vector3D position)
{
    Entity *ent = NULL;
    
    ent = entity_new();
    if (!ent)
    {
        slog("UGH OHHHH, no player for you!");
        return NULL;
    }
	px = 3;
	py = 0;
	
//    ent->model = gf3d_model_load("dino");
    ent->think = player_think;
    ent->update = player_update;
    vector3d_copy(ent->position,position);
	ent->position.y = -46;
	ent->position.x = 69;
   // ent->rotation.x = -M_PI;
	ent->rotation.x = 10.12f;
	ent->rotation.z = -0.001f;
	ent->position.z = 37.0f;
	moveCount = 0;
	stopper = 0;
	startCardMovement = 0;
	
    return ent;
}


void player_think(Entity *self)
{

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
	if (timeEnd + cameraDelay > SDL_GetTicks())
	{
		return;
	}
	if (keys[SDL_SCANCODE_SPACE])
	{
		if (startCardMovement == 0)
		{			
			cardPointer = getTileOccupation(px, py);
			if (cardPointer && cardPointer->_cardType == leader)
			{
				startCardMovement = 2;
				
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
	if (keys[SDL_SCANCODE_M])
	{
		reward();
		timeEnd = SDL_GetTicks();
		return;
	}
	if (keys[SDL_SCANCODE_P])
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
	Vector3D forward;
	Vector3D right;
	Vector3D up;
	const Uint8 * keys;

	if (timeEnd + cameraDelay > SDL_GetTicks())
	{
		return;
	}
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
	if (keys[SDL_SCANCODE_SPACE])
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
			attacker = getTileOccupation(startx, starty);
			defender = getTileOccupation(px, py);
			setCardFight(cardPointer);
			int result = cardFight(attacker,defender);
			setCardHP(attacker);
			setCardHP(defender);
			if (result == 0)
			{
				destroyCard(defender);
				removeTileOccupation(px, py);
				cardMove(px, py, cardPointer);				
				resetMovement();
				timeEnd = SDL_GetTicks();
				return;
			}
			if (result == 1)
			{
				destroyCard(attacker);
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
	if (keys[SDL_SCANCODE_P])
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

int cardFight(Card *attacker,Card *defender)
{
	if (!attacker)return;
	slog("%s (AP%i ,DP %i,HP %i)", defender->cardName, defender->cardAP, defender->cardDP, defender->cardHPcurrent);
	//Attacking into a attack monster
	if (defender->_cardPosition == Fight)
	{		
		defender->cardHPcurrent = defender->cardHPcurrent - (attacker->cardAP - (defender->cardDP / 2));
		if (defender->cardHPcurrent <= 0)
		{
			slog("Defender Dead");
			return 0; // Victor attacker
		}
		attacker->cardHPcurrent = attacker->cardHPcurrent - (defender->cardAP - (attacker->cardDP / 2));
		if (attacker->cardHPcurrent <= 0)
		{
			slog("Attacker Dead, Defender HP %i",defender->cardHPcurrent);
			return 1; // Victor defender
		}
		slog("Attacker Hp: %i, Defender HP: %i", attacker->cardHPcurrent, defender->cardHPcurrent);
		return 2; //no victor
	}
	if (defender->_cardPosition == Defense)
	{ 
		defender->cardHPcurrent = defender->cardHPcurrent - (attacker->cardAP - defender->cardDP);
		if (defender->cardHPcurrent <= 0)
		{
			slog("Defender Dead");
			return 0; // Victor attacker
		}
		slog("Defender HP %i", defender->cardHPcurrent);
		return 2;
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


/*eol@eof*/
