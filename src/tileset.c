#include "tileset.h"
#include <stdlib.h>

Tileset* create_tileset(const char* file_path){
    Tileset* tileset = malloc(sizeof(Tileset));
    tileset->texture = sfTexture_createFromFile(file_path, NULL);
    tileset->tiles_list = NULL;

    return tileset;
}

void destroy_tileset(Tileset* tileset){
    sfTexture_destroy(tileset->texture);

    while(tileset->tiles_list != NULL){
        List_Tile* tmp = tileset->tiles_list->next;
        destroy_tile(tileset->tiles_list->value);
        free(tileset->tiles_list);
        tileset->tiles_list = tmp;
    }

    free(tileset);
}

Tile* create_tile(char id, sfIntRect rect){
    Tile* tile = malloc(sizeof(Tile));
    tile->id = id;
    tile->rect = rect;

    return tile;
}

void destroy_tile(Tile* tile){
    free(tile);
}

void tileset_add_tile(Tileset* tileset, Tile* tile){
    List_Tile* list_tile = malloc(sizeof(List_Tile));
    list_tile->value = tile;
    list_tile->next = tileset->tiles_list;
    tileset->tiles_list = list_tile;
}

Tile* tileset_find_tile_by_id(Tileset* tileset, char id){
    List_Tile* list_tile = tileset->tiles_list;

    while(list_tile != NULL){
        if(list_tile->value->id == id){
            return list_tile->value;
        }

        list_tile = list_tile->next;
    }

    return NULL;
}