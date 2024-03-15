#include "menu.h"

void add_menu_option(uchar *text, Menu *menu, function callback) {
    menu->menu_options[menu->size_of_menu_options] = (MenuOption *) malloc(sizeof(MenuOption));
    MenuOption *menu_option = (MenuOption *) malloc(sizeof(MenuOption));
    strncpy(menu_option->text, text, MAX_MENU_OPTION_LENGTH);
    menu_option->callback = callback;
    menu->menu_options[menu->size_of_menu_options++] = menu_option;
}

void handle_menu(Menu *menu) {
    uchar selected_menu_option = 1;
    while (selected_menu_option != 0) {
        printf("\n");
        for (uchar i = 0 ; i < menu->size_of_menu_options; i++) {
            printf("\t%d. %s\n", i + 1, menu->menu_options[i]->text);
        }
        printf("\n###: ");
        get_uchar(&selected_menu_option);
        if (selected_menu_option <= menu->size_of_menu_options && selected_menu_option != 0) {
            MenuOption *menu_option = menu->menu_options[--selected_menu_option];
            if (menu_option != NULL) {
                function callback_fn = menu_option->callback;
                if (callback_fn != NULL)
                    callback_fn();
            }
            else
                printf("Invalid menu option.\n");
        }
        else 
            printf("Invalid menu option.\n");
    }
}

void init_menu(Menu *menu) {
    menu->size_of_menu_options = 0;
}