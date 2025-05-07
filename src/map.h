#ifndef MAP_H
#define MAP_H

#include "types.h"
#include "tileset.h"
#include "object.h"

Map* create_map(Tileset* tileset, List_Object* objects ,unsigned int w, unsigned int h, const char* cells);
void destroy_map(Map* map);
void draw_map(sfRenderWindow* window, Map* map, unsigned int window_width, unsigned int window_height);
void update_map(sfRenderWindow* window, Map* map, Player* player, unsigned int window_width, unsigned int window_height);

#endif