#include "entity.h"
#include "simple_logger.h"
#include "gf3d_model.h"
typedef struct
{
	Entity *entity_list; //List of entities
	Uint32 entity_count; //Number of entities
}EntityManager;

static EntityManager entity_manager = { 0 };

void entity_system_close(){
	int i;
	for (i = 0; i < entity_manager.entity_count; i++){
		entity_free(&entity_manager.entity_list[i]);
	}
	free(entity_manager.entity_list);
	memset(&entity_manager, 0, sizeof(EntityManager));
	slog("entity_system closed");
}

void entity_system_init(Uint32 maxEntities){
	entity_manager.entity_list = gfc_allocate_array(sizeof(Entity), maxEntities);
	if (entity_manager.entity_list == NULL)
	{
		slog("failed to allocate, cant allocate zero entites");
		return;
	}
	entity_manager.entity_count = maxEntities;
	atexit(entity_system_close);
	slog("entity_system intialized");
}

Entity *entity_new()
{
int i;
for (i = 0; i < entity_manager.entity_count; i++)
{
	if (!entity_manager.entity_list[i]._inuse)
	{
		entity_manager.entity_list[i]._inuse = 1;
		gfc_matrix_identity(entity_manager.entity_list[i].modelMat);
		return &entity_manager.entity_list[i];
	}
	slog("No free space in entity list");
	return NULL;
}
return NULL;
}
void entity_free(Entity *self){
	if (!self) return;
	gf3d_model_free(self->model);
	memset(self, 0, sizeof(Entity));
}


void entity_draw(Entity *self, Uint32 bufferFrame, VkCommandBuffer commandBuffer)
{
	if (!self)return;
	gf3d_model_draw(self->model, bufferFrame, commandBuffer, self->modelMat);
}



void entity_think(Entity *self);



void entity_draw_all(Uint32 bufferFrame, VkCommandBuffer commandBuffer)
{
	int i;
	for (i = 0; i < entity_manager.entity_count; i++){
		if (!entity_manager.entity_list[i]._inuse){
			continue; //skip iteration if it already exists
		}
		entity_draw(&entity_manager.entity_list[i], bufferFrame, commandBuffer);
	}
}


void entity_think_all();

