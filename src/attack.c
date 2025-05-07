#include "attack.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "utils.h"

#define ATTACK_DELAY_EPS 2
#define ATTACK_SPEED 1.5

Attack_Effect* create_attack_effect(const char* name, unsigned int damage, unsigned int duration, unsigned int delay){
    Attack_Effect* attack_effect = malloc(sizeof(Attack_Effect));
    attack_effect->name = name;
    attack_effect->damage = damage;
    attack_effect->duration = duration;
    attack_effect->delay = delay;

    return attack_effect;
}

void destroy_attack_effect(Attack_Effect* attack_effect){
    free(attack_effect);
}

Attack* create_attack(const char* name ,Attack_Effect* attack_effect, const char* texture_path){
    Attack* attack = malloc(sizeof(Attack));
    attack->effect = attack_effect;
    attack->name = name;
    attack->texture_path = texture_path;

    return attack;
}
void destroy_attack(Attack* attack){
    free(attack);
}

Attack_Effect_Instance* create_attack_effect_instance(Attack_Effect* attack_effect){
    Attack_Effect_Instance* instance = malloc(sizeof(Attack_Effect_Instance));
    instance->attack_effect = attack_effect;
    instance->duration = attack_effect->duration + attack_effect->delay; // Adding delay to damage n times and not n -1 times
    instance->duration_clock = sfClock_create();
    instance->delay_clock = sfClock_create();

    return instance;
}
void destroy_attack_effect_instance(Attack_Effect_Instance* instance){
    sfClock_destroy(instance->duration_clock);
    sfClock_destroy(instance->delay_clock);
    free(instance);
}

List_Attack_Effect_Instance* list_attack_effect_instance_add(List_Attack_Effect_Instance* list, Attack_Effect_Instance* attack_effect_instance){
    List_Attack_Effect_Instance* new = malloc(sizeof(List_Attack_Effect_Instance));
    new->value = attack_effect_instance;
    new->next = list;

    return new;
}

void list_attack_effect_instance_destroy(List_Attack_Effect_Instance* list){
    while(list != NULL){
        List_Attack_Effect_Instance* tmp = list->next;
        destroy_attack_effect_instance(list->value);
        free(list);
        list = tmp;
    }
}
List_Attack_Effect_Instance* list_attack_effect_instance_remove(List_Attack_Effect_Instance* list, Attack_Effect_Instance* attack_effect_instance){
    if(list == NULL){
        return NULL;
    }

    for(List_Attack_Effect_Instance* curr = list; curr->next != NULL; curr = curr->next){
        if(curr->next->value == attack_effect_instance){
            List_Attack_Effect_Instance* to_remove = curr->next;
            curr->next = curr->next->next;

            destroy_attack_effect_instance(to_remove->value);
            free(to_remove);

            return curr;
        }
    }

    return list;
}

Attack_Effect_Instance* list_attack_effect_instance_find_by_attack_effect(List_Attack_Effect_Instance* list, Attack_Effect* attack_effect){
    while(list != NULL){
        if(list->value->attack_effect == attack_effect){
            return list->value;
        }

        list = list->next;
    }

    return NULL;
}

int attack_effect_instance_finished(Attack_Effect_Instance* attack_effect_instance){
    unsigned int elapsed_time = sfTime_asMilliseconds(sfClock_getElapsedTime(attack_effect_instance->duration_clock));

    return elapsed_time > attack_effect_instance->duration;
}

int attack_effect_instance_should_damage(Attack_Effect_Instance* attack_effect_instance){
    unsigned int elapsed_time = sfTime_asMilliseconds(sfClock_getElapsedTime(attack_effect_instance->delay_clock));

    if(elapsed_time > attack_effect_instance->attack_effect->delay){
        sfClock_restart(attack_effect_instance->delay_clock);
        return 1;
    }
    else{
        return 0;
    }
}

void list_attack_destroy_with_attacks(List_Attack* list){
    while(list != NULL){
        List_Attack* next = list->next;
        destroy_attack_effect(list->value->effect);
        destroy_attack(list->value);
        free(list);
        list = next;
    }
}

void list_attack_destroy_without_attacks(List_Attack* list){
    while(list != NULL){
        List_Attack* next = list->next;
        free(list);
        list = next;
    }
}

List_Attack* list_attack_add(List_Attack* list, Attack* attack){
    List_Attack* new = malloc(sizeof(List_Attack));
    new->value = attack;
    new->next = list;
    return new;
}

void attack_launch(List_Object** objects, Attack* attack, sfVector2f origin, sfVector2f target, int launched_by_player){
    Object* newObject = create_object(origin, attack->texture_path, 0, attack);
    *objects = list_object_add_object(*objects, newObject);

    sfSprite_setOrigin(newObject->sprite, (sfVector2f){sfTexture_getSize(newObject->texture).x/2, sfTexture_getSize(newObject->texture).y/2});
    sfSprite_rotate(newObject->sprite, rad_to_deg(-atan2( target.x - origin.x , target.y - origin.y)) + 180);
    
    newObject->launched_by_player = 0;
    newObject->colliding = 0;
    newObject->velocity = (sfVector2f){get_dir(origin, target).x * ATTACK_SPEED,get_dir(origin, target).y * ATTACK_SPEED} ;
}