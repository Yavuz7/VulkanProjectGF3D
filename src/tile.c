#include "tiles.h"
#include <simple_json.h>
#include "simple_logger.h"

void loadMap(FILE *mapData){

	SJson *map;
	SJString *smap;
	if (!mapData)return;
	map = sj_load(mapData);
	if(!map)return;
	smap = sj_string_to_json_string(map);

}