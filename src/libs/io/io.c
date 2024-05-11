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
 * LAWS OF --IRAN, ISLAMIC REPUBLIC OF--. ANY DISPUTES ARISING UNDER THIS LICENSE
 * SHALL BE SUBJECT TO THE EXCLUSIVE JURISDICTION OF THE COURTS IN --IRAN, ISLAMIC REPUBLIC OF--.
 * 
 */


#include "io.h"

// I/O data methods
void flush(void) {
    uchar _;
    while ((_ = getchar()) != EOF && _ != '\n');
}


void get_uchar(uchar *output) {
    *output = getch();
}


// NOTE: This function does not cover auto memory allocation for given 'output', 
// you need to allocate it as much as you need yourself.
void get_string(uchar output[]) {
    getstr(output);
    // flush();
}



// I/O ctl methods
// ---- Global screen varibales
Screen *GLOBALSCR = NULL;
struct winsize window;
uchar screen_size_revert = 0;
volatile sig_atomic_t is_resized = 0;
uchar cwd[SYS_MAX_EXE_PATH_LENGTH];

// ---- Arrow keys
a_function up_key_handler;
a_function down_key_handler;
a_function left_key_handler;
a_function right_key_handler;
void set_key_handler(ushort key, a_function handler) {
    switch (key)
    {
    case KEY_UP:
        up_key_handler = handler;
        break;
    case KEY_DOWN:
        down_key_handler = handler;
        break;
    case KEY_LEFT:
        left_key_handler = handler;
        break;
    case KEY_RIGHT:
        right_key_handler = handler;
        break;
    }
}


void up_key_handlerf(void *arg) {
    if (up_key_handler != NULL)
        up_key_handler(arg);
}


void down_key_handlerf(void *arg) {
    if (down_key_handler != NULL)
        down_key_handler(arg);
}


void left_key_handlerf(void *arg) {
    if (left_key_handler != NULL)
        left_key_handler(arg);
}


void right_key_handlerf(void *arg) {
    if (right_key_handler != NULL)
        right_key_handler(arg);
}


void handle_resize_signal(int signal) {
    if (signal == SIGWINCH)
        is_resized = 1;
}

void *arrow_key_hit_check_thread(void *arg) {
    // Up Arrow         0x1B 0x5B 0x41
    // Down Arrow       0x1B 0x5B 0x42
    // Left Arrow       0x1B 0x5B 0x44
    // Right Arrow      0x1B 0x5B 0x43
    WINDOW *win = newwin(1, 1, 0, 0);
    keypad(win, TRUE);
    int ch;
    while (1) {
        ch = wgetch(win);
        switch(ch) {
            case KEY_UP:
            up_key_handlerf(NULL);
                break;
            case KEY_DOWN:
            down_key_handlerf(NULL);
                break;
            case KEY_LEFT:
            left_key_handlerf(NULL);
                break; 
            case KEY_RIGHT:
            right_key_handlerf(NULL);
                break;
        }
    }
}

// ---- Screen
void bound_screen_size(Size *size) {
    if (size->rows > SCREEN_MAX_ROWS)
        size->rows = SCREEN_MAX_ROWS;
    if (size->columns > SCREEN_MAX_COLUMNS)
        size->columns = SCREEN_MAX_COLUMNS;
    if (size->rows < SCREEN_MIN_ROWS)
        size->rows = SCREEN_MIN_ROWS;
    if (size->columns < SCREEN_MIN_COLUMNS)
        size->columns = SCREEN_MIN_COLUMNS;
		
		// heys
}


void construct_frame_sequence(Screen *screen) {
    for (ushort i = 0 ; i < screen->size.rows; i++)
        for (ushort j = 0 ; j < screen->size.columns; j++)
            screen->frame_sequence[screen->size.columns * i + j] = &screen->frame[i][j];
}


void feed_screen_frame(Screen *screen, uchar character) {
    for (ushort i = 0 ; i < screen->size.rows; i++)
        for (ushort j = 0 ; j < screen->size.columns; j++)
            screen->frame[i][j] = character;
}


