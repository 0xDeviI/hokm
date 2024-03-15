#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <stdlib.h>
#include "../thread/thread.h"
#include <sys/ioctl.h>
#include <string.h>
#include <ncurses.h>
#ifdef __unix__
    #define OS_WINDOWS 0
    #include <unistd.h>
#elif defined(_WIN32)
    #define OS_WINDOWS 1
    #include <windows.h>
#endif
#include "../core/constants.h"
#include "../core/globals.h"

typedef struct Screen
{
    ushort rows;
    ushort columns;
    uchar frame[SCREEN_MAX_ROWS][SCREEN_MAX_COLUMNS];
    /* data */
} Screen;


// I/O data methods
void flush(void);
void get_uchar(uchar *output);
void get_string(uchar *output[]);

// I/O ctl methods
// void malloc_screen_frame(Screen *screen);
void bound_screen_size(Screen *screen);
void *screen_resize_thread(void *arg);
void feed_screen_frame(Screen *screen, uchar character);
void init_screen(Screen *screen);
void *draw_screen_frame_thread(void *arg);
void draw_screen_frame(Screen *screen);

#endif // !IO_H