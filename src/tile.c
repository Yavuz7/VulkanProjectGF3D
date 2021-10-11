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
	char buff[255];
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
		slog_sync();
		return;
	}
	slog("Map Opened ^w^");
	slog_sync();
	return;
/*	smap = sj_string_to_json_string(map);
	if (!smap)
	{
		slog("Didn't convert the json to string, ,w, ");
		slog_sync();
		return;
	}
	
	for (x = 0; x <= 6; x++)
	{
		for (y = 0; y <= 6; y++)
		{
			textPlace += 1;
			if (smap->text[textPlace] == ",")
			{
				do
				{
					textPlace += 1;
					if (textPlace > 98)
					{
						slog("Too many numbers scanned through owo");
						slog_sync();
						return;
					}
				} while (smap->text[textPlace] == ',');
			}

			tileMap[x][y]._tileType = smap->text[textPlace];
			
			if (tileMap[x][y]._tileType > 10)
			{
				slog("Tile was set above ten, which means something that wasn't a number was read");
				slog_sync();
			}
			
			slog("Tile %d , %d set to Type %i", x, y, tileMap[x][y]._tileType);
			slog_sync();
		}
		
	}
}
void drawTiles(){
	int x, y;
	for (x = 0; x <= 6; x++)
	{
		for (y = 0; y <= 6; y++)
		{

		}
	}*/
}