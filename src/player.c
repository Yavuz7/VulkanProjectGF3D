#include "simple_logger.h"
#include "gfc_types.h"

#include "gf3d_camera.h"
#include "player.h"
#include "SDL.h"
#include "gfc_types.h"
#include "card.h"
#include "tile.h"

void player_think(Entity *self);
void player_update(Entity *self);
int px,py;
int stopper,movementStopper;

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
	movementStopper = 0;
	
    return ent;
}


void player_think(Entity *self)
{
	if (movementStopper == 1)
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
		movementStopper = 1;
	}
	if (keys[SDL_SCANCODE_P])
	{
		self->rotation.x = 10.12f;
		self->rotation.z = -0.001f;
		self->position.z = 37.0f;
	}
	if (timeEnd + cameraDelay > SDL_GetTicks())
	{
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
		slog("position of py %i", py);
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
		slog("position of py %i", py);
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
		slog("position of px %i", px);
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

		slog("position of px %i", px);
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

void cardMovement(Entity *self,int x, int y)
{
	Vector3D forward;
	Vector3D right;
	Vector3D up;
	const Uint8 * keys;


	keys = SDL_GetKeyboardState(NULL);

	if (keys[SDL_SCANCODE_P])
	{
		self->rotation.x = 10.12f;
		self->rotation.z = -0.001f;
		self->position.z = 37.0f;
	}
	if (timeEnd + cameraDelay > SDL_GetTicks())
	{
		return;
	}


	if (keys[SDL_SCANCODE_W])
	{
		if (py >= 6)
		{
			return;
		}
		stopper = movementHelper(north, south);
		if (stopper == 1)return;
		
		
		py += 1;
		self->position.y += 23;
		timeEnd = SDL_GetTicks();
		slog("position of py %i", py);
		//vector3d_add(self->position, self->position, -right);

	}
	if (keys[SDL_SCANCODE_S])
	{
		if (py <= 0)
		{
			return;
		}
		stopper = movementHelper(south, north);
		if (stopper == 1)return;
		
		py -= 1;
		timeEnd = SDL_GetTicks();
		self->position.y -= 23;
		slog("position of py %i", py);
		//vector3d_add(self->position, self->position, right);
	}
	if (keys[SDL_SCANCODE_D])
	{
		if (px >= 6)
		{
			return;
		}
		stopper = movementHelper(east,west);
		if (stopper == 1)return;

		px += 1;
		timeEnd = SDL_GetTicks();
		self->position.x += 23;
		slog("position of px %i", px);
		//vector3d_add(self->position, self->position, forward);
	}
	if (keys[SDL_SCANCODE_A])
	{
		if (px <= 0)
		{
			return;
		}
		stopper = movementHelper(west, east);
		if (stopper == 1)return;
		
		px -= 1;
		timeEnd = SDL_GetTicks();
		self->position.x -= 23;

		slog("position of px %i", px);
		//vector3d_add(self->position, self->position, -forward);
	}
}

int movementHelper(enum movement direction, enum movement opposite)
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
/*eol@eof*/
