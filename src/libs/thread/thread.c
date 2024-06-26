/*
 * License: ZOSL (Zef OpenSource License)
 * Version: 1.0
 * 
 * Copyright (c) 2024 Armin Asefi <https://github.com/0xDeviI>
 * 
 * Created Date: Thursday, March 14th 2024, 2:47:53 am
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


#include "thread.h"

thread *threads_pool[MT_MAX_PARALLEL_THREADS];
ushort threads_pool_size = 0;

void terminate_thread(thread *_thread) {
    if (_thread != NULL) {
        pthread_cancel(*_thread);
        for (ushort i = 0; i < threads_pool_size; i++) {
            if (memcmp(_thread, threads_pool[i], sizeof(thread)) == 0) {
                free(threads_pool[i]);
                threads_pool[i] = NULL;
                for (ushort j = i; j < threads_pool_size - 1; j++) {
                    threads_pool[j] = threads_pool[j + 1];
                }
                threads_pool[--threads_pool_size] = NULL;
                break;
            }
        }
    }
}


thread *create_thread(t_function func, void *arg) {
    if (func == NULL)
        return NULL;

    if (threads_pool_size >= MT_MAX_PARALLEL_THREADS) {
        threads_pool_size = MT_MAX_PARALLEL_THREADS;
        terminate_thread(threads_pool[--threads_pool_size]);
    }

    threads_pool[threads_pool_size] = (thread *) malloc(sizeof(thread));
    pthread_create(threads_pool[threads_pool_size], NULL, func, arg);
    return threads_pool[threads_pool_size++];
}


void clear_thread_mem_pool(void) {
    for (ushort i = 0; i < threads_pool_size; i++)
        terminate_thread(threads_pool[i]);
}