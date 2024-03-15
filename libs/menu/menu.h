#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../core/constants.h"
#include "../core/globals.h"
#include "../io/io.h"

typedef struct MenuOption
{
    uchar text[MAX_MENU_OPTION_LENGTH];
    function callback;
} MenuOption;


typedef struct Menu
{
    MenuOption *menu_options[MAX_MENU_OPTIONS_PER_MENU];
    uchar size_of_menu_options;
} Menu;

void add_menu_option(uchar *text, Menu *menu, function callback);
void handle_menu(Menu *menu);
void init_menu(Menu *menu);

#endif // !MENU_H