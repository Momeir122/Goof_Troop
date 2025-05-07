#include "animation.h"
#include <stdio.h>
#include <stdlib.h>

Character_Animation_Handler* create_character_animation_handler(){
    return malloc(sizeof(Character_Animation_Handler));
}

void destroy_character_animation_handler(Character_Animation_Handler* character_animation_handler){
    sfSprite_destroy(character_animation_handler->sprite);
    sfTexture_destroy(character_animation_handler->texture);
    if(character_animation_handler->clock != NULL){
        sfClock_destroy(character_animation_handler->clock);
    }

    destroy_animation(character_animation_handler->idle_down_animation);
    destroy_animation(character_animation_handler->idle_up_animation);
    destroy_animation(character_animation_handler->idle_left_animation);
    destroy_animation(character_animation_handler->idle_right_animation);
    destroy_animation(character_animation_handler->walk_down_animation);
    destroy_animation(character_animation_handler->walk_up_animation);
    destroy_animation(character_animation_handler->walk_left_animation);
    destroy_animation(character_animation_handler->walk_right_animation);

    free(character_animation_handler);
}

void update_character_animation_handler(Character_Animation_Handler* character_animation_handler){
    unsigned int current_frame = character_animation_handler->current_animation->current_frame;

    sfIntRect anim_rect = character_animation_handler->current_animation->rect;
    unsigned int frames_count_x = anim_rect.width / character_animation_handler->frame_width;
    unsigned int frames_count_y = anim_rect.height / character_animation_handler->frame_height;

    unsigned int current_x = current_frame % frames_count_x;
    unsigned int current_y = current_frame / frames_count_x;

    sfIntRect frame_rect = {anim_rect.left + current_x * character_animation_handler->frame_width, anim_rect.top + current_y * character_animation_handler->frame_height, character_animation_handler->frame_width, character_animation_handler->frame_height};

    sfSprite_setTextureRect(character_animation_handler->sprite, frame_rect);

    float frame_delay = 1000 / character_animation_handler->current_animation->fps;

    if(sfTime_asMilliseconds(sfClock_getElapsedTime(character_animation_handler->clock))  > frame_delay){
        character_animation_handler->current_animation->current_frame ++;
        sfClock_restart(character_animation_handler->clock);

        if(current_frame > frames_count_x * frames_count_y -2){
            character_animation_handler->current_animation->current_frame = 0;
        }
    }
}

void character_animation_handler_change_animation(Character_Animation_Handler* character_animation_handler, Animation* animation){
    if(character_animation_handler->current_animation != animation){
        character_animation_handler->current_animation = animation;
        sfClock_restart(character_animation_handler->clock);
    }
}

Animation* create_animation(sfIntRect rect, unsigned int fps){
    Animation* anim = malloc(sizeof(Animation));
    anim->current_frame = 0;
    anim->fps = fps;
    anim->rect = rect;

    return anim;
}

void destroy_animation(Animation* animation){
    free(animation);
}