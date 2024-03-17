/*
 * License: ZOSL (Zef OpenSource License)
 * Version: 1.0
 * 
 * Copyright (c) 2024 Armin Asefi <https://github.com/0xDeviI>
 * 
 * Created Date: Wednesday, February 14th 2024, 3:15:24 am
 * Author: Armin Asefi
 * 
 * This license agreement (the "License") is a legal agreement between 
 * you (an individual or entity) and Armin Asefi (the "Licensor") 
 * regarding the use of the software provided by the Licensor.
 * 
 * By accessing, viewing, or sharing the source code of the software
 * provided by the Licensor, you agree to be bound by the terms and 
 * conditions of this License.
 * 
 * 1. GRANT OF LICENSE
 * SUBJECT TO THE TERMS AND CONDITIONS OF THIS LICENSE, THE LICENSOR 
 * GRANTS YOU A NON-EXCLUSIVE, NON-TRANSFERABLE, ROYALTY-FREE LICENSE
 * TO ACCESS, VIEW, AND SHARE THE SOURCE CODE OF THE SOFTWARE PROVIDED
 * BY THE LICENSOR.
 * 
 * 2. RESTRICTIONS
 * YOU ARE NOT ALLOWED TO MODIFY, DISTRIBUTE, SUBLICENSE, OR USE THE
 * SOURCE CODE FOR ANY COMMERCIAL PURPOSES WITHOUT THE EXPLICIT WRITTEN
 * PERMISSION OF THE LICENSOR. YOU MAY NOT REVERSE ENGINEER, DECOMPILE,
 * OR DISASSEMBLE THE SOFTWARE.
 * 
 * 3. OWNERSHIP
 * THE LICENSOR RETAINS ALL INTELLECTUAL PROPERTY RIGHTS IN THE SOFTWARE
 * AND THE SOURCE CODE. THIS LICENSE DOES NOT GRANT YOU ANY RIGHTS TO 
 * PATENTS, COPYRIGHTS, TRADE SECRETS, OR ANY OTHER INTELLECTUAL PROPERTY
 * OWNED BY THE LICENSOR.
 * 
 * 4. DISCLAIMER OF WARRANTY
 * THE SOFTWARE IS PROVIDED "AS IS," WITHOUT ANY WARRANTY OF ANY KIND, 
 * EXPRESS OR IMPLIED. THE LICENSOR SHALL NOT BE LIABLE FOR ANY DAMAGES
 * ARISING OUT OF THE USE OR INABILITY TO USE THE SOFTWARE.
 * 
 * 5. TERMINATION
 * THIS LICENSE IS EFFECTIVE UNTIL TERMINATED. THE LICENSOR RESERVES THE
 * RIGHT TO TERMINATE THIS LICENSE AT ANY TIME IF YOU FAIL TO COMPLY WITH
 * ITS TERMS AND CONDITIONS. UPON TERMINATION, YOU MUST CEASE ALL USE OF
 * THE SOFTWARE AND DESTROY ALL COPIES OF THE SOURCE CODE.
 * 
 * 6. GOVERNING LAW
 * THIS LICENSE SHALL BE GOVERNED BY AND CONSTRUED IN ACCORDANCE WITH THE
 * LAWS OF --IRAN, ISLAMIC REPUBLIC--. ANY DISPUTES ARISING UNDER THIS LICENSE
 * SHALL BE SUBJECT TO THE EXCLUSIVE JURISDICTION OF THE COURTS IN --IRAN, ISLAMIC REPUBLIC--.
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "libs/cards/card.h"
#include "libs/lobby/lobby.h"
#include "libs/player/player.h"
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

// Down Arrow  0x1B 0x5B 0x42
// Left Arrow  0x1B 0x5B 0x44
// Right Arrow 0x1B 0x5B 0x43
// Up Arrow    0x1B 0x5B 0x41

int main(int argc, char* argv[]) {
    // Lobby lobby;
    // for (uchar i = 0; i < PLAYERS_AVAILABLE; i++) {
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

    // I don't give a fuck about storms.
    vtprintf(&screen, "Hello👿 world!\nI'm James Woodcrawl!\n");
    vtprintf(&screen, "┌─────────┐\n");


    // create_thread(test, &screen);
    // create_lobby(&lobby, "Freak Fuckers Game!", FOUR_PLAYERS);
    // vtprintf(&screen, lobby.deck[0]->card_ascii_image);

    // destroy_lobby(&lobby);
    // return 0;
    pthread_exit(NULL);
    return 0;
}