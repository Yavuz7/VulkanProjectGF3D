#ifndef __TILE_H__
#define __TILE_H__

#include "gfc_types.h"
#include <stdio.h>
#include "entity.h"
#include "gf3d_model.h"

typedef struct{
	Uint16 _tileType; /* Keeps track of Tile type*/
	Uint8 _tileOccupied; /*Keeps of whether or not there is a monster on top of tile*/
	Model *tileModel; /*The model of the tile*/
	Matrix4 tileModelMatrix;/*Model Matrix of tile*/

}Tile;

/*
*@Brief Loads tileType from file into map array
*@Param Map data file that the data will be loaded from
*/
void loadMap(char *mapData); 

/*
*@Brief Unload Tiles whatever that means
*/



void clearTiles();

/*
*@Brief load tile models on a grid
*@Param Tile to set data to
*@Param x value of grid tile
*@Param y value of grid tile
*@Todo Tile should draw the models
*/
void setTile(Tile *t,int x, int y);

void drawTiles();
#endif
