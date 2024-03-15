#ifndef THREAD_H
#define THREAD_H

#include <stdlib.h>
#include <pthread.h>
#include "../core/constants.h"
#include "../core/globals.h"

extern thread *threads_cluster[];
extern ushort threads_cluster_size;
void init_threads(void);
void terminate_thread(thread *thread);
thread *create_thread(t_function func, void *arg);
void run_thread(thread *thread);

#endif // !THREAD_H