// Created by nat-s.skv@mail.ru on 28.10.2021.
#include <stdio.h>
#include <stdlib.h>

#include <max_ascending_seq.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <zconf.h>
#include <max_ascending_seq.h>

#define ARRAY_IS_NULL -1
#define NOT_CORRECT_SIZE -2
#define MEMORY_NO_ALLOCATED -3
#define THREAD_NOT_CREATE -4

typedef struct arg_struct {
    size_t start_pos;
    size_t end_pos;
    size_t len;
    const int * array;
} args;


void *find_len(void *arg) {
    args *limits = (args *) arg;

    if (limits->start_pos == limits->end_pos) {
        int res_len = 1;
        limits->len = res_len;
        return NULL;
    }

    size_t max_len = 0, checked_len = 1;
    for (size_t i = limits->start_pos + 1; i <= limits->end_pos; ++i) {
        if (i != limits->end_pos && limits->array[i - 1] < limits->array[i]) {
            ++checked_len;
        } else {
            if (i == limits->end_pos && limits->array[limits->end_pos - 1] <
                                        limits->array[limits->end_pos]) {
                ++checked_len;
            }
            if (checked_len > max_len) {
                max_len = checked_len;
            }
            checked_len = 1;
        }
    }
    limits->len = max_len;
    return NULL;
}

size_t get_max_asc_seq_len(const int *array, size_t size) {
    if (array == NULL) {
        printf("Array is NULL");
        return ARRAY_IS_NULL;
    }
    if (size < 1) {
        printf("Wrong size");
        return NOT_CORRECT_SIZE;
    }

    if (size == 1) {
        return size;
    }

    size_t count_thread = sysconf(_SC_NPROCESSORS_ONLN);

    size_t *array_limits;

    if (size < count_thread) {
        array_limits = (size_t *)malloc(size * sizeof(size_t));
        if (array_limits == NULL) {
            printf("No memory allocated");
            return MEMORY_NO_ALLOCATED;
        }
        for (size_t i = 0; i < size; ++i) {
            array_limits[i] = i;
        }
        count_thread = size;
    } else {
        array_limits = (size_t *) malloc(count_thread * sizeof(size_t));
        if (array_limits == NULL) {
            printf("No memory allocated");
            return MEMORY_NO_ALLOCATED;
        }
        for (size_t i = 0; i < count_thread - 1; ++i) {
            array_limits[i] = size / count_thread * (i + 1) - 1;
        }
        array_limits[count_thread - 1] = size - 1;
    }

    for (size_t i = count_thread - 1; i > 0; --i) {
        while (array[array_limits[i - 1]] < array[array_limits[i - 1] + 1]) {
            ++array_limits[i - 1];
            if (array_limits[i - 1] == array_limits[i]) {
                break;
            }
        }
    }

    size_t new_count_limits = count_thread;
    for (size_t i = count_thread - 1; i > 0; --i) {
        if (array_limits[i - 1] == array_limits[i]) {
            --new_count_limits;
        }
    }

    if (count_thread != new_count_limits) {
        size_t *new_limits;
        new_limits = (size_t*)malloc(new_count_limits * sizeof(size_t));
        if (new_limits == NULL) {
            printf("No memory allocated");
            return MEMORY_NO_ALLOCATED;
        }
        size_t j = 0;
        new_limits[j++] = array_limits[0];
        for (size_t i = 1; i < count_thread; ++i) {
            if (array_limits[i - 1] != array_limits[i]) {
                new_limits[j++] = array_limits[i];
            }
        }
        free(array_limits);
        array_limits = new_limits;
        count_thread = new_count_limits;
    }

    pthread_t *threads;
    threads = (pthread_t *) malloc(count_thread * sizeof(pthread_t));
    if (threads == NULL) {
        printf("No memory allocated");
        free(array_limits);
        return MEMORY_NO_ALLOCATED;
    }
    args **limits = (args **)malloc(count_thread * sizeof(args*));
    if (limits == NULL) {
        free(array_limits);
        free(threads);
        printf("No memory allocated");
        return MEMORY_NO_ALLOCATED;
    }
    for (size_t i = 0; i < count_thread; ++i) {
        limits[i] = (args *)malloc(sizeof(args));
        if (limits[i] == NULL) {
            printf("No memory allocated");
            return MEMORY_NO_ALLOCATED;
        }
        limits[i]->array = array;
        limits[i]->start_pos = i < 1 ? 0 : array_limits[i - 1] + 1;
        limits[i]->end_pos = array_limits[i];
    }

    for (size_t i = 0; i < count_thread; ++i) {
        int check_flag = pthread_create(&threads[i], NULL,
                                        find_len, (void *)limits[i]);
        if (check_flag != 0) {
            printf("Thread not create");
            return THREAD_NOT_CREATE;
        }
    }
    for (size_t i = 0; i < count_thread; ++i) {
        pthread_join(threads[i], NULL);
    }

    int max_len = limits[0]->len;
    for (size_t i = 1; i < count_thread; ++i) {
        if (limits[i]->len > (size_t)max_len) {
            max_len = limits[i]->len;
        }
    }
    for (size_t i = 0; i < count_thread; ++i) {
        free(limits[i]);
    }
    free(limits);
    free(threads);
    free(array_limits);
    return max_len;
}
