#ifndef __TILE_H__
#define __TILE_H__

#include "gfc_types.h"
#include <stdio.h>

typedef struct
{
	Uint16 _tileType; /* Keeps track of Tile type*/
	Uint8 _tileOccupied; /*Keeps of whether or not there is a monster on top of tile*/
}Tile;

/*
*@Brief Loads tileType from file into map array
*@Param Map data file that the data will be loaded from
*/
void loadMap(FILE *mapData); 

/*
*@Brief Unload Tiles whatever that means
*/

void clearTiles();


#endif
