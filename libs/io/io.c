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
    screen->rows = window.ws_row;
    screen->columns = window.ws_col;
    bound_screen_size(screen);
    feed_screen_frame(screen, SCREEN_EMPTY_SPACE_CHARACTER);

    // ncurses init
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