void init_screen(Screen *screen, uchar is_global_screen) {
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);
    screen->location.x = 0;
    screen->location.y = 0;
    screen->size.rows = window.ws_row;
    screen->size.columns = window.ws_col;
    bound_screen_size(&screen->size);
    feed_screen_frame(screen, SCREEN_EMPTY_SPACE_CHARACTER);
    screen->is_on_menu = 0;
    screen->style_state_list_size = 0;
    is_resized = 0;

    // Constructing frame sequence
    construct_frame_sequence(screen);

    // Setting global screen
    if (is_global_screen)
        GLOBALSCR = screen;

    // Resize handler
    signal(SIGWINCH, handle_resize_signal);

    // ncurses init
    setlocale(LC_ALL, ""); // required in order to show ASCII emojis
    initscr();
    cbreak();
    noecho();

    // Hiding cursor
    curs_set(0);

    // Allowing keyboard
    keypad(stdscr, 1);

    start_color();
}


void *draw_screen_frame_thread(void *arg) {
    Screen *screen = (Screen *) arg;
    while (1) {
        erase();
        for (ushort i = 0; i < screen->size.rows; i++) {
            if (is_resized) {
                endwin();
                refresh();
                init_screen(screen, memcmp(screen, GLOBALSCR, sizeof(Screen *)) == 0);
            }

            uchar *row_frame_sequence[screen->size.columns];
            for (ushort j = 0; j < screen->size.columns; j++)
                row_frame_sequence[j] = screen->frame_sequence[screen->size.columns * i + j];
            mvprintw(i, 0, *row_frame_sequence);

            for (uchar j = 0; j < screen->style_state_list_size; j++) {
                StyleState style_state = screen->style_state_list[j];
                attron(COLOR_PAIR(style_state.code));
                mvchgat(style_state.location.y, style_state.location.x, 
                style_state.length, style_state.style, 
                style_state.code, NULL);
                attroff(COLOR_PAIR(style_state.code));
            }
        }
        refresh();
    }
}


void draw_screen_frame(Screen *screen) {
    create_thread(arrow_key_hit_check_thread, NULL);
    create_thread(draw_screen_frame_thread, (void *) screen);
}

// ---- VTOutput
void prepare_to_print_center(Location *location, Size *context_size, 
    ushort longest_size, ushort rows, ushort y_offset) {
    
    if (context_size->columns - longest_size >= 0) {
        location->x = ((ushort) (context_size->columns - longest_size) / 2);
        location->y = ((ushort) (context_size->rows - rows) / 2) + y_offset;
    }
}


void vtput_new_line(Screen *screen) {
    screen->location.x = (screen->location.y != screen->size.rows ? screen->location.y++ : 1) == -1;
}


void vtput_horizontal_tab(Screen *screen) {
    screen->location.x += TABSIZE;
    if (screen->location.x > screen->size.columns)
        screen->location.x = (screen->location.y != screen->size.rows ? screen->location.y++ : 1) == -1;
}


void vtput_vertical_tab(Screen *screen) {
    if (screen->location.y != screen->size.rows)
        screen->location.y++;
}


void vtput_formfeed_pagebreak(Screen *screen) {
    if (screen->location.y != screen->size.rows)
        screen->location.y++;
}


void vtput_carriage_return(Screen *screen) {
    screen->location.x = 0;
}


uchar vtcheck_special_character(Screen *screen, uchar character, uchar buffer[], ushort *printed_buffer) {
    if (character == '\n') {
        vtput_new_line(screen);
        return 1;
    }
    else if (character == '\f') {
        vtput_formfeed_pagebreak(screen);
        return 1;
    }
    else if (character == '\t') {
        vtput_horizontal_tab(screen);
        return 1;
    }
    else if (character == '\v') {
        vtput_vertical_tab(screen);
        return 1;
    }
    else if (character == '\r') {
        vtput_carriage_return(screen);
        return 1;
    }
    return 0;
}


void vtputch(Screen *screen, uchar data) {
    if (screen->location.x != screen->size.columns && screen->location.y != screen->size.rows)
        *screen->frame_sequence[screen->size.columns * screen->location.y + screen->location.x++] = data;
}


void vtprintf(Screen *screen, uchar *fmt, ...) {
    if (!screen->is_on_menu) {
        va_list args;
        va_start(args, fmt);
        uchar buffer[SCREEN_MAX_PRINTABLE_CHARACTERS];
        vsnprintf(buffer, SCREEN_MAX_PRINTABLE_CHARACTERS, fmt, args);
        va_end(args);
        ushort size_of_buffer = strlen(buffer);
        ushort printed_buffer = 0;
        while (printed_buffer < size_of_buffer) {
            if (printed_buffer < screen->size.columns * screen->size.rows) {
                uchar character = buffer[printed_buffer++];
                if (vtcheck_special_character(screen, character, buffer, &printed_buffer) == 0) {
                    if (screen->location.x == screen->size.columns && screen->location.y != screen->size.columns)
                        vtput_new_line(screen);
                    vtputch(screen, character);
                }
            }
            else
                break;
        }
    }
    return;
}


