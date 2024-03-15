#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "libs/cards/card.h"
// #include "libs/lobby/lobby.h"
// #include "libs/player/player.h"
// #include <netinet/in.h>
// #include <sys/socket.h>
// #include <arpa/inet.h>
#include "libs/menu/menu.h"
#include "game/menu/menu.h"

// menu:

// 1. Create Game
// 2. Join Game
// 3. Global Games
// 4. Settings
// 5. Credit
// 6. Exit


int main(int argc, char* argv[]) {
    // Lobby lobby;
    // for (uchar i = 0; i < MAX_PLAYERS; i++) {
    //     lobby.players[i] = (Player*) malloc(sizeof(Player));
    //     sprintf(lobby.players[i]->name, "Player %d", i + 1);
    //     lobby.players[i]->size_of_cards = 0;
    //     lobby.players[i]->type = HUMAN;
    // }

    // Menu menu;
    // init_menu(&menu);
    // setup_game_menu(&menu);
    // handle_menu(&menu);

    Screen screen;
    init_screen(&screen);
    draw_screen_frame(&screen);
    endwin();

    // create_lobby(&lobby, "Freak Fuckers Game!", FOUR_PLAYERS);
    // destroy_lobby(&lobby);
    // return 0;
    pthread_exit(NULL);
    return 0;
}