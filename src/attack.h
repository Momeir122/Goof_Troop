#ifndef ATTACK_H
#define ATTACK_H

#include "types.h"
#include "animation.h"
#include "object.h"

Attack* create_attack(const char* name ,Attack_Effect* attack_effect, const char* texture_path);
void destroy_attack(Attack* attack);
Attack_Effect* create_attack_effect(const char* name, unsigned int damage, unsigned int duration, unsigned int delay);
void destroy_attack_effect(Attack_Effect* attack_effect);
Attack_Effect_Instance* create_attack_effect_instance(Attack_Effect* attack_effect);
void destroy_attack_effect_instance(Attack_Effect_Instance* instance);
List_Attack_Effect_Instance* list_attack_effect_instance_add(List_Attack_Effect_Instance* list, Attack_Effect_Instance* attack_effect_instance);
void list_attack_effect_instance_destroy(List_Attack_Effect_Instance* list);
List_Attack_Effect_Instance* list_attack_effect_instance_remove(List_Attack_Effect_Instance* list, Attack_Effect_Instance* attack_effect_instance);
Attack_Effect_Instance* list_attack_effect_instance_find_by_attack_effect(List_Attack_Effect_Instance* list, Attack_Effect* attack_effect);
int attack_effect_instance_should_damage(Attack_Effect_Instance* attack_effect_instance);
int attack_effect_instance_finished(Attack_Effect_Instance* attack_effect_instance);
void list_attack_destroy_with_attacks(List_Attack* list);
void list_attack_destroy_without_attacks(List_Attack* list);
List_Attack* list_attack_add(List_Attack* list, Attack* attack);
void attack_launch(List_Object** objects, Attack* attack, sfVector2f origin, sfVector2f target, int launched_by_player);

#endif