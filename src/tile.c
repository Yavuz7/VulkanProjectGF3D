#include "tile.h"
#include <simple_json.h>
#include "simple_logger.h"
#include "gfc_text.h"
#include <stdio.h>

Tile tileMap[6][6]; /*Array to store tile map in*/

void loadMap(const char *mapData)
{
	SJson *map;
	SJString *smap;
	int x,y,textPlace;
	textPlace = -1; /*Keeps track of number place for reading from file*/
	if (!mapData)
	{
		slog("Didn't get mapData file parameter ,w,");
		slog_sync();
		return;
	}
	map = sj_load(mapData);
	if (!map)
	{
		slog("Didn't load the mapData using sj_load, ,w,");
		slog_sync();
		return;
	}
	smap = sj_string_to_json_string(map);
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