#ifndef GLOBALS_H
#define GLOBALS_H

#include <pthread.h>

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;
typedef void (*function)(void);
typedef void *(*t_function)(void *vargp); // a pointer to a thread function
typedef pthread_t thread;

#endif