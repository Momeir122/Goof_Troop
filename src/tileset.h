#ifndef TILESET_H
#define TILESET_H

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <SFML/Graphics.h>
#include "types.h"
#pragma GCC diagnostic pop

Tileset* create_tileset(const char* file_path);
Tile* create_tile(char id, sfIntRect rect);
void destroy_tile(Tile* tile);
void destroy_tileset(Tileset* tileset);
void tileset_add_tile(Tileset* tileset, Tile* tile);
Tile* tileset_find_tile_by_id(Tileset* tileset, char id);

#endif