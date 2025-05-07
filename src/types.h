#ifndef TYPES_H
#define TYPES_H

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <SFML/Graphics.h>
#pragma GCC diagnostic pop

typedef struct Tile{
    char id;
    sfIntRect rect;
}Tile;

typedef struct List_Tile{
    Tile* value;
    struct List_Tile* next;
}List_Tile;

typedef struct Tileset{
    sfTexture* texture;
    List_Tile* tiles_list;
}Tileset;

typedef struct Animation{
    sfIntRect rect;
    unsigned int current_frame;
    unsigned int fps;
}Animation;

typedef struct Character_Animation_Handler{
    sfSprite* sprite;
    sfTexture* texture;
    unsigned int frame_width;
    unsigned int frame_height;
    Animation* current_animation;
    sfClock* clock;
    Animation* idle_down_animation;
    Animation* idle_up_animation;
    Animation* idle_right_animation;
    Animation* idle_left_animation;
    Animation* walk_down_animation;
    Animation* walk_up_animation;
    Animation* walk_right_animation; 
    Animation* walk_left_animation;
}Character_Animation_Handler;

typedef struct Attack_Effect{
    const char* name;
    unsigned int damage;
    unsigned int duration;
    unsigned int delay;
}Attack_Effect;

typedef struct Attack_Effect_Instance{
    Attack_Effect* attack_effect;
    unsigned int duration;
    sfClock* duration_clock;
    sfClock* delay_clock;
}Attack_Effect_Instance;

typedef struct List_Attack_Effect_Instance{
    Attack_Effect_Instance* value;
    struct List_Attack_Effect_Instance* next;
}List_Attack_Effect_Instance;

typedef struct Attack{
    const char* name;
    Attack_Effect* effect;
    const char* texture_path;
}Attack;

typedef struct List_Attack{
    Attack* value;
    struct List_Attack* next;
}List_Attack;

typedef struct Object{
    sfSprite* sprite;
    sfTexture* texture;
    sfVector2f pos;
    int colliding;
    int pickable;
    sfVector2f velocity;
    int launched_by_player;
    Attack* attack;
}Object;

typedef struct List_Object{
    Object* value;
    struct List_Object* next;
}List_Object;

typedef struct Character_Stats{
    float speed;
}Character_Stats;

typedef struct Player_Controls{
    sfKeyCode up;
    sfKeyCode down;
    sfKeyCode left;
    sfKeyCode right;
    sfKeyCode interact;
}Player_Controls;

typedef struct Player_Settings{
    Player_Controls controls;
}Player_Settings;

typedef struct Player{
    sfVector2f pos;
    sfVector2f direction;
    Object* object_in_hand;
    Object* object_in_front;
    int isMoving;
    int draw_before_others;
    Character_Animation_Handler* animation_handler;
    List_Attack_Effect_Instance* attack_effect_instances;
    Attack* selected_attack;
    List_Attack* attacks;
    Character_Stats* stats;
    Player_Settings settings;
}Player;

typedef struct Enemy{
    sfVector2f pos;
    sfVector2f direction;
    int isMoving;
    Character_Animation_Handler* animation_handler;
    Character_Stats* stats;
}Enemy;

typedef struct Menu{
    sfFont* font;
    sfText* title_text;
    const char* character_image_path;
    int i_image_path;
    sfTexture* character_texture;
    sfSprite* character_sprite;
}Menu;

typedef struct Map{
    Tileset* tileset;
    unsigned int width;
    unsigned int height;
    const char* tiles_cells;
    List_Object* objects;
}Map;

#endif