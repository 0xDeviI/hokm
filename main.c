#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libs/cards/card.h"
#include "libs/lobby/lobby.h"
#include "libs/player/player.h"
// #include <netinet/in.h>

int main(int argc, char* argv[]) {
    Lobby lobby;
    for (uchar i = 0; i < MAX_PLAYERS; i++) {
        lobby.players[i] = (Player*) malloc(sizeof(Player));
        sprintf(lobby.players[i]->name, "Player %d", i + 1);
        lobby.players[i]->size_of_cards = 0;
        lobby.players[i]->type = HUMAN;
    }
    
    create_lobby(&lobby, "Freak Fuckers Game!", FOUR_PLAYERS);

    // for (uchar i = 0; i < MAX_PLAYERS; i++) {
    //     printf("Cards of player %d are:\n", i + 1);
    //     print_deck(lobby.players[i]->cards, lobby.players[i]->size_of_cards);
    // }

    destroy_lobby(&lobby);
    return 0;
}