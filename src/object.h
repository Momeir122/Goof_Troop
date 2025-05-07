#ifndef OBJECT_H
#define OBJECT_H

#include "types.h"
#include "player.h"
#include "attack.h"

Object* create_object(sfVector2f pos, const char* texture_path, int pickable, Attack* attack);
void destroy_object(Object* object);
void draw_object(sfRenderWindow* window, Object* object);
void destroy_list_object(List_Object* list_object);
List_Object* list_object_add_object(List_Object* list_object, Object* object);
void list_object_update_objects(sfRenderWindow* window ,List_Object* list_object, Player* player);
void list_object_collide_with_player(List_Object* list_object, Player* player);

#endif