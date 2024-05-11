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
 * LAWS OF --IRAN, ISLAMIC REPUBLIC OF--. ANY DISPUTES ARISING UNDER THIS LICENSE
 * SHALL BE SUBJECT TO THE EXCLUSIVE JURISDICTION OF THE COURTS IN --IRAN, ISLAMIC REPUBLIC OF--.
 * 
 */


#ifndef CONSTANTS_H
#define CONSTANTS_H

// ########### Lobby Constants ###########
#define LOBBY_MAX_GAME_NAME 32


// ########### Player Constants ###########
#define PLAYER_MAX_NAME 32
#define PLAYERS_AVAILABLE 4


// ########### Cards Constants ###########
#define CARDS_AVAILABLE 52
#define CARDS_PER_HAND 17
// #define CARD_INVALID_FLAG 0xA0
#define CARDS_DECK_SHUFFLE_DEPTH 1
#define CARDS_MAX_ASCII_IMAGE_LENGTH 160
#define CARDS_PER_ROW 5
#define CARDS_MAX_ASCII_IMAGE_HEIGHT 7


// ########### Menu Constants ###########
#define MENU_MAX_OPTION_LENGTH 64
#define MENU_MAX_OPTIONS_PER_MENU 16
#define MENU_MAX_NAME 32


// ########### Memory Logic Constants ###########
#define MEM_MAX_STRING_BUFFER_LENGTH 32


// ########### Screen Constants ###########
// The game could run on 2K resolution monitor at its max size: 2560x1440.
// Values 247x64 calculated from width and height of a console in ARCH GNU/Linux
// with display size of 1920x1080 as it was 185x48.
#define SCREEN_MAX_COLUMNS 247
#define SCREEN_MAX_ROWS 64
// The game could run on a terminal sized as 80x24 in its minimum screen size.
// Values 80x24 are gathered from a - 1366x768 display - running a Debian 12 
// GNU/Linux - gnome desktop environment - default gnome terminal - and its default size. 
#define SCREEN_MIN_COLUMNS 80
#define SCREEN_MIN_ROWS 24
#define SCREEN_MAX_PRINTABLE_CHARACTERS (SCREEN_MAX_COLUMNS * SCREEN_MAX_ROWS)
#define SCREEN_EMPTY_SPACE_CHARACTER ' '
#define SCREEN_CELL_MAX_DATA_LENGTH 16


// ########### Multi-tasking Constants ###########
#define MT_MAX_PARALLEL_THREADS 8


// ########### Audio Constants ###########
#define AUDIO_PATH_MAX_NAME 256


// ########### MSPC Constants ###########
#define MSPC_2080V_FILE_NAME "mspc2080.key"
#define MSPC_MACHINE_ID_FILE_NAME "/etc/machine-id"
// defines maximum file length for MSPC-2080 padding variation
#define MSPC_2080V_FILE_LENGTH 8192


// ########### System Constants ###########
#define SYS_MAX_BUFFER_SIZE 2048
#define SYS_MAX_EXE_PATH_LENGTH 1024


// ########### CAS Constants ###########
#define CAS_V1_FILE_NAME "gasset.cas"
#define CAS_V1_DEFAULT_LENGTH 2192      // lf-padding + AES padding + cas header: (1024 * 2) + (64 * 2) + 9
#define CAS_V1_JUNK_SIZE 1024
#define CAS_V1_AES_PAD_SIZE 64
#define CAS_V1_MAX_FILE_ID_LENGTH 64
#define CAS_V1_ASSET_PADDING 128
#define CAS_V1_ASSET_SEP_LENGTH 8
#define CAS_V1_CSPRNG "/dev/urandom"
#define CAS_V1_MAX_PAYLOAD_HEADER_LENGTH 65536

#endif