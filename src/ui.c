#include "ui.h"
#include <stdio.h>

#define HIGHLIGHT_COLOR sfYellow
#define TEXT_MARGIN_VERTICAL 100
#define TEXT_MARGIN_HORIZONTAL 200
#define MODE_SELECTION_SIZE 100

sfVector2f ui_get_relative_mouse_pos(sfRenderWindow* window, sfView* ui_view){
    return sfRenderWindow_mapPixelToCoords(window, sfMouse_getPositionRenderWindow(window), ui_view);
}

void ui_show_mode_selection(sfRenderWindow* window, sfVector2u win_size, sfView* ui_view,int* ui_index, const char* ui_modes[4],sfFont* font){
    unsigned int modes_count = 4;

    for(int i = 0; i < modes_count; i++){
        sfText* text = sfText_create();
        sfText_setFont(text, font);
        sfText_setString(text, ui_modes[i]);
        sfText_setPosition(text, (sfVector2f){i*TEXT_MARGIN_HORIZONTAL, win_size.y });
        sfText_setScale(text, (sfVector2f){1,2});

        if(i == *ui_index){
            sfText_setColor(text, HIGHLIGHT_COLOR);
        }

        sfRenderWindow_drawText(window, text, NULL);

        if(sfMouse_isButtonPressed(sfMouseLeft)){
            sfFloatRect text_rect = sfText_getGlobalBounds(text);
            sfVector2f mouse_pos = ui_get_relative_mouse_pos(window, ui_view);

            if(sfFloatRect_contains(&text_rect, mouse_pos.x, mouse_pos.y)){
                *ui_index = i;
            }
        }

        sfText_destroy(text);
    }
}

void ui_show_player_attacks(sfRenderWindow* window, sfVector2u win_size ,sfView* ui_view, Player* player, sfFont* font){
    List_Attack* attacks = player->attacks;

    unsigned int i = 0;

    while(attacks != NULL){
        sfText* text = sfText_create();
        sfText_setFont(text, font);
        sfText_setString(text,attacks->value->name);
        sfText_setPosition(text, (sfVector2f){0, win_size.y + i*TEXT_MARGIN_VERTICAL + MODE_SELECTION_SIZE});
        sfText_setScale(text, (sfVector2f){1,2});

        if(attacks->value == player->selected_attack){
            sfText_setColor(text, HIGHLIGHT_COLOR);
        }

        sfRenderWindow_drawText(window, text, NULL);

        if(sfMouse_isButtonPressed(sfMouseLeft)){
            sfFloatRect text_rect = sfText_getGlobalBounds(text);
            sfVector2f mouse_pos = ui_get_relative_mouse_pos(window, ui_view);

            if(sfFloatRect_contains(&text_rect, mouse_pos.x, mouse_pos.y)){
                player->selected_attack = attacks->value;
            }
        }

        sfText_destroy(text);
        attacks = attacks->next;
        i++;
    }
}

void ui_update(sfRenderWindow* window, sfVector2u win_size ,sfView* ui_view, int* ui_index, const char* ui_modes[4], Player* player, sfFont* font){
    ui_show_mode_selection(window, win_size, ui_view, ui_index, ui_modes, font);
    
    if(*ui_index == 0){
        ui_show_player_attacks(window, win_size, ui_view, player, font);
    }
}