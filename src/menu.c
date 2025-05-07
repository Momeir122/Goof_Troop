#include "menu.h"
#include <stdlib.h>

Menu* create_menu(sfRenderWindow* window, const char* title){
    Menu* menu = malloc(sizeof(Menu));

    menu->character_image_path = "images/characters/fox.png";
    menu->i_image_path = 0;

    menu->font = sfFont_createFromFile("fonts/console.ttf");
    
    menu->title_text = sfText_create();
    sfText_setFont(menu->title_text, menu->font);
    sfText_setString(menu->title_text, title);

    menu->character_texture = sfTexture_createFromFile(menu->character_image_path, NULL);

    menu->character_sprite = sfSprite_create();
    sfSprite_setTexture(menu->character_sprite, menu->character_texture, sfFalse);
    sfSprite_setTextureRect(menu->character_sprite, (sfIntRect){0,0,32,32});
    sfSprite_setPosition(menu->character_sprite, (sfVector2f){sfRenderWindow_getSize(window).x/2, sfRenderWindow_getSize(window).y/2});
    sfSprite_setOrigin(menu->character_sprite, (sfVector2f){16,16});
    sfSprite_setScale(menu->character_sprite, (sfVector2f){5.0, 5.0});

    return menu;
}

void destroy_menu(Menu* menu){
    sfFont_destroy(menu->font);
    sfText_destroy(menu->title_text);
    sfTexture_destroy(menu->character_texture);
    sfSprite_destroy(menu->character_sprite);
    free(menu);
}

void menu_change_character(Menu* menu, int direction){
    const char* characters_images_paths[4] = {"images/characters/fox.png", "images/characters/bird.png", "images/characters/raccoon.png", "images/characters/cat.png"};
    menu->i_image_path += direction;

    if(menu->i_image_path > 3){
        menu->i_image_path = 0;
    }
    else if(menu->i_image_path < 0){
        menu->i_image_path = 3;
    }

    menu->character_image_path = characters_images_paths[menu->i_image_path];
    sfTexture_destroy(menu->character_texture);
    menu->character_texture = sfTexture_createFromFile(menu->character_image_path, NULL);
    sfSprite_setTexture(menu->character_sprite, menu->character_texture, sfFalse);
}

void update_menu(sfRenderWindow* window, Menu* menu){
    sfRenderWindow_drawText(window, menu->title_text, NULL);
    sfRenderWindow_drawSprite(window, menu->character_sprite, NULL);
}

const char* Menu_handle(sfRenderWindow* window){
    Menu* menu = create_menu(window, "Goof Troop");

    int isRunning = 1;

    while(isRunning){
        sfRenderWindow_clear(window, sfBlue);
        sfEvent event;

        while(sfRenderWindow_pollEvent(window, &event)){
            if(event.type == sfEvtClosed){
                isRunning = 0;
                sfRenderWindow_close(window);
            }
            if(event.type == sfEvtKeyPressed){
                if(event.key.code == sfKeyLeft){
                    menu_change_character(menu, -1);
                }
                if(event.key.code == sfKeyRight){
                    menu_change_character(menu, 1);
                }
                if(event.key.code == sfKeyEnter){
                    isRunning = 0;
                }
            }
        }

        update_menu(window, menu);

        sfRenderWindow_display(window);
    }

    const char* res = menu->character_image_path;
    destroy_menu(menu);
    return res;
}