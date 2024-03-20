/*
 * License: ZOSL (Zef OpenSource License)
 * Version: 1.0
 * 
 * Copyright (c) 2024 Armin Asefi <https://github.com/0xDeviI>
 * 
 * Created Date: Thursday, March 7th 2024, 4:06:26 pm
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


#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <stdlib.h>
#include "../thread/thread.h"
#include <sys/ioctl.h>
#include <string.h>
#include <locale.h>
#include <ncurses.h>
#include <stdarg.h>
#include <signal.h>
#ifdef __unix__
    #define OS_WINDOWS 0
    #include <unistd.h>
#elif defined(_WIN32)
    #define OS_WINDOWS 1
    #include <windows.h>
#endif
#include "../core/constants.h"
#include "../core/globals.h"


// Colors
#define VT_COLOR_NORMAL "\e[m"
// ---- Bold text
#define VT_COLOR_BOLD "\e[1m"
// ---- Foregorund colors
#define VT_COLOR_BLACK "\e[30m"
#define VT_COLOR_RED "\e[31m"
// #define VT_COLOR_RED "\e~rf"
#define VT_COLOR_GREEN "\e[32m"
#define VT_COLOR_YELLOW "\e[33m"
#define VT_COLOR_BLUE "\e[34m"
#define VT_COLOR_MAGENTA "\e[35m"
#define VT_COLOR_CYAN "\e[36m"
#define VT_COLOR_WHITE "\e[37m"
// ---- Background colors
#define VT_COLOR_BG_BLACK "\e[40m"
#define VT_COLOR_BG_RED "\e[41m"
#define VT_COLOR_BG_GREEN "\e[42m"
#define VT_COLOR_BG_YELLOW "\e[43m"
#define VT_COLOR_BG_BLUE "\e[44m"
#define VT_COLOR_BG_MAGENTA "\e[45m"
#define VT_COLOR_BG_CYAN "\e[46m"
#define VT_COLOR_BG_WHITE "\e[47m"


typedef struct Size
{
    volatile ushort rows;
    volatile ushort columns;
} Size;


typedef struct Location
{
    ushort x;
    ushort y;
} Location;


typedef struct CellColor
{
    ushort foreground;
    ushort background;
} CellColor;



typedef struct StyleState
{
    ushort code;
    Location location;
    ushort length;
    uint style;
    CellColor colors;
} StyleState;


typedef struct Screen
{
    Size size;
    uchar frame[SCREEN_MAX_ROWS][SCREEN_MAX_COLUMNS];
    Location location;
    uchar *frame_sequence[SCREEN_MAX_PRINTABLE_CHARACTERS];
    uchar is_on_menu;
    StyleState style_state_list[SCREEN_MAX_PRINTABLE_CHARACTERS];
    ushort style_state_list_size;
} Screen;


// I/O data methods //
void flush(void);
void get_uchar(uchar *output);
void get_string(uchar output[]);
// I/O data methods //



// I/O ctl methods //
// ---- Global screen varibales
extern Screen *GLOBALSCR;
extern Size last_frame_size;
extern struct winsize window;


// ---- Arrow keys
extern a_function up_key_handler;
extern a_function down_key_handler;
extern a_function left_key_handler;
extern a_function right_key_handler;
void set_key_handler(ushort key, a_function handler);
void up_key_handlerf(void *arg);                        // handler function
void down_key_handlerf(void *arg);                      // handler function
void left_key_handlerf(void *arg);                      // handler function
void right_key_handlerf(void *arg);                     // handler function
void *arrow_key_hit_check_thread(void *arg);

// ---- Screen
void bound_screen_size(Screen *screen);
void construct_frame_sequence(Screen *screen);
void feed_screen_frame(Screen *screen, uchar character);
void init_screen(Screen *screen, uchar is_global_screen);
void *draw_screen_frame_thread(void *arg);
void draw_screen_frame(Screen *screen);

// ---- VTOutput
void vtput_new_line(Screen *screen);
void vtput_horizontal_tab(Screen *screen);
void vtput_vertical_tab(Screen *screen);
void vtput_formfeed_pagebreak(Screen *screen);
void vtput_carriage_return(Screen *screen);
uchar vtcheck_especial_character(Screen *screen, uchar character, uchar buffer[], ushort *printed_buffer);
void vtputch(Screen *screen, uchar data);
void vtprintf(Screen *screen, uchar *fmt, ...);
void vtgprintf(uchar *fmt, ...);
void vtclear(Screen *screen);
void vtgclear(void);

// ---- Style
uchar is_color_pair_exists(Screen *screen, CellColor cell_color);
void add_screen_style(Screen *screen, ushort y, ushort x, ushort range, short foreground, short background, uint style);
// I/O ctl methods //

#endif // !IO_H