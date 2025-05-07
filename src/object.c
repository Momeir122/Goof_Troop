#include "object.h"
#include <stdlib.h>
#include <stdio.h>

Object* create_object(sfVector2f pos, const char* texture_path, int pickable, Attack* attack){
    Object* object = malloc(sizeof(Object));
    object->pos = pos;
    object->colliding = 1;
    object->pickable = pickable;
    object->velocity = (sfVector2f){0,0};
    object->launched_by_player = 0;
    object->attack = attack;
    object->sprite = sfSprite_create();

    object->texture = sfTexture_createFromFile(texture_path, NULL);

    sfSprite_setTexture(object->sprite, object->texture, sfFalse);
    sfSprite_setScale(object->sprite, (sfVector2f){0.5, 0.5});
    sfSprite_setPosition(object->sprite, pos);

    return object;
}

void destroy_object(Object* object){
    sfTexture_destroy(object->texture);
    sfSprite_destroy(object->sprite);
    destroy_attack(object->attack);
    free(object);
}

void draw_object(sfRenderWindow* window, Object* object){
    sfSprite_setPosition(object->sprite, object->pos);
    sfRenderWindow_drawSprite(window, object->sprite, NULL);
}

void destroy_list_object(List_Object* list_object){
    while(list_object != NULL){
        List_Object* tmp = list_object->next;
        destroy_object(list_object->value);
        free(list_object);
        list_object = tmp;
    }
}

List_Object* list_object_add_object(List_Object* list_object, Object* object){
    List_Object* list = malloc(sizeof(List_Object));
    list->value = object;
    list->next = list_object;

    return list;
}

void list_object_update_objects(sfRenderWindow* window ,List_Object* list_object, Player* player){
    while(list_object != NULL){
        if(list_object->value != player->object_in_hand){
            draw_object( window ,list_object->value);

            list_object->value->pos.x += list_object->value->velocity.x;
            list_object->value->pos.y += list_object->value->velocity.y;
        }
        list_object = list_object->next;
    }
}

void list_object_collide_with_player(List_Object* list_object, Player* player){
    player->object_in_front = NULL;

    while(list_object != NULL){
        if(player_collide_with_rect(player, sfSprite_getGlobalBounds(list_object->value->sprite), list_object->value->colliding)){
            if((list_object->value->velocity.x != 0 || list_object->value->velocity.y != 0) && !list_object->value->launched_by_player){
                player_hit(player, list_object->value->attack);
            }
        }

        if(player_interact_rect_colliding_with_object(player, sfSprite_getGlobalBounds(list_object->value->sprite)) && list_object->value->pickable){
            player->object_in_front = list_object->value;
        }

        list_object = list_object->next;
    }
}