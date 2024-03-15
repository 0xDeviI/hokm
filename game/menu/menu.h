#ifndef GAME_MENU
#define GAME_MENU

#include "../libs/menu/menu.h"

void menu_create_game(void);
void menu_join_game(void);
void menu_global_games(void);
void menu_settings(void);
void menu_credit(void);
void menu_exit(void);
void setup_game_menu(Menu *menu);

#endif // !GAME_MENU