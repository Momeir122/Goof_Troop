#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <SFML/Graphics.h>
#include "player.h"
#include "tileset.h"
#include "map.h"
#include "object.h"
#include "menu.h"
#include "attack.h"
#include "ui.h"
#include <stdio.h>
#pragma GCC diagnostic pop

#define WIN_WIDTH 1200
#define WIN_HEIGHT 800
#define FPS 180
#define UI_RATIO 0.2
#define CAMERA_VIEW_X 320
#define CAMERA_VIEW_Y 160

sfRenderWindow* create_window(const char* title, unsigned int w, unsigned int h){
    sfVideoMode video_mode = {w,h,32};
    return sfRenderWindow_create(video_mode, title, sfDefaultStyle , NULL);
}

void handle_window_events(sfEvent event, Player* player){
    if(event.type == sfEvtKeyPressed){
        if(event.key.code == player->settings.controls.interact){
            if(player->object_in_hand == NULL){
                if(player->object_in_front != NULL){
                    player->object_in_hand = player->object_in_front;
                    player->object_in_hand->colliding = 0;
                    player->object_in_hand->velocity = (sfVector2f){0,0};
                }
            }
            else{
                if(player->isMoving){
                    player_launch_object_in_hand(player);
                }
                else{
                    player_drop_object_in_hand(player);
                }
            }
        }
    }
}


void Game(sfRenderWindow* window, const char* character_image_path){
    Player* player = create_player((sfVector2f){0, 0}, character_image_path);

    sfFont* default_font = sfFont_createFromFile("fonts/console.ttf");

    List_Object* objects = NULL;
    objects = list_object_add_object(objects, create_object((sfVector2f){0,50}, "images/objects/barrel.png", 0, NULL));
    objects = list_object_add_object(objects, create_object((sfVector2f){30,100}, "images/objects/box.png" , 1, NULL));
    objects = list_object_add_object(objects, create_object((sfVector2f){70,50}, "images/objects/barrel.png", 0, NULL));
    objects = list_object_add_object(objects, create_object((sfVector2f){0,120}, "images/objects/box.png", 1, NULL));
    objects = list_object_add_object(objects, create_object((sfVector2f){90,120}, "images/objects/barrel.png" ,1 , NULL));

    Tileset* tileset = create_tileset("images/tiles/beach_tiles.png");
    tileset_add_tile(tileset, create_tile('1',(sfIntRect){0,0,16,16}));
    tileset_add_tile(tileset, create_tile('2',(sfIntRect){32,32,16,16}));
    tileset_add_tile(tileset, create_tile('3',(sfIntRect){400,16,16,16}));

    Map* map = create_map(tileset, objects, 40, 10, "2211111111122222222112111111211212111121121111112112111111211212111121121111112112222222211111111111133333333333333333211211111121121211112112111111211211111121121211112112111111211222222221111111111122111111111222222221121111112112121111211211111121121111112112121111211211111121122222222111111111111333333333333333332112111111211212111121121111112112111111211212111121121111112112222222211111111111");
    
    character_animation_handler_change_animation(player->animation_handler, player->animation_handler->walk_down_animation);
    
    sfView* game_view = sfView_createFromRect((sfFloatRect){0,0,CAMERA_VIEW_X, CAMERA_VIEW_Y});
    sfView_setViewport(game_view, (sfFloatRect){0,0,1,1 - UI_RATIO});
    

    sfView* ui_view = sfView_createFromRect((sfFloatRect){0,WIN_HEIGHT,WIN_WIDTH, WIN_HEIGHT});
    sfView_setViewport(ui_view, (sfFloatRect){0,1 - UI_RATIO,1,UI_RATIO});

    Attack_Effect* attack_effect = create_attack_effect("Poison" ,20, 10000, 1000);

    player->attacks = list_attack_add(player->attacks, create_attack("Poison attack", attack_effect, "images/attacks/green_slash.png"));
    player->attacks = list_attack_add(player->attacks, create_attack("Fire attack", attack_effect, "images/attacks/red_slash.png"));
    player->attacks = list_attack_add(player->attacks, create_attack("Ice attack", attack_effect, "images/attacks/blue_slash.png"));
    player->selected_attack = player->attacks->value;

    player_hit(player, player->selected_attack);

    int ui_index = 0;
    const char* ui_modes[4] = {"ATTACKS", "INVENTORY", "STATS", "SETTINGS"};

    while(sfRenderWindow_isOpen(window)){
        sfRenderWindow_clear(window, sfBlack);
        sfEvent event;

        sfVector2f centered_player_pos = {player->pos.x + sfSprite_getGlobalBounds(player->animation_handler->sprite).width/2, player->pos.y + sfSprite_getGlobalBounds(player->animation_handler->sprite).height/2};

        sfRenderWindow_setView(window, game_view);
        sfView_setCenter(game_view, centered_player_pos);

        while(sfRenderWindow_pollEvent(window, &event)){
            if(event.type == sfEvtClosed){
                sfRenderWindow_close(window);
            }
            if(event.type == sfEvtMouseButtonPressed){
                if(event.mouseButton.button == sfMouseRight){
                    sfVector2f mouse_pos = sfRenderWindow_mapPixelToCoords(window, sfMouse_getPositionRenderWindow(window), game_view);
                    attack_launch(&map->objects, player->selected_attack, centered_player_pos, mouse_pos, 1);
                }
            }
            handle_window_events(event, player);
        }

        update_map(window, map, player, sfView_getSize(game_view).x, sfView_getSize(game_view).y);

        if(player->draw_before_others){
            list_object_update_objects(window, map->objects, player);
            update_player(window, player);
        }
        else{
            update_player(window, player);
            list_object_update_objects(window, map->objects, player);
        }
        if(player->object_in_hand != NULL){
            draw_object(window, player->object_in_hand);
        }

        sfRenderWindow_setView(window, ui_view);

        ui_update(window, (sfVector2u){WIN_WIDTH, WIN_HEIGHT}, ui_view, &ui_index, ui_modes, player, default_font);

        sfRenderWindow_display(window);
    }

    destroy_map(map);
    destroy_player(player);
    destroy_list_object(objects);
    sfView_destroy(game_view);
    sfFont_destroy(default_font);
}

/*Oh my days - Nigga!*/
int main(void){
    sfRenderWindow* window = create_window("Goof Troop", WIN_WIDTH, WIN_HEIGHT);
    sfRenderWindow_setFramerateLimit(window, FPS);

    sfImage* icon_image = sfImage_createFromFile("images/objects/barrel.png");
    sfRenderWindow_setIcon(window, sfImage_getSize(icon_image).x, sfImage_getSize(icon_image).y, sfImage_getPixelsPtr(icon_image));
    sfImage_destroy(icon_image);

    Game(window, Menu_handle(window));

    sfRenderWindow_destroy(window);

    return 0;
}

// gcc src/*.c -o bin/out.exe -I./include -L./lib -lcsfml-graphics -lcsfml-window -lcsfml-system