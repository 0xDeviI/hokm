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
    ushort x;
    ushort y;
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
void vtput_new_line(Screen *screen);
void vtput_horizontal_tab(Screen *screen);
void vtput_vertical_tab(Screen *screen);
void vtput_formfeed_pagebreak(Screen *screen);
void vtput_carriage_return(Screen *screen);
void vtputch(Screen *screen, uchar data);
void vtprintf(Screen *screen, uchar *fmt, ...);

#endif // !IO_H