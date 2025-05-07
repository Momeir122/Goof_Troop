#ifndef ANIMATION_H
#define ANIMATION_H

#include "types.h"

Character_Animation_Handler* create_character_animation_handler();
void destroy_character_animation_handler(Character_Animation_Handler* character_animation_handler);
void update_character_animation_handler(Character_Animation_Handler* charater_animation_handler);
void character_animation_handler_change_animation(Character_Animation_Handler* charater_animation_handler, Animation* animation);
Animation* create_animation(sfIntRect rect, unsigned int fps);
void destroy_animation(Animation* animation);

#endif 