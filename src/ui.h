#ifndef UI_H
#define UI_H

#include "types.h"

void ui_show_player_attacks(sfRenderWindow* window, sfVector2u win_size ,sfView* ui_view, Player* player, sfFont* font);
sfVector2f ui_get_relative_mouse_pos(sfRenderWindow* window, sfView* ui_view);
void ui_show_mode_selection(sfRenderWindow* window, sfVector2u win_size, sfView* ui_view,int* ui_index, const char* ui_modes[4],sfFont* font);
void ui_update(sfRenderWindow* window, sfVector2u win_size ,sfView* ui_view, int* ui_index, const char* ui_modes[4], Player* player, sfFont* font);

#endif