#include "thread.h"

thread *threads_pool[MAX_PARALLEL_THREADS];
ushort threads_pool_size = 0;

void init_threads(void) {
}

void terminate_thread(thread *thread) {
    pthread_cancel(*thread);
}

thread *create_thread(t_function func, void *arg) {
    if (threads_pool_size == MAX_PARALLEL_THREADS) {
        terminate_thread(threads_pool[--threads_pool_size]);
        free(threads_pool[threads_pool_size]);
        threads_pool[threads_pool_size] = NULL;
    }

    threads_pool[threads_pool_size] = (thread *) malloc(sizeof(thread));
    pthread_create(threads_pool[threads_pool_size], NULL, func, arg);
    return threads_pool[threads_pool_size++];
}

void run_thread(thread *thread) {
    pthread_join(*thread, NULL);
}