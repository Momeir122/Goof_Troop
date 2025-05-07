#include "player.h"
#include <stdio.h>

#define LAUNCH_SPEED 1.5

Player* create_player(sfVector2f pos, const char* texture_path){
    Player* player = malloc(sizeof(Player));

    player->pos = pos;
    player->isMoving = 0;
    player->direction = (sfVector2f){0,1};
    player->draw_before_others = 1;
    player->attack_effect_instances = NULL;
    player->attacks = NULL;

    player->stats = malloc(sizeof(Character_Stats));
    player->stats->speed = 0.5;

    /* Creating the animation handler*/
    Character_Animation_Handler* player_animation_handler = create_character_animation_handler();
    player_animation_handler->texture = sfTexture_createFromFile(texture_path, NULL);
    player_animation_handler->frame_width = 32;
    player_animation_handler->frame_height = 32;
    player_animation_handler->clock = sfClock_create();

    /* Setting the animations */
    player_animation_handler->idle_down_animation = create_animation((sfIntRect){0,0,128,32}, 4);
    player_animation_handler->idle_right_animation = create_animation((sfIntRect){0,32,128,32}, 4);
    player_animation_handler->idle_left_animation = create_animation((sfIntRect){0,64,128,32}, 4);
    player_animation_handler->idle_up_animation = create_animation((sfIntRect){0,96,128,32}, 4);
    
    player_animation_handler->walk_down_animation = create_animation((sfIntRect){0,160,128,64}, 12);
    player_animation_handler->walk_right_animation = create_animation((sfIntRect){0,288,128,64}, 12);
    player_animation_handler->walk_left_animation = create_animation((sfIntRect){0,224,128,64}, 12);
    player_animation_handler->walk_up_animation = create_animation((sfIntRect){0,352,128,64}, 12);;

    player_animation_handler->sprite = sfSprite_create();
    sfSprite_setTexture(player_animation_handler->sprite, player_animation_handler->texture, sfFalse);

    player->animation_handler = player_animation_handler;
    character_animation_handler_change_animation(player->animation_handler, player->animation_handler->idle_down_animation);

    /* Setting controls*/
    Player_Controls controls;
    controls.down = sfKeyS;
    controls.up = sfKeyZ;
    controls.left = sfKeyQ;
    controls.right = sfKeyD;
    controls.interact = sfKeySpace;

    player->settings.controls = controls;

    player->object_in_hand = NULL;

    return player;
}

void destroy_player(Player* player){
    destroy_character_animation_handler(player->animation_handler);
    list_attack_effect_instance_destroy(player->attack_effect_instances);
    list_attack_destroy_with_attacks(player->attacks);
    free(player->stats);
    free(player);
}

void draw_player(sfRenderWindow* window, Player* player){
    sfRenderWindow_drawSprite(window, player->animation_handler->sprite, NULL);
}

void player_input_handler(Player* player){
    if(sfKeyboard_isKeyPressed(player->settings.controls.left)){
        player->direction.y = 0;
        player->direction.x = -1;
        player->isMoving = 1;
    }
    else if(sfKeyboard_isKeyPressed(player->settings.controls.right)){
        player->direction.y = 0;
        player->direction.x = 1;
        player->isMoving = 1;
    }
    else if(sfKeyboard_isKeyPressed(player->settings.controls.up)){
        player->direction.x = 0;
        player->direction.y = -1;
        player->isMoving = 1;
    }
    else if(sfKeyboard_isKeyPressed(player->settings.controls.down)){
        player->direction.x = 0;
        player->direction.y = 1;
        player->isMoving = 1;
    }
    else{
        player->isMoving = 0;
    }
}

void update_player_current_animation(Player* player){
    if(player->isMoving){
        if(player->direction.x == 1){
            character_animation_handler_change_animation(player->animation_handler, player->animation_handler->walk_right_animation);
        }
        if(player->direction.x == -1){
            character_animation_handler_change_animation(player->animation_handler, player->animation_handler->walk_left_animation);
        }
        if(player->direction.y == 1){
            character_animation_handler_change_animation(player->animation_handler, player->animation_handler->walk_down_animation);
        }
        if(player->direction.y == -1){
            character_animation_handler_change_animation(player->animation_handler, player->animation_handler->walk_up_animation);
        }
    }
    else{
        if(player->direction.x == 1){
            character_animation_handler_change_animation(player->animation_handler, player->animation_handler->idle_right_animation);
        }
        if(player->direction.x == -1){
            character_animation_handler_change_animation(player->animation_handler, player->animation_handler->idle_left_animation);
        }
        if(player->direction.y == 1){
            character_animation_handler_change_animation(player->animation_handler, player->animation_handler->idle_down_animation);
        }
        if(player->direction.y == -1){
            character_animation_handler_change_animation(player->animation_handler, player->animation_handler->idle_up_animation);
        }
    }
}

void update_player(sfRenderWindow* window, Player* player){
    player_input_handler(player);
    update_player_current_animation(player);
    draw_player(window, player);
    update_character_animation_handler(player->animation_handler);
    player_update_object_in_hand(player);
    player_apply_attack_effects(player);
    
    player->pos.x += player->direction.x * player->stats->speed * player->isMoving;
    player->pos.y += player->direction.y * player->stats->speed * player->isMoving;

    sfSprite_setPosition( player->animation_handler->sprite, player->pos);
}

