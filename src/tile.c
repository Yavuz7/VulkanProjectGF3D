#include "tile.h"
#include <simple_json.h>
#include "simple_logger.h"
#include "gfc_text.h"
#include "gfc_matrix.h"
#include <stdio.h>
#include <stdlib.h>

Tile tileMap[7][7]; /*Array to store tile map in*/

void loadMap(char *mapData)
{
	FILE *map;
	char buff[256];
	int x, y;
	if (!mapData)
	{
		slog("Didn't get mapData file parameter ,w,");
		slog_sync();
		return;
	}
	map = fopen(mapData, "r");
	if (!map)
	{
		slog("Didn't load the mapData %s using fopen, ,w,", mapData);
		fclose(map);
		slog_sync();
		return;
	}
	slog("Map Opened ^w^");
	slog_sync();

	for (x = 0; x < 7; x++)
	{
		for (y = 0; y < 7; y++)
		{
			fscanf(map,"%i", buff);
			tileMap[x][y]._tileType = buff[0];
			setTile(&tileMap[x][y],x,y);
			slog("Tile %d , %d set to Type %i", x, y, tileMap[x][y]._tileType);
			slog_sync();
		}
		
	}
	fclose(map);
}
void clearTiles(){
	
}
void setTile(Tile *t, int x, int y)
{
	
	gfc_matrix_identity(t->tileModelMatrix);
	t->tileModel = gf3d_model_load("tile1");

	gfc_matrix_scale(t->tileModelMatrix, vector3d(2, 2, 2));
	gfc_matrix_translate(t->tileModelMatrix, vector3d(1 + x * 23, 1 + y * 23, -10));
	//gf3d_model_draw(t->tileModel, t->tileModelMatrix);
	return;
}

void drawTiles()
{

	int x, y;
	for (x = 0; x < 7; x++)
	{
		for (y = 0; y < 7; y++)
		{
			Tile *temp = &tileMap[x][y];
			if (!temp->tileModel)
			{
				//slog("nu tilemodel");
				return;
			}
			gf3d_model_draw(temp->tileModel, temp->tileModelMatrix);
		}

	}
}
