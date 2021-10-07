#include "tile.h"
#include <simple_json.h>
#include "simple_logger.h"

Tile tileMap[6][6]; /*Array to store tile map in*/

void loadMap(FILE *mapData)
{
	SJson *map;
	SJString *smap;
	int x,y;
	if (!mapData)return;
	map = sj_load(mapData);
	if(!map)return;
	smap = sj_string_to_json_string(map);
	for (x = 0; x <= 6; x++)
	{
		for (y = 0; y <= 6; y++)
		{
			tileMap[x][y]._tileType = 1;
		}

	}
}