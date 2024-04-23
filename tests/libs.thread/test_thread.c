/*
 * License: ZOSL (Zef OpenSource License)
 * Version: 1.0
 * 
 * Copyright (c) 2024 Armin Asefi <https://github.com/0xDeviI>
 * 
 * Created Date: Friday, March 29th 2024, 8:16:13 pm
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


#include <stdio.h>
#include <check.h>
#include "../../src/libs/core/constants.h"
#include "../../src/libs/thread/thread.h"
#include "../../src/libs/time/time.h"


void *thread_function_a(void *arg) {
    // it does nothing
}

void *thread_increase(void *arg) {
    int *pValue = (int *) arg;
    for (int i = 0; i < 5; i++) {
        (*pValue)++;
        sleepmsf(1000);
    }
}

void *thread_decrease(void *arg) {
    int *pValue = (int *) arg;
    for (int i = 0; i < 5; i++) {
        (*pValue)--;
        sleepmsf(3000);
    }
}

START_TEST(test_create_thread)
{
    clear_thread_mem_pool();
    thread *t = create_thread(thread_function_a, NULL);
    ck_assert_ptr_nonnull(t);
}
END_TEST

START_TEST(test_terminate_thread)
{
    clear_thread_mem_pool();
    thread *t = create_thread(thread_function_a, NULL);
    terminate_thread(t);
    ck_assert_int_eq(threads_pool_size, 0);
    ck_assert_ptr_null(threads_pool[0]);
}
END_TEST

START_TEST(test_threads_pool_full)
{
    clear_thread_mem_pool();
    
    // Create threads until the pool is full
    for (int i = 0; i < MT_MAX_PARALLEL_THREADS; i++) {
        create_thread(thread_function_a, NULL);
    }

    // Create one more thread which should result in the termination of last thread
    thread *t = create_thread(thread_function_a, NULL);
    ck_assert_int_eq(threads_pool_size, MT_MAX_PARALLEL_THREADS);
    ck_assert_ptr_nonnull(t);
    ck_assert_ptr_eq(t, threads_pool[threads_pool_size - 1]);
}
END_TEST

START_TEST(test_clear_thread_mem_pool)
{
    clear_thread_mem_pool();
    // Check if all memory from the thread pool is cleared
    for (int i = 0; i < MT_MAX_PARALLEL_THREADS; i++) {
        ck_assert_ptr_null(threads_pool[i]);
    }
}
END_TEST

START_TEST(test_terminate_non_existing_thread)
{
    clear_thread_mem_pool();
    thread *t = NULL;
    terminate_thread(t); // Terminate a non-existing thread
    ck_assert_ptr_null(t);
}
END_TEST

START_TEST(test_memory_leaks)
{
    clear_thread_mem_pool();
    for (int i = 0; i < MT_MAX_PARALLEL_THREADS; i++)
        create_thread(thread_function_a, NULL);

    for (int i = 0; threads_pool_size > 0;)
        terminate_thread(threads_pool[i]);

    ck_assert_int_eq(threads_pool_size, 0);
}
END_TEST

START_TEST(test_null_arguments)
{
    clear_thread_mem_pool();
    terminate_thread(NULL); // Terminate with NULL argument
    thread *t = create_thread(NULL, NULL);
    ck_assert_ptr_null(t);
}
END_TEST

START_TEST(test_concurrency)
{
    clear_thread_mem_pool();
    int value = 0xFF;
    int *pValue = &value;
    create_thread(thread_increase, (void *) pValue);
    create_thread(thread_decrease, (void *) pValue);
    sleepmsf(4000);
    ck_assert_int_eq(value, 257);
}
END_TEST

// Implement other test cases similarly

Suite *thread_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Thread");

    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_create_thread);
    tcase_add_test(tc_core, test_terminate_thread);
    tcase_add_test(tc_core, test_threads_pool_full);
    tcase_add_test(tc_core, test_clear_thread_mem_pool);
    tcase_add_test(tc_core, test_terminate_non_existing_thread);
    tcase_add_test(tc_core, test_memory_leaks);
    tcase_add_test(tc_core, test_null_arguments);
    tcase_add_test(tc_core, test_concurrency);

    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = thread_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
