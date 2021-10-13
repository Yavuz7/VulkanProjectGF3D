#include "tile.h"
#include <simple_json.h>
#include "simple_logger.h"
#include "gfc_text.h"
#include <stdio.h>
#include <stdlib.h>

Tile tileMap[6][6]; /*Array to store tile map in*/

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

	for (x = 0; x <= 6; x++)
	{
		for (y = 0; y <= 6; y++)
		{
			fscanf(map,"%i", buff);
			tileMap[x][y]._tileType = buff[0];
			//drawTiles(x,y);
			slog("Tile %d , %d set to Type %i", x, y, tileMap[x][y]._tileType);
			slog_sync();
		}
		
	}
	fclose(map);
}
void clearTiles(){
	
}
void drawTiles(int x, int y){

}