int player_collide_with_rect(Player* player, sfFloatRect rect, int physics){
    sfFloatRect player_rect = sfSprite_getGlobalBounds(player->animation_handler->sprite);
    player_rect.left += player_rect.width/3;
    player_rect.width = player_rect.width/3;

    player_rect.top += player_rect.height/2;
    player_rect.height = player_rect.height/4;

    if(sfFloatRect_intersects(&player_rect, &rect, NULL) && physics){
        player->pos.x -= player->direction.x * player->stats->speed;
        player->pos.y -= player->direction.y * player->stats->speed;
        return 1;
    }
    else{
        return 0;
    }

    player_rect = sfSprite_getGlobalBounds(player->animation_handler->sprite);

    /* Manage drawing order*/

    if(sfFloatRect_intersects(&player_rect, &rect, NULL)){
        if(player_rect.top  > rect.top + rect.height){
            player->draw_before_others = 1;
        }
        else if(player_rect.top + player_rect.height < rect.top){
            player->draw_before_others = 0;
        }
    }
}

sfFloatRect player_get_interact_rect(Player* player){
    sfFloatRect interact_rect = sfSprite_getGlobalBounds(player->animation_handler->sprite);
    interact_rect.left += interact_rect.width * player->direction.x;
    interact_rect.top += interact_rect.height * player->direction.y;

    return interact_rect;
}

int player_interact_rect_colliding_with_object(Player* player, sfFloatRect rect){
    sfFloatRect interact_rect = player_get_interact_rect(player);

    return sfFloatRect_intersects(&interact_rect, &rect, NULL);
}

void player_update_object_in_hand(Player* player){
    if(player->object_in_hand != NULL){
        float dy = abs(sfSprite_getGlobalBounds(player->animation_handler->sprite).height - 1.25*sfSprite_getGlobalBounds(player->object_in_hand->sprite).height);
        float dx = sfSprite_getGlobalBounds(player->animation_handler->sprite).width /4 ;

        player->object_in_hand->pos = (sfVector2f){player->pos.x + dx, player->pos.y - dy};
    }
}

void player_drop_object_in_hand(Player* player){
    player->object_in_hand->colliding = 1;
    sfFloatRect interact_rect = player_get_interact_rect(player);

    float posx = player->pos.x + sfSprite_getGlobalBounds(player->animation_handler->sprite).width/2  - sfSprite_getGlobalBounds(player->object_in_hand->sprite).width/2;
    float posy = player->pos.y +sfSprite_getGlobalBounds(player->animation_handler->sprite).width/2 - sfSprite_getGlobalBounds(player->object_in_hand->sprite).height/2;
    float dx = (sfSprite_getGlobalBounds(player->animation_handler->sprite).left/5) * player->direction.x;
    float dy = (sfSprite_getGlobalBounds(player->animation_handler->sprite).left/6) * player->direction.y;

    player->object_in_hand->pos = (sfVector2f){posx +dx, posy +dy};

    player->object_in_hand = NULL;
}

void player_launch_object_in_hand(Player* player){
    player->object_in_hand->velocity = (sfVector2f){player->direction.x * LAUNCH_SPEED, player->direction.y * LAUNCH_SPEED};
    
    player->object_in_hand->colliding = 0;
    player->object_in_hand->launched_by_player = 1;
    sfFloatRect interact_rect = player_get_interact_rect(player);

    float posx = player->pos.x + sfSprite_getGlobalBounds(player->animation_handler->sprite).width/2  - sfSprite_getGlobalBounds(player->object_in_hand->sprite).width/2;
    float posy = player->pos.y +sfSprite_getGlobalBounds(player->animation_handler->sprite).width/2 - sfSprite_getGlobalBounds(player->object_in_hand->sprite).height/2;
    float dx = (sfSprite_getGlobalBounds(player->animation_handler->sprite).left/5) * player->direction.x;
    float dy = (sfSprite_getGlobalBounds(player->animation_handler->sprite).left/6) * player->direction.y;

    player->object_in_hand->pos = (sfVector2f){posx +dx, posy +dy};

    player->object_in_hand = NULL;
}

void player_hit(Player* player, Attack* attack){
    if(attack == NULL){
        return;
    }

    printf("Player got hit by %s .\n", attack->name);

    Attack_Effect_Instance* instance = list_attack_effect_instance_find_by_attack_effect(player->attack_effect_instances, attack->effect);

    if(instance == NULL){
        Attack_Effect_Instance* newInstance = create_attack_effect_instance(attack->effect);
        player->attack_effect_instances = list_attack_effect_instance_add(player->attack_effect_instances, newInstance);
    }
    else{
        instance->duration += attack->effect->duration;
    }
}

void player_apply_attack_effects(Player* player){
    List_Attack_Effect_Instance* list = player->attack_effect_instances;

    while(list != NULL){
        Attack_Effect_Instance* attack_effect_instance = list->value;

        if(attack_effect_instance_finished(attack_effect_instance)){
           list_attack_effect_instance_remove(player->attack_effect_instances, attack_effect_instance);
        }
        else{
            if(attack_effect_instance_should_damage(attack_effect_instance)){
                printf("The player took %d damage by %s .\n", attack_effect_instance->attack_effect->damage, attack_effect_instance->attack_effect->name);
            }
        }

        list = list->next;
    }
}