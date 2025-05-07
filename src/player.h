#ifndef PLAYER_H
#define PLAYER_H

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <SFML/Graphics.h>
#include "types.h"
#include "animation.h"
#include "attack.h"
#include <stdlib.h>
#pragma GCC diagnostic pop

Player* create_player(sfVector2f pos, const char* texture_path);
void destroy_player(Player* player);
void draw_player(sfRenderWindow* window, Player* player);
void update_player(sfRenderWindow* window, Player* player);
void player_input_handler(Player* player);
void update_player_current_animation(Player* player);
int player_collide_with_rect(Player* player, sfFloatRect rect, int physics);
int player_interact_rect_colliding_with_object(Player* player, sfFloatRect rect);
void player_update_object_in_hand(Player* player);
void player_drop_object_in_hand(Player* player);
void player_launch_object_in_hand(Player* player);
sfFloatRect player_get_interact_rect(Player* player);
void player_hit(Player* player, Attack* attack);
void player_apply_attack_effects(Player* player);

#endif