#ifndef __TILE_H__
#define __TILE_H__

#include "gfc_types.h"
#include <stdio.h>
#include "entity.h"
#include "gf3d_model.h"


typedef struct{
	Uint16 _tileType; /* Keeps track of Tile type*/
	Uint8 _tileOccupied; /*Keeps track of whether or not the tile occupied*/
	void *occupation; /*card pointer to card it is occupied by*/
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

/*
*@Brief Sets tile to occupied and sets pointer to card occupying
*@Param x coordinate of card location
*@Param y coordinate of card location
*@Param Pointer to card that is now occupying tile
*/
void setTileOccupation(Uint8 x, Uint8 y, void * Card);

/*
*@Brief Getter for void pointer of tile
*@Param x coordinate of tile 
*@Param y coordinate of tile
*/
void *getTileOccupation(int x, int y);
/*
*@Brief Checks if tile is occupied
*@Param x coordinate of tile
*@Param y coordinate of tile
*/
int checkTileOccupation(int x, int y);
/*
*@Brief Draws tiles in main game loop
*/
void drawTiles();
/*
*@Brief Removes occupation on tile
*@Param x coordinate of where card was
*@Param y coordinate of where card was
*/
void removeTileOccupation(int x, int y);
#endif
