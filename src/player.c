#include "simple_logger.h"
#include "gfc_types.h"

#include "gf3d_camera.h"
#include "player.h"
#include "SDL.h"
#include "gfc_types.h"

void player_think(Entity *self);
void player_update(Entity *self);
int px,py;
Uint32 timeStart, timeEnd;

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
	ent->rotation.x = 10.12;
	ent->rotation.z = -0.001;
	ent->position.z = 37;
    return ent;
}


void player_think(Entity *self)
{
    Vector3D forward;
    Vector3D right;
    Vector3D up;
    const Uint8 * keys;
	

    keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
	

    vector3d_angle_vectors(self->rotation, &forward, &right, &up);
    vector3d_set_magnitude(&forward,1.0);
    vector3d_set_magnitude(&right,1.0);
    vector3d_set_magnitude(&up,0.1);

	if (keys[SDL_SCANCODE_P])
	{
		self->rotation.x = 10.12;
		self->rotation.z = -0.001;
		self->position.z = 37;
	}
	if (timeEnd + 300 > SDL_GetTicks())
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
    if (keys[SDL_SCANCODE_SPACE])self->position.z += 0.40;
    if (keys[SDL_SCANCODE_Z])self->position.z -= 0.40;

    if (keys[SDL_SCANCODE_UP])self->rotation.x -= 0.0080;
    if (keys[SDL_SCANCODE_DOWN])self->rotation.x += 0.0080;
    if (keys[SDL_SCANCODE_LEFT])self->rotation.z += 0.0080;
    if (keys[SDL_SCANCODE_RIGHT])self->rotation.z -= 0.0080;

}

void player_update(Entity *self)
{
    if (!self)return;
    gf3d_camera_set_position(self->position);
    gf3d_camera_set_rotation(self->rotation);
}

/*eol@eof*/
