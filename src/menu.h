#ifndef MENU_H
#define MENU_H

#include "types.h"

Menu* create_menu(sfRenderWindow* window, const char* title);
void destroy_menu(Menu* menu);
void menu_change_character(Menu* menu, int direction);
void update_menu(sfRenderWindow* window, Menu* menu);
const char* Menu_handle(sfRenderWindow* window);

#endif