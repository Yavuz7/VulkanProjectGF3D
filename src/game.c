#include <SDL.h>            

#include "simple_logger.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"

#include "gf3d_vgraphics.h"
#include "gf3d_pipeline.h"
#include "gf3d_swapchain.h"
#include "gf3d_model.h"
#include "gf3d_camera.h"
#include "gf3d_texture.h"
#include "gf3d_sprite.h"

#include "entity.h"
#include "agumon.h"
#include "player.h"
#include "world.h"
#include "card.h"
#include "tile.h"
#include "menu.h"

int main(int argc,char *argv[])
{
    int done = 0;
    int a;
    Uint8 validate = 0;
    const Uint8 * keys;
    
	Sprite *mouse = NULL;
	Sprite *overlay = NULL;
	int mousex, mousey;
	float mouseFrame = 0;
    World *w;
    
    for (a = 1; a < argc;a++)
    {
        if (strcmp(argv[a],"-disable_validate") == 0)
        {
            validate = 0;
        }
    }
    
    init_logger("gf3d.log");    
    slog("gf3d begin");
    gf3d_vgraphics_init(
        "gf3d",                 //program name
        1200,                   //screen width
        700,                    //screen height
        vector4d(0.51,0.75,1,1),//background color
        0,                      //fullscreen
        validate                //validation
    );
	slog_sync();
    
    entity_system_init(1024);

	mouse = gf3d_sprite_load("images/pointer.png", 32, 32, 16);
	//overlay = gf3d_sprite_load("images/temoverlay.png", 32, 32, 1);
	slog_sync();
	//setCardData("00001", c );
	//setCardData("00002", c);

	loadMap("mapDat/map1.txt");
	
	setCardFileData();

    // main game loop
	slog_sync();
    gf3d_camera_set_scale(vector3d(1,1,1));

    slog("gf3d main loop begin");
	setPlayers();
	

    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame

		SDL_GetMouseState(&mousex, &mousey);

		mouseFrame += 0.01;
		if (mouseFrame >= 16)mouseFrame = 0;

        entity_think_all();
        entity_update_all();
        gf3d_camera_update_view();
        gf3d_camera_get_view_mat4(gf3d_vgraphics_get_view_matrix());

        // configure render command for graphics command pool
        // for each mesh, get a command and configure it from the pool
        gf3d_vgraphics_render_start();

              //  world_draw(w);
                entity_draw_all();
				drawTiles();

				gf3d_sprite_draw(mouse, vector2d(mousex, mousey), vector2d(1, 1), (Uint32)mouseFrame);
				//gf3d_sprite_draw(overlay, vector2d(.2, .2), vector2d(2, 2), 1);
        gf3d_vgraphics_render_end();

        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
    }    
	endDuel();
    world_delete(w);
	clearTiles();
    
    vkDeviceWaitIdle(gf3d_vgraphics_get_default_logical_device());    
    //cleanup
    slog("gf3d program end");
    slog_sync();
    return 0;
}

/*eol@eof*/
