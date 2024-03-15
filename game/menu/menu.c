#include "menu.h"

void menu_create_game(void) {

}

void menu_join_game(void) {

}

void menu_global_games(void) {

}

void menu_settings(void) {

}

void menu_credit(void) {

}

void menu_exit(void) {

}

void setup_game_menu(Menu *menu) {
    add_menu_option("Create Game", menu, menu_create_game);
    add_menu_option("Join Game", menu, menu_join_game);
    add_menu_option("Global Games", menu, menu_global_games);
    add_menu_option("Settings", menu, menu_settings);
    add_menu_option("Credit", menu, menu_create_game);
    add_menu_option("Exit", menu, menu_exit);
}