void vtgprintf(uchar *fmt, ...) {
    if (GLOBALSCR != NULL && !GLOBALSCR->is_on_menu) {
        va_list args;
        va_start(args, fmt);
        uchar buffer[SCREEN_MAX_PRINTABLE_CHARACTERS];
        vsnprintf(buffer, SCREEN_MAX_PRINTABLE_CHARACTERS, fmt, args);
        vtprintf(GLOBALSCR, buffer);
        va_end(args);
    }
}


void vtclear(Screen *screen) {
    clear_color_state_lists(screen);
    for (uchar i = 0 ; i < screen->size.rows; i++)
        for (uchar j = 0; j < screen->size.columns; j++)
            screen->frame[i][j] = SCREEN_EMPTY_SPACE_CHARACTER;
    screen->location.x = 0;
    screen->location.y = 0;
}


void vtgclear(void) {
    vtclear(GLOBALSCR);
}

// ---- Style
void clear_color_state_lists(Screen *screen) {
    screen->style_state_list_size = 0;
}


uchar is_color_pair_exists(Screen *screen, CellColor cell_color) {
    for (ushort i = 0; i < screen->style_state_list_size; i++) {
        StyleState style_state = screen->style_state_list[i];
        if (style_state.colors.foreground == cell_color.foreground && style_state.colors.background == cell_color.background)
            return style_state.code;
    }
    return 0;
}


ushort add_screen_style(Screen *screen, Location *location, ushort range, short foreground, short background, uint style) {
    if (screen->style_state_list_size < SCREEN_MAX_PRINTABLE_CHARACTERS) {
        StyleState style_state;
        CellColor cell_color;
        cell_color.foreground = foreground;
        cell_color.background = background;
        style_state.colors = cell_color;
        style_state.code = is_color_pair_exists(screen, cell_color);
        if (style_state.code == 0) {
            style_state.code = (short) screen->style_state_list_size + 1;
            init_pair(style_state.code, style_state.colors.foreground, style_state.colors.background);
        }
        style_state.length = range;
        memcpy(&style_state.location, location, sizeof(Location));
        style_state.style = style;
        screen->style_state_list[screen->style_state_list_size++] = style_state;
        return style_state.code;
    }
    return 0;
}

// ---- System
ullong get_file_size(uchar *file_path) {
    FILE *fp = fopen(file_path, "rb");
    
    if (fp == NULL)
        return 0;

    if (fseek(fp, 0, SEEK_END) < 0) {
        fclose(fp);
        return 0;
    }

    long file_size = ftell(fp);
    fclose(fp);
    return file_size;
}


void get_cwd_path(uchar *file_path, ushort size_of_file_path) {
    strncpy(file_path, cwd, size_of_file_path);
}


uchar is_sproc_exists(void) {
    struct stat sb;
    return stat("/proc", &sb) == 0 && S_ISDIR(sb.st_mode);
}


void init_io_system(char* argv[]) {
    if (is_sproc_exists()) {
        #if OS_LINUX == 1
        readlink("/proc/self/exe", cwd, SYS_MAX_EXE_PATH_LENGTH);
        #elif OS_FREE_BSD == 1
        readlink("/proc/curproc/file", cwd, SYS_MAX_EXE_PATH_LENGTH);
        #elif OS_SOLARIS == 1
        readlink("/proc/self/path/a.out", cwd, SYS_MAX_EXE_PATH_LENGTH);
        #endif
    }
    else {
        if (argv[0][0] == '/') {
            strncpy(cwd, argv[0], SYS_MAX_EXE_PATH_LENGTH);
        }
        else if (strchr(argv[0], '/') != NULL) {
            #if OS_WINDOWS == 1
            // TODO: replacing windows code
                DWORD WINAPI GetCurrentDirectory(
                    _In_  DWORD  nBufferLength,
                    _Out_ LPTSTR lpBuffer
                );
            #else
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                strncat(cwd, argv[0] + 1, SYS_MAX_EXE_PATH_LENGTH - 1);
            }
            #endif
        }
    }
    for (ushort i = strlen(cwd) - 1; i >= 0; i--)
        if (cwd[i] == '/')
            break;
        else
            cwd[i] = '\0';
}