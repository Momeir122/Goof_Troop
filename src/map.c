#include "map.h"
#include <stdio.h>
#include <stdlib.h>

Map* create_map(Tileset* tileset,List_Object* objects ,unsigned int w, unsigned int h, const char* tiles_cells){
    Map* map = malloc(sizeof(Map));
    map->tileset = tileset;
    map->objects = objects;
    map->height = h;
    map->width = w;
    map->tiles_cells = tiles_cells;

    return map;
}

void destroy_map(Map* map){
    destroy_tileset(map->tileset);
    destroy_list_object(map->objects);
    free(map);
}

void draw_map(sfRenderWindow* window, Map* map, unsigned int window_width, unsigned int window_height) {
    for (int y = 0; y < map->height; y++) {
        for (int x = 0; x < map->width; x++) {
            Tile* tile = tileset_find_tile_by_id(map->tileset, map->tiles_cells[x + y * map->width]);
            if (!tile) continue;

            sfSprite* sprite = sfSprite_create();
            sfSprite_setTexture(sprite, map->tileset->texture, sfFalse);
            sfSprite_setTextureRect(sprite, tile->rect);

            // Positionner le tile en fonction de sa position dans la grille
            sfSprite_setPosition(sprite, (sfVector2f){
                x * tile->rect.width,
                y * tile->rect.height
            });

            sfRenderWindow_drawSprite(window, sprite, NULL);
            sfSprite_destroy(sprite);
        }
    }
}

void update_map(sfRenderWindow* window, Map* map, Player* player, unsigned int window_width, unsigned int window_height){
    draw_map(window, map, window_width, window_height);
    list_object_collide_with_player(map->objects, player);
}