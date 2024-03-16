/*
 * License: ZOSL (Zef OpenSource License)
 * Version: 1.0
 * 
 * Copyright (c) 2024 Armin Asefi <https://github.com/0xDeviI>
 * 
 * Created Date: Thursday, March 7th 2024, 4:06:29 pm
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


#include "io.h"

// I/O data methods
void flush(void) {
    uchar _;
    while ((_ = getchar()) != EOF && _ != '\n');
}

void get_uchar(uchar *output) {
    scanf("%hhu", output);
    flush();
}

// NOTE: This function does not cover auto memory allocation for given 'output', 
// you need to allocate it as much as you need yourself.
void get_string(uchar *output[]) {
    scanf("%s", *output);
    flush();
}



// I/O ctl methods
void bound_screen_size(Screen *screen) {
    if (screen->rows > SCREEN_MAX_ROWS)
        screen->rows = SCREEN_MAX_ROWS;
    if (screen->columns > SCREEN_MAX_COLUMNS)
        screen->columns = SCREEN_MAX_COLUMNS;
}

void *screen_resize_thread(void *arg) {
    struct winsize window;
    Screen *screen = (Screen *) arg;
    while (1) {
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);
        screen->rows = window.ws_row;
        screen->columns = window.ws_col;
        bound_screen_size(screen);
    }
}

void feed_screen_frame(Screen *screen, uchar character) {
    for (ushort i = 0 ; i < screen->rows; i++)
        for (ushort j = 0 ; j < screen->columns; j++)
            screen->frame[i][j] = character;
}

void init_screen(Screen *screen) {
    struct winsize window;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);
    screen->x = 0;
    screen->y = 0;
    screen->rows = window.ws_row;
    screen->columns = window.ws_col;
    bound_screen_size(screen);
    feed_screen_frame(screen, SCREEN_EMPTY_SPACE_CHARACTER);

    // ncurses init
    setlocale(LC_ALL, ""); // required in order to show ASCII emojis
    initscr();
    cbreak();
    noecho();
    curs_set(0);
}

void *draw_screen_frame_thread(void *arg) {
    Screen *screen = (Screen *) arg;
    create_thread(screen_resize_thread, (void *) screen);
    while (1) {
        erase();
        for (ushort i = 0; i < screen->rows; i++) {
            for (ushort j = 0; j < screen->columns; j++) {
                mvprintw(i, j, "%c", screen->frame[i][j]);
            }
        }
        refresh();
        // if (SCREEN_FRAME_USE_DELAYED_REFRESH_RATE == 1)
        //     napms(SCREEN_FRAME_REFRESH_RATE_MS);
    }
}

void draw_screen_frame(Screen *screen) {
    create_thread(draw_screen_frame_thread, (void *) screen);
}

void vtput_new_line(Screen *screen) {
    screen->x = (screen->y != screen->rows ? screen->y++ : 1) == -1;
}

void vtput_horizontal_tab(Screen *screen) {
    screen->x += 4;
    if (screen->x > screen->columns)
        screen->x = (screen->y != screen->rows ? screen->y++ : 1) == -1;
}

void vtput_vertical_tab(Screen *screen) {
    if (screen->y != screen->rows)
        screen->y++;
}


void vtput_formfeed_pagebreak(Screen *screen) {
    if (screen->y != screen->rows)
        screen->y++;
}

void vtput_carriage_return(Screen *screen) {
    screen->x = 0;
}

void vtputch(Screen *screen, uchar data) {
    if (data == '\n')
        vtput_new_line(screen);
    else if (data == '\t')
        vtput_horizontal_tab(screen);
    else if (data == '\f')
        vtput_formfeed_pagebreak(screen);
    else if (data == '\r')
        vtput_carriage_return(screen);
    else if (data == '\v')
        vtput_vertical_tab(screen);
    else
        screen->frame[screen->y][screen->x++] = data;
}


void vtprintf(Screen *screen, uchar *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    uchar buffer[SCREEN_MAX_PRINTABLE_CHARACTERS];
    vsnprintf(buffer, SCREEN_MAX_PRINTABLE_CHARACTERS, fmt, args);
    va_end(args);
    ushort size_of_buffer = strlen(buffer);
    ushort printed_buffer = 0;
    while (printed_buffer < size_of_buffer && screen->y != screen->rows) {
        if (screen->x == screen->columns)
            vtput_new_line(screen);
        vtputch(screen, buffer[printed_buffer++]);
    }
    return;
}