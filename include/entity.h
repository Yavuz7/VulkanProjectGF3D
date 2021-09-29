#ifndef ENTITY_H_
#define ENTITY_H_

#include "gfc_types.h"
#include "gf3d_model.h"

typedef struct Entity_S{
	Uint8 _inuse;/*Keeps track of memory Usage*/
	Model *model;/*pointer to entity to draw*/
	void(*think)();/*pointer to think function*/
	Matrix4 modelMat; //animation matrixfor model
	//etc
}Entity;



/*
*@Brief Intializes Entity
*@Param MaxEntities
*/

void entity_system_init(Uint32 maxEntities); //Initializes entity

/*Pointer to new entity*/

Entity *entity_new();

/*
*@Brief Frees Entities
*@Param Entity being freed
*/

void entity_free(Entity *self);

/*
*@Brief Draws Entities
*@Param Entity being drawn
*/

void entity_draw(Entity *self);

/*
*@Brief Entity Logic
*@Param Entity using Logic
*/

void entity_think(Entity *self);

/*
*@Brief Draw entities 
*@Param 
*/

void entity_draw_all();

/*
*@Brief Entities Act
*@Param 
*/

void entity_think_all();

#endif